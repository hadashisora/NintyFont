// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <cstdint>

#include "formats/fontbase.h"
#include "propertylist.h"

namespace NintyFont::CTR
{
    //This is literally copy-pasted from binaryrevolutionfont.h
    class BinaryCitrusFont : public FontBase
    {
    private:
        std::pair<uint32_t, uint32_t> cellSize; //width, height
        std::pair<uint32_t, uint32_t> sheetSize; //width, height
        //Private methods
        void recalculateSheetInfo(int32_t pixels = 0U);
        void drawLinefeed(QPainter *painter);
        void drawAscent(QPainter *painter);
        void drawBaseline(QPainter *painter);
        void drawWidths(QPainter *painter);
        //Property list stuff
        std::vector<PropertyList::PropertyBase *> *fontProperties;
    public:
        //Contructor
        BinaryCitrusFont(std::string filePath);
        //Destructor
        ~BinaryCitrusFont();
        //Methods
        void saveBinaryFont(std::string filePath) override;
        std::vector<PropertyList::PropertyListEntryDescriptor *> *getGlyphPropertyDescriptors(void) override;
        std::vector<PropertyList::PropertyBase *> *getPropertiesList(void) override;
        std::pair<uint32_t, uint32_t> getGlyphImageSize(void) override;
        bool canCreateCopyGlyphs(void) override;
        bool canDeleteGlyphs(void) override;
        Glyph *createEmptyGlyph(void) override;
        std::vector<PropertyList::PropertyBase *> * getDrawablePropertiesList(void) override;
        void drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns) override;
        static bool identifyFile(uint8_t *bytes);
        static std::string returnFileTypeString(void);
        static std::string returnFileExtensionString(void);
    };
}
