// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// A small template struct that wraps an array
// pointer along with it's size.
// This has to exist because std::array is of
// fixed size and we only really need to pass
// around an array of variable size and know
// it's size, so std::vector is totally overkill
// for this job.
#pragma once

#include <cstdint>

namespace NintyFont::util
{
    template <typename T> struct array
    {
        T *arr;
        uint32_t size;
    };
}
