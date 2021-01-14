rm -R build
mkdir build && cd build
cmake -G "MSYS Makefiles" -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=Release ..
mingw32-make -j4
strip NintyFont.exe