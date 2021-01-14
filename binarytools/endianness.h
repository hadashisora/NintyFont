// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for a static class which
// alows the program to determine it's
// endianness (only BE and LE) at runtime
// for correctly handling endian-dependent
// operations on any system
#pragma once

#include <cstdint>

#define ENDIANNESS_LITTLE true
#define ENDIANNESS_BIG false

namespace BinaryTools
{
    class Endianness
    {
    public:
        static bool isSystemLittleEndian(void);
    };
}
