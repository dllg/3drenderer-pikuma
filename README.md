# 3drenderer-pikuma

Simple C application created when doing the course "Learning graphics programming" at pikuma.com.

# Building

Make sure you have a compiler installed, [cmake](https://cmake.org/) and [vcpkg](https://github.com/microsoft/vcpkg).

On mac:

```
$ pushd
$ xcode-select --install
$ brew install cmake
$ mkdir -p repos
$ cd repos
$ git clone git@github.com:microsoft/vcpkg.git
$ cd vcpkg
$ ./bootstrap-vcpkg.sh
$ ./vcpkg install sdl2
$ popd
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Running

```
$ ./build/bin/renderer
```
