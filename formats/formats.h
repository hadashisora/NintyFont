// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is a header which lists all the supported
// font formats, their constructors and identifier
// functions
#pragma once

#define NUM_FORMATS 5

#include "fontbase.h"

template<class T> NintyFont::FontBase* openFontFileTemplate(std::string path)
{
    return new T(path);
}

struct FontFormatDescriptor
{
    NintyFont::FontBase *(* const ctor)(std::string); //Constructor for the specific font format
    bool (* const identifyFile)(uint8_t *); //Takes in 4 to 8 bytes at the start of the file to determine whether the file can be handled by this descriptor
    std::string (* const getFileExtensionString)(void); //Returns file extension string for the file select dialong (ex. "*.brfnt")
    std::string (* const getFileTypeString)(void); //Returns file type string for the file select dialog (ex. "NW4R font files (*.brfnt *.rfnt)")
};

extern FontFormatDescriptor fontDescriptors[NUM_FORMATS];
// extern NintyFont::FontBase *(* const fontConstructors[NUM_FORMATS])(std::string);
// extern bool (* const fontIndetificators[NUM_FORMATS])(uint8_t *);
// extern std::string (* const fontExtensions[NUM_FORMATS])(void);
// extern std::string (* const fontFileTypes[NUM_FORMATS])(void);
