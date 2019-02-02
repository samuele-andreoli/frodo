# FRODO! Take off the ring

 * **category**: Library
 * **copyright**: 2019 Samuele Andreoli
 * **license**: ISC License

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

## Test
You can run the tests for the project running the command
```
make test
```

The test report will be saved in the file `test_report.txt` in the project root directory

## Benchmarks
You can run the benchmarks for the project running the command
```
make bench
```

The benchmark report will be saved in the file `benchmark_report.txt` in the project root directory

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