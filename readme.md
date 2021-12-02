# NintyFont
NintyFont is a program for editing Nintendo's binary fonts, written in C++ using Qt5 for GUI. It's meant as sort of expanded version of RoadrunnerWMC's BRFNTify-Next.

## Credits
* Tempus, RoadrunnerWMC - making the original [BRFNTify-Next](https://github.com/RoadrunnerWMC/BRFNTify-Next), from which this borrows the GUI design (and a lot of GUI code, since I wasn't very good with Qt when I started this), as well as GC/Wii texture codec code from TPLLib
* gdkchan, kwsch - 3DS texture decoding from [Ohana3DS-Rebirth](https://github.com/gdkchan/Ohana3DS-Rebirth)
* [Citra emulator](https://github.com/citra-emu/citra) for being a go-to reference on how to CMake
* TheDzeraora - improving the BRFNTify-Next idea to seamlessly support different font formats, binary IO, font format handling, BFTTF/BFOTF crypto, CTR texture encoding

## Notice
This software was made purely as a passion project by a single person, and there hasn't been a lot of testing done since I just don't have the time to do it. Thus, as per the GPL, I give no warranty on this s/w and just hope that with the limited testing I gave it, it ends up working for you too. Sorry for the inconvenience. If you find major bugs, please report them to the issue tracker on the repo.

## Supported formats
* BRFNT - Binary Revolution FoNT, used in many Wii titles utilizing NW4R package. Both opening and saving is supported. BRFNU/BRFNA aren't supported yet;
* NFTR/RTFN - Nitro FonT Resource, used by a lot of DS titles utilizing the NTR_SYSTEM SDK package. As well as normal format, the slightly different version used in GameFreak's Pocket Monsters BW/B2W2 games is also supported. Both opening and saving work;
* Pocket Monsters D/P/Pt/HG/SS fonts, supports both opening and saving;
* BCFNT - Binary Citrus FoNT, used in many 3DS titles utilizing old versions of NW4C. Both opening and saving work, but texture encoding hasn't been tested very thoroughly yet;
* BFTTF/BFOTF - Binary caFe TrueType Font / OpenType Font, a simple XOR encryption ontop of normal TTF/OTF fonts. Supports encryption/decryption of both WiiU and Switch variants, as well as Windows one (probably meant for development purposes, I doubt a commercial game would have a font encrypted with the Windows key).

### In progress / planned to support
* JUTResFont/FONTbfn1 - JUtility Resource Font (at least called so in the J library used in WindWaker), used by many 1st and 2nd party Nintendo GameCube and (early )Wii games. Opening is supported (but very much broken, cause I've not figured a clean way to do things with it yet), no saving. More testing needs to be done, as a lot of this is based on RE'd code from Zelda WW and a lot of guesswork;
* BFFNT - Binary caFe FoNT (or at least it used to be initially), initially a WiiU/Cafe NW4F format, then with later versions of NW4C also became a 3DS/CTR format, and with the release of the Switch/NX also became used there. Currently not implemented at all, needs WiiU and Switch texture codecs;
* (more may be supported later as per request).

# Building instructions
Basic build instructions for native compiling. For development you'd obviously need at least some knowledge on how to work with CMake files.

## Building for Linux
### Requirements
Note: package names are provided only for `apt`(Debian, Ubuntu, Mint, etc...)(use `apt-get` if your system doesn't have `apt`) and `pacman`(Arch). For distros using other package managers, I suggest searching online for proper package names.
* A computer running a Linux distro (duh)
* A build chain with C++17 support (preferrably GCC(`apt install build-essential`, `pacman -S base-devel`))
* Qt5 (`apt install qtbase5-dev`, `pacman -S qt5`)
* CMake (`apt install cmake`, `pacman -S cmake`)
* fmt (`apt install libfmt-dev`, `pacman -S fmt`)
* git (`apt install git`, `pacman -S git`)

### Building
```bash
git clone https://github.com/TheFearsomeDzeraora/NintyFont.git NintyFont
cd NintyFont
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
strip NintyFont
```
* the last command `strip NintyFont` is optional, since it just removes all symbols from the executable to make it smaller

This will produce an executable file named `NintyFont` in the `build` folder

## Building for Windows (MSYS2)
### Requirements
Note: everything other than MSYS2 itself has to be installed from the MSYS2 prompt (MSYS2 MinGW 64-bit)
* A computer running 64-bit version of Windows NT (Windows 7 and higher, since MSYS2 doesn't support XP and Vista anymore) (duh)
* [MSYS2](https://www.msys2.org/)
* A build chain with C++17 support (`pacman -S mingw-w64-x86_64-toolchain`)
* Qt5 (`pacman -S mingw-w64-x86_64-qt5`)
* CMake (`pacman -S mingw-w64-x86_64-cmake`)
* fmt (`pacman -S mingw-w64-x86_64-fmt`)
* git (`pacman -S git`)

### Building
For ease of use a batch script is provided for automatically compiling this (certainly __not__ because I was tired of recalling these commands on another computer just to make Windows build myself)
```bash
git clone https://github.com/TheFearsomeDzeraora/NintyFont.git NintyFont
cd NintyFont
./msys2_build.sh
```
This will produce `NintyFont.exe` in the `build` folder. Note that you also need the supporting DLLs for this to run (which can be found in the `mingw64/bin` folder of your MSYS2 install directory)(if you're too lazy to hunt these down, just grab a prebuilt release and replace the EXE there with the one you compiled)(alternitively you may try doing a static build, but in my experience Qt doesn't always like being linked statically).

## Building for Windows (MinGW-w64 static cross-compile under Linux)
### Requirements
Note: packages are listed only for Ubuntu/Mint(`apt`), may also work on Debian, but I don't know package names under other distros.
* A computer running a Linux distro (duh)
* MinGW-w64 (`sudo apt install mingw-w64 mingw-w64-tools`)
* Static Qt5 compiled for MinGW (good luck with this) (you can try following [a guide I made](https://gist.github.com/TheFearsomeDzeraora/6444cea25c64eb887b4c3462b125c441), though it may or may not work for you)
* Static fmt compiled for MinGW (it uses CMake as well, so you can just use the toolchan file provided here(`mingw-w64-x86_64.cmake`) to build it)
* CMake (`sudo apt install cmake`)
* git (`sudo apt install git`)

### Building
```bash
git clone https://github.com/TheFearsomeDzeraora/NintyFont.git NintyFont
cd NintyFont
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-x86_64.cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC=1 ..
make -j$(nproc)
x86_64-w64-mingw32-strip NintyFont.exe
```
* the last command `x86_64-w64-mingw32-strip NintyFont.exe` is optional, since it just removes all symbols from the executable to make it smaller

This will produce an executable file named `NintyFont.exe` in the `build` folder

## Building for OS X / macOS
\*To be done\*

## Building for Windows (MSVC)
* `goto "Building on Windows (MSYS2)";` :)
