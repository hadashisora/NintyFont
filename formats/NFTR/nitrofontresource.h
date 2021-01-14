// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the main header file for NFTR
// (Nitro FonT Resource, sometimes referred
// to as RTFN thanks to LE fourCC, Nitro being
// the codename of the DS)
#pragma once

#include <cstdint>

#include "formats/fontbase.h"

namespace NintyFont::NTR
{
    class NitroFontResource : public FontBase
    {
    private:
        std::pair<uint32_t, uint32_t> cellSize; //width, height
        //Property list stuff
        std::vector<PropertyList::PropertyBase *> *fontProperties;
    public:
        //Constructor
        NitroFontResource(std::string filePath);
        //Destructor
        ~NitroFontResource();
        //Methods
        void saveBinaryFont(std::string filePath) override;
        std::vector<PropertyList::PropertyListEntryDescriptor *> *getGlyphPropertyDescriptors(void) override;
        std::vector<PropertyList::PropertyBase *> *getPropertiesList(void) override;
        std::pair<uint32_t, uint32_t> getGlyphImageSize(void) override;
        bool canCreateCopyGlyphs(void) override;
        bool canDeleteGlyphs(void) override;
        Glyph *createEmptyGlyph(void) override;
        std::vector<PropertyList::PropertyBase *> *getDrawablePropertiesList(void) override;
        void drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns) override;
        static bool identifyFile(uint8_t *bytes);
        static std::string returnFileTypeString(void);
        static std::string returnFileExtensionString(void);
    };
}
