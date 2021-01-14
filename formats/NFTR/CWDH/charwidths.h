// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for CWDH width item
#pragma once

#include <cstdint>
#include <vector>
#include <list>

#include "glyph.h"
#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::NTR::Format
{
    class CharWidths
    {
    public:
        //Constructors
        CharWidths(int8_t t_left, uint8_t t_glyphWidth, uint8_t t_charWidth);
        CharWidths(BinaryTools::BinaryReader *br);
        //Methods
        CharWidths *copy(void);
        static std::vector<CharWidths *> *createWidthEntries(std::list<Glyph *> *glyphs);
        virtual void serialize(BinaryTools::BinaryWriter *bw);
        ~CharWidths();
        //Fields
        int8_t left; //left space width of character
        uint8_t glyphWidth; //glyph width of character
        uint8_t charWidth; //character width = left space width + glyph width + right space width
    };
}
