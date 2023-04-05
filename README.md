# xx2dw
xx2d's web gl 2 version

# windows tutorial

1. download & install msys2: https://www.msys2.org/
2. run "MSYS2 MinGW x64" shortcut, execute: packman -S mingw-w64-x86_64-make
3. config windows env path: ???:????\msys64\mingw64\bin
4. copy mingw32-make.exe to make.exe
5. git clone https://github.com/emscripten-core/emsdk.git & install & actiive

build script:
```
mkdir build
cd build
emcmake cmake ..
emmake make -j16
```

# online demo

https://denghe.github.io/xx2dw/
