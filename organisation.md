This file describes how the CCCP and the LibSPM work:

# LibSPM


## Presentation
The LibSPM is a shared library (.so) that contains all of the major functions of the Soviet Package Manager.

It's written in C++.
The source code is in the `src/libspm` directory.


## Functioning

The functions of the library have been reorganized in the `soviet` namespace.
> `soviet::exec` 

The `soviet::package` class contains all package manipulation functions. They are accessed through a package object.
```cpp
#include "../../include/libspm.so"

soviet::package pkg;\
pkg.name = "neofetch";\
pkg.install();
```
The majority of the time, package objects are created using data from.spm files.
The libspm also includes utility functions such as `soviet::msg()`, `soviet::exec()`, and `soviet::downloadFile()`.





## Issues
This object-oriented approach isn't ideal (but it works), because it reduces overall modularity (in other languages, you can't just use `soviet::package`, you have to pass through a help function like 'cccp' that takes action arguments and does all the work). Because everything must pass through the same function, it greatly increases code complexity. A more functional approach, in my opinion, would be better and easier to refactor.





### TODO
Our team is working on a new version of the LibSPM that will be written in pure C and will have a fully functional organization.





# CCCP

Here, "cccp" refers to the command-line executable that parses agruments and sends action to the libspm.
```bash
cccp --install neofetch
``` 
> (INSTALL action is sent to the libspm and neofetch as parameter)

--> _libspm_
```cpp
soviet::package pkg;
pkg.name = "neofetch"
pkg.install();
return 1;
```

We could have implemented this section in the libspm, but we chose to keep it separate for modularity (this way we could push an update the libspm without changing the executable). We're working on another version of this program in another language to demonstrate the libspm's flexibility.