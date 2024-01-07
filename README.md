# NaiveChainCpp

A naive blockchain implementation written in C++.

### Windows

This project builds out of the box in Visual Studio 2022 with an integrated
installation of vcpkg.

### Linux with CLion

1. Install vcpkg from https://github.com/Microsoft/vcpkg. (I assume it is installed in `~/tools/`)
2. Go to `File -> Settings -> Build, Execution, Deployment -> CMake`.
3. Find `CMake Options`.
4. Insert `-DCMAKE_TOOLCHAIN_FILE=~/tools/vcpkg/scripts/buildsystems/vcpkg.cmake`.

### Inspiration

This project is heavily inspired by the [NaiveChain tutorial](https://medium.com/@lhartikk/a-blockchain-in-200-lines-of-code-963cc1cc0e54)
written by Lauri Hartikka.

### Future updates

The final goal is to add networking to the node and share a blockchain among multiple clients.
