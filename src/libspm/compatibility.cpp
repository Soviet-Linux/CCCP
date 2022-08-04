#include <sys/stat.h> 


// class stuff
#include "../../include/libspm.h"

// json stuff
#include "../../lib/nlohmann/json.hpp"

using namespace nlohmann;

int soviet::installAur(const std::string& pkg_name)
{
    mkdir(format("%s/%s",vars.TMP_DIR.c_str(),pkg_name.c_str()),0777);

    std::string PBUrl = format("https://aur.archlinux.org/cgit/aur.git/plain/PKGBUILD?h=%s",pkg_name.c_str());
    std::string RepoUrl = format("https://aur.archlinux.org/%s.git",pkg_name.c_str());

    std::string tmp_path = format("%s/%s/PKGBUILD",vars.TMP_DIR.c_str(),pkg_name.c_str());
    msg(DBG2,"Downloading %s to %s",PBUrl.c_str(),tmp_path.c_str());
    downloadFile(PBUrl, tmp_path);

    std::string download_cmd = format("git clone %s && mkdir $NAME-$VERSION && mv %s/* $NAME-$VERSION",RepoUrl.c_str(),pkg_name.c_str());
    installCompatible(tmp_path,download_cmd);
    return 0;

}
int soviet::installCompatible(const std::string& file_path,const std::string& OptDownload)
{
   
    std::string FileName = file_path.substr(file_path.find_last_of("/")+1,file_path.size());
    if (FileName == "PKGBUILD")
    {
        std::string AbsPath = soviet::format("%s/%s",soviet::cwd.c_str(),file_path.c_str());
        soviet::msg(soviet::INFO, "installing %s in archlinux compatibility mode", file_path.c_str());
        package ArchPkg;
        ArchPkg.type = "src";
        ArchPkg.FileType = SPM_FILE;
        std::string download_cmd;
        if (OptDownload ==  "")
        {
            download_cmd = soviet::format("mkdir $NAME-$VERSION && cp %s .",AbsPath.c_str());
        }
        else {
            download_cmd = OptDownload;
        }
        ArchPkg.name = soviet::arch2spm(file_path,download_cmd);
        msg(soviet::INFO,"PKGBUILD succesfully converted !");
        ArchPkg.packagePath = soviet::format("%s/%s.spm",soviet::cwd.c_str(),ArchPkg.name.c_str());
        ArchPkg.dataSpmPath = soviet::format("%s/%s.spm",soviet::vars.SPM_DIR.c_str(),ArchPkg.name.c_str());
        ArchPkg.install();

    }
    else
    {
        soviet::msg(soviet::level::FATAL, "%s files  arent compatible , sorry", FileName.c_str());
    }
}

std::string soviet::arch2spm (const std::string& arch_file,const std::string& arch_download)
{

    msg(DBG2,"Converting %s to spm",arch_file.c_str());

    bool withDownload = (arch_download.length() > 0);

    json spmJson = arch2json(arch_file);

    std::string SpmName = spmJson["name"].get<std::string>();

    /* Here i have a problem : i could parse the command used to build the package and basically rewrite libalpm myself 
    of i could just all makepkg. I choose to call makepkg , but it causes a little problem , what shoudl we do with the fucking PKGBUILD ? 
    If we had the url (from the aur for example) we could download it a install time.
    For now i will pack it into a source archive. 
    I'll leave an if(){...} else 
    */
    msg(DBG2,"Creating install and make commands");
    spmJson["info"]["make"] = "cccp-makepkg";
    spmJson["info"]["install"] = format("mv pkg/%s/* $BUILD_ROOT",SpmName.c_str());


    std::string spm_file = format("%s/%s.spm",cwd.c_str(),SpmName.c_str()); 


    if (withDownload)
    {
        msg(INFO,"Writing to spm file with download info!");
        spmJson["info"]["download"] = arch_download;
        msg(DBG3,"New spm json: %s",spmJson.dump(4).c_str());

        std::ofstream FinalSpm;
        FinalSpm.open(spm_file.c_str());
        FinalSpm << spmJson.dump(4);
        FinalSpm.close();
        msg(DBG3,"spm file is closed !");


    }
    else
    {
        msg(WARNING,"You gave no download command , i'll try to build without");
        msg(INFO,"Launching creation of an src.spm.tar.gz package archive with the PKGBUILD");
        spmJson["info"]["prepare"] = "mv ../PKGBUILD .";

        std::string tar_spm_file = format("%s.src.spm.tar.gz",spm_file.substr(0,spm_file.find_last_of(".")).c_str());  
        msg(DBG3,"New tar file name is : %s",tar_spm_file.c_str());
        std::string tmp_spm = vars.TMP_DIR + "/arch_spm.tmp";

        std::ofstream spm;
        spm.open(tmp_spm.c_str());
        spm << spmJson.dump(4);
        spm.close();
        package archPkg;
        archPkg.packagePath = tmp_spm;
        archPkg.createArchive(tar_spm_file, {arch_file});
        
    }
    return SpmName;

}


