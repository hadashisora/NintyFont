// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::DOL::Format
{
    class FONTbfn1
    {
    public:
        //Ctors
        FONTbfn1(BinaryTools::BinaryReader *br);
        //Dtor
        ~FONTbfn1();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint64_t magic; //Should be 0x464F4E5462666E31, FONTbfn1 in ASCII
        uint32_t fileSize; //Length of the entire file in bytes
        uint32_t dataBlocks; //Number of other sections/blocks in the file
        //Entire 0x10 bytes of null padding, yay!
    };
}
