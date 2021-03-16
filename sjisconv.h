// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <string>
#include <map>
#include <cstdint>

namespace NintyFont
{
    class SJISConv
    {
    private:
        std::map<uint16_t, uint16_t> nameLookUpTable;
    public:
        //Ctor
        SJISConv(std::string sjisPath = "SHIFTJIS.TXT");
        //Dtor
        ~SJISConv();
        //Methods
        uint16_t codeToUTF16(uint16_t code);
        uint8_t *codeToUTF8(uint16_t code);
    };
}
