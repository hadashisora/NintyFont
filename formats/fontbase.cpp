// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "fontbase.h"

namespace NintyFont
{
    void FontBase::saveBinaryFont(std::string filePath)
    {
        throw std::runtime_error("Don't call the base version of saveBinaryFont!");
    }

    std::vector<PropertyList::PropertyListEntryDescriptor *> *FontBase::getGlyphPropertyDescriptors(void)
    {
        throw std::runtime_error("Don't call the base version of getGlyphPropertyDescriptors!");
    }

    std::vector<PropertyList::PropertyBase *> *FontBase::getPropertiesList(void)
    {
        throw std::runtime_error("Don't call the base version of getPropertiesList!");
    }

    std::vector<PropertyList::PropertyBase *> *FontBase::getDrawablePropertiesList(void)
    {
        throw std::runtime_error("Don't call the base version of getDrawablePropertiesList!");
    }

    std::pair<uint32_t, uint32_t> FontBase::getGlyphImageSize(void)
    {
        throw std::runtime_error("Don't call the base version of getGlyphImageSize!");
    }

    bool FontBase::canDeleteGlyphs(void)
    {
        throw std::runtime_error("Don't call the base version of canDeleteGlyphs!");
    }

    bool FontBase::canCreateCopyGlyphs(void)
    {
        throw std::runtime_error("Don't call the base version of canCopyGlyphs!");
    }

    Glyph *FontBase::createEmptyGlyph(void)
    {
        throw std::runtime_error("Don't call the base version of createEmptyGlyph!");
    }

    CharEncodings FontBase::getStdCharEncoding(void)
    {
        throw std::runtime_error("Don't call the base version of getStdCharEncoding!");
    }

    std::string FontBase::getImageFormatString(ImageFormats format)
    {
        std::string str = "";
        switch (format)
        {
            case ImageFormats::None:
                str = "";
                break;
            case ImageFormats::L2:
                str = "L2";
                break;
            case ImageFormats::L2A:
                str = "L2A";
                break;
            case ImageFormats::L4:
                str = "L4";
                break;
            case ImageFormats::L4A:
                str = "L4A";
                break;
            case ImageFormats::L8:
                str = "L8";
                break;
            case ImageFormats::HL8:
                str = "HL8";
                break;
            case ImageFormats::A8:
                str = "A8";
                break;
            case ImageFormats::A4:
                str = "A4";
                break;
            case ImageFormats::L8A:
                str = "L8A";
                break;
            case ImageFormats::RGB565:
                str = "RGB565";
                break;
            case ImageFormats::RGB5A1:
                str = "RGB5A1";
                break;
            case ImageFormats::RGB5A3:
                str = "RGB5A3";
                break;
            case ImageFormats::RGBA4:
                str = "RGBA4";
                break;
            case ImageFormats::RGB8:
                str = "RGB8";
                break;
            case ImageFormats::RGBA8:
                str = "RGBA8";
                break;
            case ImageFormats::ETC1:
                str = "ETC1";
                break;
            case ImageFormats::ETC1A4:
                str = "ETC1A4";
                break;
        }
        return str;
    }

    std::string FontBase::getCharEncodingString(CharEncodings format)
    {
        std::string str = "";
        switch (format)
        {
            case CharEncodings::UTF8:
                str = "UTF-8";
                break;
            case CharEncodings::UTF16:
                str = "UTF-16";
                break;
            case CharEncodings::CP1252:
                str = "CP-1251";
                break;
            case CharEncodings::ShiftJIS:
                str = "Shift-JIS";
                break;
            case CharEncodings::Num:
                str = "";
                break;
        }
        return str;
    }

    void FontBase::drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns)
    {
        throw std::runtime_error("Don't call the base version of drawDrawableProperties!");
    }

    FontBase::~FontBase()
    {
        for (auto glyph = glyphs.begin(); glyph != glyphs.end(); glyph++)
        {
            delete *glyph;
        }
    }
}
