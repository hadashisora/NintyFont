// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the source file for the
// UnicodeNames class, which parses
// DerivedName.txt and looks up Unicode
// character names for character codes
#include "unicodenames.h"

#include <fstream>
#include <sstream>
#include <fmt/core.h>

namespace NintyFont
{
    UnicodeNames::UnicodeNames(std::string derivedNamePath)
    {
        //Initialize the class by parsing DerivedName.txt
        std::ifstream file = std::ifstream(derivedNamePath); //Open file for reading

        std::string line;
        std::string codePointStr;
        std::string codePointEndStr;
        uint32_t codePoint;
        uint32_t codePointEnd;
        std::string charNameStr;

        //Loop through all lines
        while(std::getline(file, line))
        {
            if (line.length() > 0) //Check that the line isn't empty
            {
                if (line.at(0) != '#') //Check if line starts with a hashsign, if so it's a comment and shouldn't be parsed
                {
                    std::istringstream str = std::istringstream(line);
                    if (line.find("..") == std::string::npos) //Check against .. existing on the line. If found we have to process a range of characters instead of just one (that's in the else statement)
                    {
                        std::getline(str, codePointStr, ' '); //Get the code point
                        std::getline(str, charNameStr, ';'); //Dummy read to get rid of semicolon and everything before it
                        str.get(); //Another dummy read to get rid of a whitespace
                        std::getline(str, charNameStr); //Read until the end of line
                        codePoint = std::stoul(codePointStr, 0, 16); //Convert hex string into a uint32_t
                        nameLookUpTable[codePoint] = charNameStr;
                    }
                    else
                    {
                        std::getline(str, codePointStr, '.'); //Get the first number
                        str.get(); //This is a dummy read that will get rid of the second dot after the number
                        std::getline(str, codePointEndStr, ' '); //Get the second number
                        std::getline(str, charNameStr, ';'); //Dummy read to get rid of semicolon and everything before it
                        str.get(); //Another dummy read to get rid of a whitespace
                        std::getline(str, charNameStr, '*'); //Read until the asterisk
                        codePoint = std::stoul(codePointStr, 0, 16); //Convert hex string into a uint32_t
                        codePointEnd = std::stoul(codePointEndStr, 0, 16); //Convert hex string into a uint32_t
                        for (; codePoint <= codePointEnd; codePoint++)
                        {
                            nameLookUpTable[codePoint] = fmt::format("{0}{1:04X}", charNameStr, codePoint);
                        }
                    }
                }
            }
        }
        file.close();
    }

    std::string UnicodeNames::getCharNameFromUnicodeCodepoint(uint32_t codePoint)
    {
        std::string result;
        try
        {
            result = nameLookUpTable.at(codePoint);
        }
        catch (std::out_of_range) //Catches the exception when the code point doesn't exist in the LUT
        {
            result = "";
        }
        return result;
    }

    UnicodeNames::~UnicodeNames()
    {
        
    }
}
