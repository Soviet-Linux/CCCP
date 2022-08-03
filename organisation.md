In this file the functionement of the CCCP and the LIBSPM is detailled :

# Libspm
## Presentation
The libspm is a shared library (.so) that contains all of the major function of the Soviet Package Manager
It's written in C++.  
You can find the source in the ```src/libspm``` dir.
## Functionement
All the function of the lib are regrouped in the `soviet` namespace.
> `soviet::exec` 

All package manipulation function are contained in the `soviet::package`  class? They are called from a package object.
```cpp
#include "../../include/libspm.so"

soviet::package pkg;\
pkg.name = "neofetch";\
pkg.install();```
```
Package object most of the time are created with data contained in .spm files.  
The libspm also provides some utilities function like ` soviet::msg()`, `soviet::exec()` or `soviet::downloadFile()`.

## Issues
This Object Oriented approach isn't optimal (but it works), because it reduces overall modularity (From other languages , you cant just use `soviet::package` , you need to pass through help function like `cccp` that takes an action arguments and do all the work).  
It also gretly increases code complexity since everything must pass through the same function.
I think that a more functional approach would be better and simpler to refactor.

### TODO
Our team is working on a new version of the libspm that would use pure C and a completely functional organisation.

# CCCP

Here we call "cccp" the executable called from the command-line that parse agruments and send action to the libspm.  
```bash
cccp --install neofetch
``` 
> (sends INSTALL action to the libspm and neofetch parameter )   

--> _libspm_ 
```cpp
soviet::package pkg;
pkg.name = "neofetch"
pkg.install();
return 1;
```

This part is basically useless , we could have implemented it in the libspm , but we chose to keep it separate for modularity (This way we could push an update the the libspm without changing the executable).

We are working on other version of this program in other language to demonsrate the flexibility of the libspm.


