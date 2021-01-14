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
    class INF1
    {
    public:
        //Ctors
        INF1(BinaryTools::BinaryReader *br);
        //Dtor
        ~INF1();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic; //0x0, Should be 0x494E4631, INF1 in ASCII
        uint32_t length; //0x4, Length of the section/block, should be always 0x20
        uint16_t fontType; //0x8, Character encoding(the reason this is called fontType is because that's how it's called officially), probably the same as in NFTR
        uint16_t ascent; //0xA, Ascent
        uint16_t descent; //0xC, Descent
        uint16_t width; //0xE, Width
        uint16_t leading; //0x10, Leading
        uint16_t defaultCharCode; //0x12, Index of default character
    };
}
