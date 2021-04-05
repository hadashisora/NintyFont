cmake_minimum_required(VERSION 3.5)

project(NintyFont LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(fmt)

if(MSVC)
    message(FATAL_ERROR "Use MSYS2 instead of MSVC, please.")
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    globalstuffs.h
    mainwindow.cpp
    mainwindow.h
    glyph.cpp
    glyph.h
    viewwidget.cpp
    viewwidget.h
    fontinfopanel.cpp
    fontinfopanel.h
    importsheetwindow.cpp
    importsheetwindow.h
    qfilepathfield.cpp
    qfilepathfield.h
    binarytools/endianness.h
    binarytools/endianness.cpp
    binarytools/binaryreader.cpp
    binarytools/binaryreader.h
    binarytools/bitreader.cpp
    binarytools/bitreader.h
    binarytools/binarywriter.cpp
    binarytools/binarywriter.h
    binarytools/blocklinker.cpp
    binarytools/blocklinker.h
    formats/formats.cpp
    formats/formats.h
    formats/fontbase.cpp
    formats/fontbase.h
    formats/NFTR/ntrfontmacros.h
    formats/NFTR/nitrofontresource.cpp
    formats/NFTR/nitrofontresource.h
    formats/NFTR/finf.cpp
    formats/NFTR/finf.h
    formats/NFTR/cglp.cpp
    formats/NFTR/cglp.h
    formats/NFTR/nftr.cpp
    formats/NFTR/nftr.h
    formats/NFTR/CMAP/cmap.cpp
    formats/NFTR/CMAP/cmap.h
    formats/NFTR/CMAP/cmapentry.cpp
    formats/NFTR/CMAP/cmapentry.h
    formats/NFTR/CWDH/cwdh.cpp
    formats/NFTR/CWDH/cwdh.h
    formats/NFTR/CWDH/charwidths.cpp
    formats/NFTR/CWDH/charwidths.h
    formats/RFNT/rvlfontmacros.h
    formats/RFNT/binaryrevolutionfont.cpp
    formats/RFNT/binaryrevolutionfont.h
    formats/RFNT/rfnt.cpp
    formats/RFNT/rfnt.h
    formats/RFNT/tglp.cpp
    formats/RFNT/tglp.h
    formats/BFN1/jutresfont.cpp
    formats/BFN1/jutresfont.h
    formats/BFN1/fontbfn1.cpp
    formats/BFN1/fontbfn1.h
    formats/BFN1/inf1.cpp
    formats/BFN1/inf1.h
    formats/BFN1/wid1.cpp
    formats/BFN1/wid1.h
    formats/BFN1/wid1entry.cpp
    formats/BFN1/wid1entry.h
    formats/BFN1/gly1.cpp
    formats/BFN1/gly1.h
    formats/BFN1/map1.cpp
    formats/BFN1/map1.h
    formats/PM4F/pocketmonstersgen4font.cpp
    formats/PM4F/pocketmonstersgen4font.h
    formats/PM4F/pmheader.cpp
    formats/PM4F/pmheader.h
    formats/conv/bfttf.cpp
    formats/conv/bfttf.h
    formats/CFNT/binarycitrusfont.cpp
    formats/CFNT/binarycitrusfont.h
    formats/CFNT/cfnt.cpp
    formats/CFNT/cfnt.h
    image/texturecodec.h
    image/revolution.cpp
    image/ctr.cpp
    image/nitro.cpp
    image/cafe.cpp
    image/nx.cpp
    qhexspinbox.cpp
    qhexspinbox.h
    unicodenames.cpp
    unicodenames.h
    sjisconv.cpp
    sjisconv.h
    propertylist.cpp
    propertylist.h
    charproppanel.cpp
    charproppanel.h
    aboutwindow.cpp
    aboutwindow.h
    controls/controls.h
    controls/control.h
    controls/codepointpicker.cpp
    controls/codepointpicker.h
    controls/spinbox.cpp
    controls/spinbox.h
    controls/label.cpp
    controls/label.h
    controls/boolpicker.cpp
    controls/boolpicker.h
    controls/endiannesspicker.cpp
    controls/endiannesspicker.h
    membercallback.h
    sizedarray.h
    )

configure_file("DerivedName.txt" "DerivedName.txt" COPYONLY)
configure_file("SHIFTJIS.TXT" "SHIFTJIS.TXT" COPYONLY)

#set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE FALSE)
#It's no use...Qt5 forces -fPIC no matter what, since the Ubuntu apt packages
#for it are compiled with "-reduce-relocations" flag presumably forcing use of PIC.
#This has a side effect that this program can't be launched by double clicking
#the file in Nemo/Nautilus, since the mime type of PIE is of shared library...
#Still beats me as to why this hasn't been fixed yet...

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::WinMain)
    if(MINGW)
        if (STATIC)
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-mwindows -static-libgcc -static-libstdc++ -static")
        else()
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-mwindows -static-libgcc -static-libstdc++")
        endif()
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets fmt::fmt)
