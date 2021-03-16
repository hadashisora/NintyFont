// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the main header file for Pocket
// Monsters generation 4 games font files
// (D/P/Pt/HG/SS), name genericly because
// this format doesn't have fourCC magic
// unlike Nintendo's fonts
#pragma once

#include <cstdint>

#include "formats/fontbase.h"

namespace NintyFont::NTR::PM4
{
    class PocketMonstersGen4Font : public FontBase
    {
    public:
        //Contructor
        PocketMonstersGen4Font(std::string filePath);
        //Destructor
        ~PocketMonstersGen4Font();
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
        CharEncodings getStdCharEncoding(void) override;
        static bool identifyFile(uint8_t *bytes);
        static std::string returnFileTypeString(void);
        static std::string returnFileExtensionString(void);
    };
}
