// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <cstdint>
#include <vector>

#include "formats/fontbase.h"
#include "propertylist.h"

namespace NintyFont::DOL
{
    //This is named after how this format is named in Nintendo's internal library.
    //class JUtility::JUTResFont : public JUtility::JUTFont
    class JUTResFont : public FontBase
    {
    private:
        std::pair<uint32_t, uint32_t> cellSize; //width, height
        std::pair<uint32_t, uint32_t> sheetSize; //width, height
        //Property list stuff
        std::vector<PropertyList::PropertyBase *> *fontProperties;
    public:
        JUTResFont(std::string filePath);
        static bool identifyFile(uint8_t *bytes);
        static std::string returnFileTypeString(void);
        static std::string returnFileExtensionString(void);
    };
}
