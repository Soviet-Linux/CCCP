
// class stuff
#include "../../include/libspm.hpp"

void soviet::package::getSources ()
{
        msg(INFO,"[+] Getting sources for " + name + "...");
        var_spm(packagePath);
        msg(DBG3,"[+] packagePath: " + packagePath);
        std::string sources_cmd = format(" NAME=%s && VERSION=%s && URL=%s && cd %s && %s",name.c_str(),version.c_str(),url.c_str(),vars.MAKE_DIR.c_str(),info["download"].c_str());
        msg(DBG2,"Downloading sources with %s",sources_cmd.c_str());
        system(sources_cmd.c_str());
        msg(DBG2,"Downloading sources finished");

        


}
