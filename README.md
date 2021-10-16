# Yo motherfucker fork this!!!

Ha!!

Hi Sam, what's up. Me and the rest of the team here miss ya dude. Don't be a stranger. You keep in touch and if you need anything let us know.

We seem to be doing allright, the company is on fire. Over 2B in deposits now. You build that motherfucker!! Ask your profs if they have done something equally as remarkable in their 20's if they start giving you shit.

You wouldn't recognize the company now over 100 people.

Anyway, if you ever get back to London way make sure we know, I'd love to take you out for that final drink we never got to do.

Cheers,
Brian

 * **category**: Library
 * **copyright**: 2019 Samuele Andreoli
 * **license**: ISC License

## Description
C implementation of the FRODO post quantum secure key exchange.

## Dependencies
In order to build this library you need the following dependencies:
 * The [milagro-crypto-c](https://github.com/apache/incubator-milagro-crypto-c.git) library
 * [OpenSSL](https://github.com/openssl/openssl)

The milagro-crypto-c dependency is automatically resolved when making the project.

## Build
You can build the project running the command
```
$ make build
```

#### Build configuration
The build can be configured using a few flags.
* BACKEND allows to specify the crypto library to use as backend. The supported backends are milagro-crypto-c [1] and OpenSSL [2]
* OPTIMIZATION allows to specify if the library should be optimized for memory usage [1] or speed [2]
* FRODO_CONFIGURATION allows to specify the desired configuration for Frodo: TOY [1], CLASSIC [2], RECOMMENDED [3] or PARANOID [4]

Additional configurations are listed in the `Configuration` section of the Makefile

## Install
You can install the project running the command
```
$ make install
```

The default path for install is `install/`

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

## Run examples
In order to run the examples you need to add milagro and frodo to your dynamic linker path. Assuming you are using ld as linker, you can use

```
LD_LIBRARY_PATH=../install/amcl/lib:../install/frodo/lib:${LD_LIBRARY_PATH}
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
