// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for CWDH (Character WiDtH) binary data
// Reused by all newer NW font formats to date,
// somewhat based on BFN1's WID1
#pragma once

#include <cstdint>
#include <vector>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "charwidths.h"

namespace NintyFont::NTR::Format
{
    class CWDH
    {
    private:
        uint32_t entriesOffset; //Used to keep track of where to read the entries from
    public:
        //Ctor
        CWDH(std::vector<CharWidths *> *entries, uint32_t magic = 0x43574448U);
        CWDH(BinaryTools::BinaryReader *br);
        //Dtor
        ~CWDH();
        //Methods
        virtual void validateSignature(void);
        virtual void readEntries(BinaryTools::BinaryReader *br);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic; //Should always be 0x43574448, CWDH in ASCII
        uint32_t length; //CWDH section length in bytes
        uint16_t indexBegin; //First glyph index
        uint16_t indexEnd; //Last glyph index
        uint32_t ptrNext; //Pointer to the next (next CWDH + 0x8), 0x0 if last
        std::vector<CharWidths *> *entries; //CharWidths entries are stored after the header
    };
}
