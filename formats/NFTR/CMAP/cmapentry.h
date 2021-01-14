// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for CMAP entry item
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"

namespace NintyFont::NTR::Format
{
    class CMAPEntry
    {
    public:
        //Constructors
        CMAPEntry(uint16_t t_code, uint16_t t_index);
        CMAPEntry(BinaryTools::BinaryReader *br);
        //Methods
        virtual void serialize(BinaryTools::BinaryWriter *bw);
        ~CMAPEntry();
        //Fields
        uint16_t code; //Character code
        uint16_t index; //Glyph index
    };
}
