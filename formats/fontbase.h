// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes a base
// class for other font classes
#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "glyph.h"
#include "propertylist.h"

namespace NintyFont
{
    enum class Platforms : uint8_t
    {
        Dolphin,
        Revolution,
        Cafe,
        Nx,
        Nitro,
        Ctr
    };

    enum class CharEncodings : uint8_t
    {
        UTF8,
        UTF16,
        ShiftJIS,
        CP1252,
        Num
    };

    enum class ImageFormats : uint8_t
    {
        None, //Fallback option, just in case
        L2, //DS gen 4 pokemon fonts, 2 bit luminance
        L2A,
        L4, //I4, L4, 4 bit luminance
        L4A, //IA4, LA4, 4 bit luminance + 4 bit alpha
        L8, //I8, L8, 8 bit luminance
        HL8, //CTR HL8
        A8, //CTR A8, 8 bit alpha-only, identical to L8
        A4, //CTR a4, 4 bit alpha-only, identical to L4
        L8A, //IA8, LA8, 8 bit luminance + 8 bit alpha
        RGB565, //5 bit red + 6 bit green + 5 bit blue
        RGB5A1, //5 bpc RGB + 1 bit alpha
        RGB5A3, //4 bpc RGB + 3 bit alpha or 5 bpc RGB, based on the first bit
        RGBA4, //4 bpc RGBA
        RGB8, //8 bpc RGB
        RGBA8, //8 bpc RBGBA
        ETC1, //CTR ETC1, Ericsson Texture Compression 1
        ETC1A4, //CTR ETC1A4, ETC1 with 4 bits of alpha
    };

    class FontBase
    {
    public:
        //Methods
        //Most of these should be made into pure virtual at some point
        virtual void saveBinaryFont(std::string filePath);
        virtual std::vector<PropertyList::PropertyListEntryDescriptor *> *getGlyphPropertyDescriptors(void);
        virtual std::vector<PropertyList::PropertyBase *> *getPropertiesList(void);
        virtual std::vector<PropertyList::PropertyBase *> *getDrawablePropertiesList(void);
        virtual std::pair<uint32_t, uint32_t> getGlyphImageSize(void);
        virtual bool canDeleteGlyphs(void);
        virtual bool canCreateCopyGlyphs(void);
        virtual Glyph *createEmptyGlyph(void);
        virtual std::string getImageFormatString(ImageFormats format);
        virtual std::string getCharEncodingString(CharEncodings format);
        virtual void drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns);
        //Destructor
        virtual ~FontBase();
        //Fields
        bool edited;
        std::string fontPath;
        std::vector<Glyph *> glyphs;
    };
}
