// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes the binary
// header structure of CFNT, mostly just
// overriding stuff from NFTR as it contains
// exactly the same data
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "formats/NFTR/nftr.h"

namespace NintyFont::CTR::Format
{
    class CFNT
    {
    public:
        //Ctors
        CFNT(BinaryTools::BinaryReader *br);
        CFNT(uint32_t t_version = 0x3000000U, uint32_t t_magic = 0x544E4643U);
        //Dtor
        virtual ~CFNT();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic; //Leave magic as a writable field because this class will be inherited for future versions of the format
        uint16_t BOM; //uint8_t order mark, FFFE is Little and FEFF is Big Endian
        uint16_t headerSize; //Size of the CFNT header
        uint32_t version; //Version
        uint32_t fileSize; //Size of the full file in bytes
        uint16_t dataBlocks; //Number of data blocks in the file
        uint16_t reserved; //Aka two bytes of null padding
    };
}
