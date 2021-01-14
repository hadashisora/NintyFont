// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for binary NFTR header data
// Newer NW font formats use the same header
// data, just with different fourCC magic
// like RFNT/CFNT/FFNT
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::NTR::Format
{
    class NFTR
    {
    public:
        //Ctors
        NFTR(BinaryTools::BinaryReader *br);
        NFTR(uint16_t t_version = 0x0102U, uint32_t t_magic = 0x4E465452U);
        //Dtors
        virtual ~NFTR();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic; //Leave magic as a writable field because this class will be inherited for future versions of the format
        uint16_t BOM; //uint8_t order mark, FFFE is Little and FEFF is Big Endian
        uint16_t version; //Version
        uint32_t fileSize; //Size of the full file in bytes
        uint16_t ptrInfo; //Pointer to the begining of FINF section
        uint16_t dataBlocks; //Number of data blocks in the file
    };
}
