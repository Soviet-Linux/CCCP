# Easy Communist Markup Package

## why ? 
The actual package format used by the _libspm_ (json) , is complicated to parse and hard to read.  
It has been suggested many times that we should switch to a simpler alternative like TOML or YAML.

## goals 
The goal of this markup language , and packaging format , is to be simple to read and write , easy to parse and to be as close as possible to the actual json format.  
It should be easy to convert from json to this format and vice versa.  
It should be easy to convert from this format to a TOML or YAML format (we might have some troubles with special operators ).



## syntax
The syntax is really close to TOML , but with some differences and additional features : 
 - it doesnt support subsections.
 - if no operator is found the line is considered a a raw string and is added to the previous line (with a newline).
 - if a line starts with a `#` it is considered a comment and is ignored.
 - We support different types of key/value operators :
    - `=` : the default operator , it is used to assign a value to a key.
    - `+=` : it is used to append a value to a key.
    - `-=` : it is used to remove a value from a key.
    - `++` : it is used to increment a key.
    - `--` : it is used to decrement a key.
    - `>=` : it is used to set the key to be all superiors or equals numbers to the value.
    - `<=` : it is used to set the key to be all inferior or equals numbers to the value.

## implementation
The parser for the language will be written in C and integrated in the _libspm_.  
The implementation is really simple , it is just a parser that converts the markup to a C struct

## examples
 - ### Simple package model
_using TOML markdown syntax highlighting_
```toml
[info] # basic information about the package
name = <package name>
version = <package version>
type = <src or bin>
license = <package license>

[description] # package description , it can be on multiple lines and we support makdown
This package is really important.
It is essential to the system.
It is a dependency of many other packages.


[dependencies] # the dependencies version support operators are: =, '<','>','<=','>='.
<dependency name> >= <dependency version>
<dependency name> = <dependency version>
<dependency name> =< < <dependency version>

[makedeps] # the dependencies needed to build the package
<dependency name> >= <dependency version>
<dependency name> = <dependency version>

[install] # this is basically a bash script to install the package
sh autogen.sh
./configure --prefix=/usr
make
make DESTDIR=$BUILD_ROOT install

[special] # a script to run after the package is installed
echo "the package is installed"
echo "done..."

[locations] # the locations of the files installed by the package
/usr/bin/<binary name>
/usr/share/<data name>
<other locations>
```
_in the example above , the `<...>` are placeholders for the actual values_




