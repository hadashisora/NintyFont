// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file contains inline functions
// (not really macros, but eh) for accessing
// items of the property list based on predefined
// indices, specifically used for RFNT (the rest
// are defined in the included here ntrfontmacros.h
// as they are basically the same)
#pragma once

#include <cstdint>

#include "propertylist.h"
#include "formats/NFTR/ntrfontmacros.h"

//RVL-specific font property indexes
#define NITROFONT_PROP_RVL_IMAGEFORMAT 8U

namespace NintyFont::RVL::Format
{
    inline uint8_t getRvlFontImageFormat(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_RVL_IMAGEFORMAT)))->value;
    }

    inline void setRvlFontImageFormat(std::vector<PropertyList::PropertyBase *> *props, uint8_t val)
    {
        ((PropertyList::Property<uint8_t> *)(props->at(NITROFONT_PROP_RVL_IMAGEFORMAT)))->value = val;
    }
}