json soviet::arch2json(const std::string& PKGBUILD)
{
    std::string arch_json_string = exec(format("arch-parser %s",PKGBUILD.c_str()));


    json archParsed = json::parse(arch_json_string);

    json spmJson = json::parse(PATTERN);
    
    // this is a weird hack , it may not work
    if (archParsed["pkgname"].is_array())
    {
        spmJson["name"] = archParsed["pkgname"][0];
    }
    else {
        spmJson["name"] = archParsed["pkgname"].get<std::string>();
    }
    
    // ading type for compatibility with old spm files
    spmJson["type"] = "src";
    spmJson["version"] = archParsed["pkgver"].get<std::string>();
    for (int i = 0; i < archParsed["optdepends"].size(); i++)
    {
        std::string optdepends = archParsed["optdepends"][i];
        std::string::size_type pos = optdepends.find(":");
        if (pos != std::string::npos)
        {
            optdepends = optdepends.substr(0, pos);
        }
        spmJson["optionaldependencies"].push_back(optdepends);
    }
    msg(DBG2,"New spm json opdepends is : %s",spmJson["optionaldependencies"].dump(4).c_str());
    for (int i = 0; i < archParsed["makedepends"].size(); i++)
    {
        spmJson["makedependencies"].push_back(archParsed["makedepends"][i]);
    }
    msg(DBG2,"New spm json makedepends is : %s",spmJson["makedependencies"].dump(4).c_str());
    for (int i = 0; i < archParsed["depends"].size(); i++)
    {
        spmJson["dependencies"].push_back(archParsed["depends"][i]);
    }
    spmJson["url"] = archParsed["url"].get<std::string>();

    // convert json array to json string
    std::string licenseString = "";
    for (int i = 0; i < archParsed["license"].size(); i++)
    {
        licenseString.append(archParsed["license"][i].get<std::string>());
    }
    spmJson["license"] = licenseString;

    msg(DBG2,"New spm json license is : %s",spmJson["license"].dump(4).c_str());

    spmJson["description"] = archParsed["pkgdesc"].get<std::string>();

    return spmJson;
}



// This is some shit i implemented to create a download command from url
/*
        // do stuff to download 
        msg(DBG3,"Adding instruction to download PKGBUILD from %s",arch_url.c_str());
        msg(DBG3,"Checking if the url comes from vcs ( git, hg ) : %s",arch_url.substr(0,arch_url.find_first_of("+")).c_str())
        if (arch_url.substr(0,arch_url.find_first_of("+")) == "git" )
        {
            // for now , everything will crash is the repo name isnt the package name
            // TODO: implement a system to get the repo name from the url
            spmJson["download"] = format("git clone %s && mv $NAME $NAME-$VERSION",arch_url.c_str())
            msg(DBG2,"Downloading using git with %s",spmJson["download"].c_str())
            
        }
        else
        {
            
        }
*/