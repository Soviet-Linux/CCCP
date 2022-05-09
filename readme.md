# <p align="center">CCCP(CCCP Crafter of Controlled Packages)</p>


## [![Repography logo](https://images.repography.com/logo.svg)](https://repography.com) / Recent activity [![Time period](https://images.repography.com/25154606/Soviet-Linux/CCCP/recent-activity/5adba74725fc50e43e2d9a4804862db8_badge.svg)](https://repography.com)
[![Timeline graph](https://images.repography.com/25154606/Soviet-Linux/CCCP/recent-activity/5adba74725fc50e43e2d9a4804862db8_timeline.svg)](https://github.com/Soviet-Linux/CCCP/commits)
[![Issue status graph](https://images.repography.com/25154606/Soviet-Linux/CCCP/recent-activity/5adba74725fc50e43e2d9a4804862db8_issues.svg)](https://github.com/Soviet-Linux/CCCP/issues)
[![Pull request status graph](https://images.repography.com/25154606/Soviet-Linux/CCCP/recent-activity/5adba74725fc50e43e2d9a4804862db8_prs.svg)](https://github.com/Soviet-Linux/CCCP/pulls)
[![Trending topics](https://images.repography.com/25154606/Soviet-Linux/CCCP/recent-activity/5adba74725fc50e43e2d9a4804862db8_words.svg)](https://github.com/Soviet-Linux/CCCP/commits)
[![Top contributors](https://images.repography.com/25154606/Soviet-Linux/CCCP/recent-activity/5adba74725fc50e43e2d9a4804862db8_users.svg)](https://github.com/Soviet-Linux/CCCP/graphs/contributors)

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
1. Name = The name of the package

1. Types :
    1. "src" is for source packages that need to download the source files.

    1. "bin" is for binary packages created using --create option.

    1. "local" is for sources packages that have their sources in the
    
    SRC_DIR "/var/cccp/src"



1. Version = The package version. (UNUSED)

1. Dependencies = Package dependencies, such as software required to build or download the package.

1. Download = The command for downloading the sources in "src" packages. (If it is not used, leave it blank.)

1. Build = The command used to build the package. The target directory must be $BUILD_ROOT.
   
   (example: `make && make prefix=$BUILD_ROOT install`)

1. Special =
If you need to run a command after the installation is complete. (This is not yet implemented.)

1. Locations =
When you install or build a package, CCCP automatically generates the location list. Leave it blank.


