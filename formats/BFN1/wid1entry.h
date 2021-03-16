// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <cstdint>
#include <vector>
#include <list>

#include "glyph.h"
#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::DOL::Format
{
    class WID1Entry
    {
    public:
        //Constructors
        WID1Entry(uint8_t w1, uint8_t w2);
        WID1Entry(BinaryTools::BinaryReader *br);
        //Methods
        WID1Entry *copy(void);
        static std::vector<WID1Entry *> *createWidthEntries(std::list<Glyph *> *glyphs);
        void serialize(BinaryTools::BinaryWriter *bw);
        ~WID1Entry();
        //Fields
        uint8_t w1;
        uint8_t w2;
    };
}
