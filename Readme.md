# <p align="center">CCCP(CCCP Crafter of Controlled Packages)</p>

the [CCCP](https://github.com/Soviet-Linux/CCCP) package manager (CCCP Crafter of Controlled Packages), uses either installer binaries produced with the —create argument or sources from a .spm (soviet package manager) file. 


Here is a list of the parameters that may be used with the CCCP.:
1. --install (installs a .spm files)
1. --binary (installs a binary package in .tar.gz format)
1. --create(creates a installer binary)


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
    "locations" : ["<location>","<other location>"]
}
```
if its a source package leave the location empty



