# <p align="center">CCCP(CCCP Crafter of Controlled Packages)</p>

the [CCCP](https://github.com/Soviet-Linux/CCCP) package manager (CCCP Crafter of Controlled Packages), uses either installer binaries produced with the —create argument or sources from a .spm (soviet package manager) file. 


Here is a list of the parameters that may be used with the CCCP.:
1. --install (installs a .spm files)
1. --binary (installs a binary package in .tar.gz format)
1. --create (creates a installer binary)
1. --remove (removes a package)


Follow this example to create a package:
```
{
    "name" : "<package name>",
    "type" : "<src,bin,local>",
    "version" : "<version>",
    "dependencies" : ["<a dependencie>","<other dependencie>"],
    "info" : 
    {
        "download" : "<download command>",
        "build" : "<build command>",
        "special" : "<special command>"
    },
    "locations" : []
}
```
Name :
The name of the package
Types : 
"src" is for source packages that need to download the source files.
"bin" is for binary packages created using --create option.
"local" is for sources packages that have their sources in the SRC_DIR "/var/cccp/src"
Version :
The package version. (UNUSED)
Dependencies :
The package dependencies , including software needed to build or download the package. 
Download :
The command used to download the sources in "src" packages.(If its unused leave it blank).
Build :
The command used to build the package.The destination dir must be $BUILD_ROOT . (ex : "make && make prefix=$BUILD_ROOT install")
Special :
If you need to execute a command after installation.(Not yet implemented because nobody needs it rn , if you want you can dm on discord to add it )
Locations :
The location list is automatically filled by the PM when you install ou create a package. Leave it empty.



