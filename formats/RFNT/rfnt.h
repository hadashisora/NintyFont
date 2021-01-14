// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes the binary
// header structure of RFNT, mostly just
// overriding stuff from NFTR as it contains
// exactly the same data
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "formats/NFTR/nftr.h"

namespace NintyFont::RVL::Format
{
    class RFNT : public NTR::Format::NFTR
    {
    public:
        //Ctors
        RFNT(BinaryTools::BinaryReader *br);
        RFNT(uint16_t t_version = 0x0104U, uint32_t t_magic = 0x52464E54U);
        //Methods
        void validateSignature(void) override;
        void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker) override;
    };
}
