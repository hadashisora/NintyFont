// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for static Endianness class
// Solution taken from StackExchange, but I
// can't seem to find/remember the source
#include <stdexcept>

#include "endianness.h"

namespace BinaryTools
{
    static const uint32_t testLE = 0x03020100;
    static const uint32_t testBE = 0x00010203;
    static const uint8_t test[4] = {0, 1, 2, 3};

    bool Endianness::isSystemLittleEndian()
    {
        if (testLE == *(uint32_t *)test) return ENDIANNESS_LITTLE;
        else if (testBE == *(uint32_t *)test) return ENDIANNESS_BIG;
        else throw std::runtime_error("Unknown CPU endianness!");
    }
}
