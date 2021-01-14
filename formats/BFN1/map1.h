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
#include "formats/NFTR/CMAP/cmapentry.h"

namespace NintyFont::DOL::Format
{
    class MAP1
    {
    private:
        uint32_t entriesOffset; //Used to keep track of where to read the entries from, not in the actual data
    public:
        //Ctors
        MAP1(BinaryTools::BinaryReader *br);
        //Dtor
        ~MAP1();
        //Methods
        virtual void validateSignature(void);
        virtual void readEntries(BinaryTools::BinaryReader *br);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic;
        uint32_t length;
        uint16_t fontType; //Aka mapping method
        uint16_t startCode;
        uint16_t endCode;
        std::vector<NTR::Format::CMAPEntry *> *entries;
    };
}
