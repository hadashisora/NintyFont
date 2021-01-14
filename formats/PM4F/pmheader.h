// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes binary structure
// of PM gen4 game font header
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::NTR::PM4::Format
{
    class PMHeader
    {
    public:
        //Ctors
        PMHeader(BinaryTools::BinaryReader *br);
        PMHeader(uint32_t glyphCount, uint8_t maxWidth, uint8_t maxHeight);
        //Dtors
        ~PMHeader();
        //Methods
        void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t ptrImage;
        uint32_t ptrWidths;
        uint32_t glyphCount;
        uint8_t maxWidth;
        uint8_t maxHeight;
        uint8_t letterCharX;
        uint8_t letterCharY;
    };
}
