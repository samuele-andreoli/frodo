# FRODO! Take off the ring

 * **category**: Library
 * **copyright**: 2018 Samuele Andreoli
 * **license**: ASL 2.0 [[Apache License 2.0]](http://www.apache.org/licenses/LICENSE-2.0)

## Description
C implementation of the FRODO post quantum secure key exchange.

## Dependencies
In order to build this library you need the following dependencies:
 * The [milagro-crypto-c](https://github.com/milagro-crypto/milagro-crypto-c) library

They are automatically resolved when making the project.

## Build
You can build the project running the command
```
$ make
```

## Build individual files as executable
Individual files in src/ can be compiled into an executable.

In the src directory, compile the file as
```
$ gcc -std=c99 <filename>.c -I../include -I../install/amcl/include/ -L ../install/amcl/lib -lamcl_core -DHAS_MAIN -o <filename>
```
add the milagro crypto library to the ld path [assuming you are using ld as linker]
```
LD_LIBRARY_PATH=../install/amcl/lib:${LD_LIBRARY_PATH}
```
and run as
```
$ ./<filename>
```
This runs a debug main() that performs a fixed smoke test of the code in the file.