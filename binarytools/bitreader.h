// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header for bit reader class, which utilizes BinaryReader
// to read file as bits without overcluttering higher-level
// binary handling code
// Ported over from C# from one of my other unfinished projects
#pragma once

#include <math.h>

#include "binaryreader.h"

namespace BinaryTools
{
    class BitReader
    {
    private:
        uint64_t bitPosition;
        uint64_t bytePosition;
        uint8_t buffer;
        BinaryReader *br;
    public:
        //Ctor
        BitReader(BinaryReader *br);
        //Dtor
        ~BitReader();
        //Methods
        bool isByteAligned(void);
        uint8_t readBits(uint8_t count);
        uint8_t readBitsBackwards(uint8_t count);
        uint8_t readBitsNormalized(uint8_t count);
        uint8_t readBitsNormalizedBackwards(uint8_t count);
        uint8_t readBit(void);
        void update(void);
        void updatePosition(uint64_t pos);
    };
}
