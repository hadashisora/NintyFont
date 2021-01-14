// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file contains inline functions
// for simplifying access to property list
// items for NFTR files (and anything based
// on it, such as RFNT/CFNT/FFNT and probably
// whatever they come up with in the next
// NintendoWare)
#pragma once

#include <cstdint>
#include <vector>

#include "propertylist.h"
#include "formats/fontbase.h"

//Defines for glyph property indexes
#define NITROGLYPH_PROP_INDEX 0U
#define NITROGLYPH_PROP_CODE 1U
#define NITROGLYPH_PROP_LEFT 2U
#define NITROGLYPH_PROP_GLYPHWIDTH 3U
#define NITROGLYPH_PROP_CHARWIDTH 4U

//Defines for font property indexes
//Shared
#define NITROFONT_PROP_ENDIANNESS 0U
#define NITROFONT_PROP_CHARENC 1U
#define NITROFONT_PROP_LINEFEED 2U
#define NITROFONT_PROP_HEIGHT 3U
#define NITROFONT_PROP_WIDTH 4U
#define NITROFONT_PROP_ASCENT 5U
#define NITROFONT_PROP_BASELINE 6U
#define NITROFONT_PROP_VERSION 7U
//NTR-specific
#define NITROFONT_PROP_NTR_BPP 8U
#define NITROFONT_PROP_NTR_VERTICAL 9U
#define NITROFONT_PROP_NTR_ROTATION 10U
#define NITROFONT_PROP_NTR_GAMEFREAK 11U

//Inline functions for properties, better than macros anyways
namespace NintyFont::NTR::Format
{
    //NITRO GLYPH

    inline uint16_t getGlyphIndex(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint16_t> *)(props->at(NITROGLYPH_PROP_INDEX)))->value;
    }

    inline uint16_t getGlyphCode(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint16_t> *)(props->at(NITROGLYPH_PROP_CODE)))->value;
    }

    inline int8_t getGlyphLeft(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<int8_t> *)(props->at(NITROGLYPH_PROP_LEFT)))->value;
    }

    inline uint8_t getGlyphGlyphWidth(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROGLYPH_PROP_GLYPHWIDTH)))->value;
    }

    inline uint8_t getGlyphCharWidth(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROGLYPH_PROP_CHARWIDTH)))->value;
    }

    inline void setGlyphIndex(std::vector<PropertyList::PropertyBase *> *props, uint16_t val)
    {
        ((PropertyList::Property<uint16_t> *)(props->at(NITROGLYPH_PROP_INDEX)))->value = val;
    }

    inline void setGlyphCode(std::vector<PropertyList::PropertyBase *> *props, uint16_t val)
    {
        ((PropertyList::Property<uint16_t> *)(props->at(NITROGLYPH_PROP_CODE)))->value = val;
    }

    inline void setGlyphLeft(std::vector<PropertyList::PropertyBase *> *props, int8_t val)
    {
        ((PropertyList::Property<int8_t> *)(props->at(NITROGLYPH_PROP_LEFT)))->value = val;
    }

    inline void setGlyphGlyphWidth(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROGLYPH_PROP_GLYPHWIDTH)))->value = val;
    }

    inline void setGlyphCharWidth(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROGLYPH_PROP_CHARWIDTH)))->value = val;
    }

    //NITRO FONT

    inline bool getFontEndianness(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<bool> *)(props->at(NITROFONT_PROP_ENDIANNESS)))->value;
    }

    inline uint8_t getFontCharEncoding(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_CHARENC)))->value;
    }

    inline uint8_t getFontLinefeed(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_LINEFEED)))->value;
    }

    inline uint8_t getFontHeight(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_HEIGHT)))->value;
    }

    inline uint8_t getFontWidth(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_WIDTH)))->value;
    }

    inline uint8_t getFontAscent(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_ASCENT)))->value;
    }

    inline uint8_t getFontBaseline(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_BASELINE)))->value;
    }

    inline uint16_t getFontVersion(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint16_t> *)(props->at(NITROFONT_PROP_VERSION)))->value;
    }

    inline void setFontEndianness(std::vector<PropertyList::PropertyBase *> *props, bool val)
    {
        ((PropertyList::Property<bool> *)(props->at(NITROFONT_PROP_ENDIANNESS)))->value = val;
    }

    inline void setFontCharEncoding(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_CHARENC)))->value = val;
    }

    inline void setFontLinefeed(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_LINEFEED)))->value = val;
    }

    inline void setFontHeight(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_HEIGHT)))->value = val;
    }

    inline void setFontWidth(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_WIDTH)))->value = val;
    }

    inline void setFontAscent(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_ASCENT)))->value = val;
    }

    inline void setFontBaseline(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_BASELINE)))->value = val;
    }

    inline void setFontVersion(std::vector<PropertyList::PropertyBase *> *props, uint16_t val)
    {
        ((PropertyList::Property<uint16_t> *)(props->at(NITROFONT_PROP_VERSION)))->value = val;
    }

    //NTR-only

    inline uint8_t getNtrFontBpp(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_NTR_BPP)))->value;
    }

    inline uint8_t getNtrFontVertical(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<bool> *)(props->at(NITROFONT_PROP_NTR_VERTICAL)))->value;
    }

    inline uint8_t getNtrFontRotation(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_NTR_ROTATION)))->value;
    }

    inline void setNtrFontBpp(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_NTR_BPP)))->value = val;
    }

    inline void setNtrFontVertical(std::vector<PropertyList::PropertyBase *> *props, bool val)
    {
        ((PropertyList::Property<bool> *)(props->at(NITROFONT_PROP_NTR_VERTICAL)))->value = val;
    }

    inline void setNtrFontRotation(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_NTR_VERTICAL)))->value = val;
    }
}
