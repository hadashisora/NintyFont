// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include <fstream>
#include <sstream>
#include <fmt/core.h>

#include "sjisconv.h"

namespace NintyFont
{
    SJISConv::SJISConv(std::string sjisPath)
    {
        //Initialize the class by parsing DerivedName.txt
        std::ifstream file = std::ifstream(sjisPath); //Open file for reading

        std::string line;
        std::string sjisStr;
        std::string unicodeStr;
        uint16_t sjisCode;
        uint16_t unicodeCode;

        //Loop through all lines
        while(std::getline(file, line))
        {
            if (line.length() > 0) //Check that the line isn't empty
            {
                if (line.at(0) != '#') //Check if line starts with a hashsign, if so it's a comment and shouldn't be parsed
                {
                    std::istringstream str = std::istringstream(line);
                    std::getline(str, sjisStr, '\t'); //Get the SJIS code
                    std::getline(str, unicodeStr, '\t'); //Get the Unicode code
                    sjisCode = std::stoul(sjisStr, 0, 16); //Convert hex string into a uint16_t
                    unicodeCode = std::stoul(unicodeStr, 0, 16); //Convert hex string into a uint16_t
                    nameLookUpTable[sjisCode] = unicodeCode;
                }
            }
        }
        file.close();
    }

    uint16_t SJISConv::codeToUTF16(uint16_t code)
    {
        uint16_t result;
        try
        {
            result = nameLookUpTable.at(code);
        }
        catch (std::out_of_range) //Catches the exception when the code point doesn't exist in the LUT
        {
            throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Invalid ShiftJIS code 0x{:04X}!", __FILE__, __LINE__, code));
        }
        return result;
    }

    SJISConv::~SJISConv()
    {
        
    }
}
