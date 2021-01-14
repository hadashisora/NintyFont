// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <cstdint>
#include <vector>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "formats/NFTR/CWDH/charwidths.h"

namespace NintyFont::DOL::Format
{
    class WID1
    {
    public:
        //Ctors
        WID1(BinaryTools::BinaryReader *br);
        //Dtor
        ~WID1();
        //Methods
        virtual void validateSignature(void);
        virtual void readEntries(BinaryTools::BinaryReader *br);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic;
        uint32_t length;
        uint16_t startCode; //Code point of first character
        uint16_t endCode; //Code point of last character
        std::vector<NTR::Format::CharWidths *> *entries;
    };
}
