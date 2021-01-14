// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for CharWidths class
#include "charwidths.h"

namespace NintyFont::NTR::Format
{
    CharWidths::CharWidths(int8_t t_left, uint8_t t_glyphWidth, uint8_t t_charWidth)
    {
        left = t_left;
        glyphWidth = t_glyphWidth;
        charWidth = t_charWidth;
    }

    CharWidths::CharWidths(BinaryTools::BinaryReader *br)
    {
        left = br->readSByte();
        glyphWidth = br->readByte();
        charWidth = br->readByte();
    }

    CharWidths *CharWidths::copy(void)
    {
        return new CharWidths(left, glyphWidth, charWidth);
    }

    std::vector<CharWidths *> *CharWidths::createWidthEntries(std::list<Glyph *> *glyphs)
    {
        std::vector<CharWidths *> *entries = new std::vector<CharWidths *>();
        entries->reserve(glyphs->size());
        for (auto glyph = glyphs->begin(); glyph != glyphs->end(); glyph++)
        {
            int8_t left = ((PropertyList::Property<int8_t> *)(((*glyph)->props->at(2))))->value; //Left
            uint8_t glyphWidth = ((PropertyList::Property<uint8_t> *)(((*glyph)->props->at(3))))->value; //Glyph width
            uint8_t charWidth = ((PropertyList::Property<uint8_t> *)(((*glyph)->props->at(4))))->value; //Char width
            entries->push_back(new CharWidths(left, glyphWidth, charWidth));
        }
        return entries;
    }

    void CharWidths::serialize(BinaryTools::BinaryWriter *bw)
    {
        bw->write(left);
        bw->write(glyphWidth);
        bw->write(charWidth);
    }

    CharWidths::~CharWidths()
    {
        
    }
}
