// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the header file for UnicodeNames class,
// which parses DerivedName.txt and does the look-up
#pragma once

#include <string>
#include <map>
#include <cstdint>

namespace NintyFont
{
    //This class provides Unicode character names based on the code point
    class UnicodeNames
    {
    private:
        std::map<uint32_t, std::string> nameLookUpTable;
    public:
        //Ctor
        UnicodeNames(std::string derivedNamePath = "DerivedName.txt");
        //Dtor
        ~UnicodeNames();
        //Methods
        std::string getCharNameFromUnicodeCodepoint(uint32_t codePoint);
    };
}
