// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for FINF class
#include <fmt/core.h>

#include "finf.h"

namespace NintyFont::NTR::Format
{
    FINF::FINF(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        fontType = br->readByte();
        lineFeed = br->readByte();
        defaultCharIndex = br->readUInt16();
        defaultWidths = new CharWidths(br);
        encoding = br->readByte();
        ptrGlyph = br->readUInt32();
        ptrWidth = br->readUInt32();
        ptrMap = br->readUInt32();
        if (length == 0x20U)
        {
            height = br->readByte();
            width = br->readByte();
            ascent = br->readByte();
            padding = br->readByte();
        }
        else if (length == 0x1CU)  //Old version has a size of 0x1C, doesn't have the four bytes below
        {
            height = 0xFFU;
            width = 0xFFU;
            ascent = 0xFFU;
            padding = 0xFFU;
        }
        else throw std::runtime_error(fmt::format("Invalid FINF section length 0x{:X}", length));
    }

    FINF::FINF(uint32_t t_length, uint8_t t_fontType, uint8_t t_lineFeed, uint16_t t_defaultCharIndex,
               CharWidths *widths, uint8_t t_encoding, uint8_t t_height, uint8_t t_width,
               uint8_t t_ascent, uint32_t t_magic)
    {
        magic = t_magic;
        length = t_length;
        fontType = t_fontType;
        lineFeed = t_lineFeed;
        defaultCharIndex = t_defaultCharIndex;
        defaultWidths = widths;
        encoding = t_encoding;
        ptrGlyph = 0x0U;
        ptrWidth = 0x0U;
        ptrMap = 0x0U;
        height = t_height;
        width = t_width;
        ascent = t_ascent;
        padding = 0x0U;
    }

    void FINF::validateSignature()
    {
        if (magic != 0x46494E46U && magic != 0x464E4946U) throw std::runtime_error("Invalid FINF signature!!!");
        else return;
    }

    void FINF::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->incLookupValue("ptrInfo", bw->getPosition());
        linker->incLookupValue("blockCount", 1U);
        bw->write(magic);
        bw->write(length);
        bw->write(fontType);
        bw->write(lineFeed);
        bw->write(defaultCharIndex);
        defaultWidths->serialize(bw);
        bw->write(encoding);
        linker->addPatchAddr(bw->getPosition(), "ptrGlyph");
        bw->write(ptrGlyph);
        linker->addPatchAddr(bw->getPosition(), "ptrWidth");
        bw->write(ptrWidth);
        linker->addPatchAddr(bw->getPosition(), "ptrMap");
        bw->write(ptrMap);
        if (length == 0x20U)
        {
            bw->write(height);
            bw->write(width);
            bw->write(ascent);
            bw->write((uint8_t)0x0);
        }
    }

    FINF::~FINF()
    {
        delete defaultWidths;
    }
}
