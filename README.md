<div align="center">

![](assets/algoticks_logo.png)


# Algoticks

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![Build Status](https://travis-ci.org/jkotra/algoticks.svg?branch=master)](https://travis-ci.org/jkotra/algoticks)

---
<pre>
Algoticks is an algorithmic trading simulator written in C.
</pre>



[READ: Getting started with Algoticks](https://stdin.top/posts/getting-started-with-algoticks/)

[READ: Writing an Algorithm for Algoticks](https://stdin.top/posts/write-algo-for-algoticks/)

</div>

---

## Building

### Release

Recommended Compiler: `GCC`

Recommended Standard: `C11`

Dependencies:
- [`JSON-C`](https://github.com/json-c/json-c)

```
mkdir bin
cd bin
cmake ..
make
```

upon successful compilation, the binary executable along with config files are found in `release` (`bin/release`) folder.

### Debug

```
mkdir bin
cd bin
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

upon successful compilation, the binary executable along with config files are found in `debug` (`bin/debug`) folder.

---

### Building on Windows (experimental)

* Install [JSON-C](https://github.com/json-c/json-c#building-on-unix-and-windows-with-vcpkg-)
* [Install C and C++ support in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019)
* Install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/)

`include` path is set to `C:/vcpkg/installed/x64-windows/include` in `CMakeLists.txt`. Edit the path if you installed `vcpkg` in some other directory.

`lib` path is set to `C:/vcpkg/installed/x64-windows/lib` in `CMakeLists.txt`. Edit the path if you installed `vcpkg` in some other directory.

```
mkdir bin
cd bin

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
mingw32-make.exe

```

---

## Usage

```
Usage: algoticks [OPTION...]
Algoticks - algorithmic trading simulator.

  -b, --benchmark            Benchmark mode
  -B, --benchmarkfile=FILE   Benchmark file.
  -C, --configfile=FILE      Config file.
  -d, --derivative           Derivative mode
  -D, --debug=LEVEL          Debug mode.
  -l, --live                 Live mode. Wait for date at EOF
  -s, --socket=PORT          Stream to/from socket.
  -S, --settingsfile=FILE    settings file.
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to <jagadeesh@stdin.top>.


```

### Derivative

Derivative is a new feature introduced in `v1.3`.

if derivative option is enabled (`--derivative`), the signals will be generated on `datasource`(considered here as `Index`) and position is taken on data from `derivative_datasource`.

`datasource` and `derivative_datasource` must have matching dates and are sorted.

 Derivative requires addition data source that should be specified in `config.json`:

```json
    "derivative": {
    
    "derivative_datasource": "NIFTY_JUL10500CE.csv",
    "derivative_interval": 0
    
    },
```

To use derivative mode, use `-d` arg.

---

## Feed Live data from a TCP socket.

(Linux Only)

from version `v1.4` live data can be fed into algoticks from a TCP socket. use `-s [PORT]` to listen to specific port on localhost(127.0.0.1) for data.

for example, client, refer to [stream_to_socket.py](tests/stream_to_socket.py)


### Callbacks

Callbacks are custom functions that are called if included in `config.json`

```json
    "callbacks": ["callbacks/log.so"],
```

---

## Testing

Algoticks uses [check.h Unit Testing Framework](https://libcheck.github.io/check/) for testing.

[How to install check.h?](https://libcheck.github.io/check/web/install.html)

To run all Tests:

```
cd tests
cmake .
make
./algoticks_tests
```

Output:
```
Running suite(s): Algoticks
100%: Checks: 13, Failures: 0, Errors: 0

```


---

### Timeline

Start of development: 13-Jun-2020
Repository made public: 02-Jul-2020

---

### Credits

Icons made by <a href="https://creativemarket.com/eucalyp" title="Eucalyp">Eucalyp</a> from <a href="https://www.flaticon.com/" title="Flaticon"> www.flaticon.com</a>
