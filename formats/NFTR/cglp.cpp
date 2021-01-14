// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for CGLP class
#include "cglp.h"

namespace NintyFont::NTR::Format
{
    CGLP::CGLP(uint8_t t_cellWidth, uint8_t t_cellHeight, uint16_t t_cellSize, int8_t t_baselinePos, uint8_t t_maxCharWidth, uint8_t t_bpp, uint8_t t_flags)
    {
        magic = 0x43474C50U;
        cellWidth = t_cellWidth;
        cellHeight = t_cellHeight;
        cellSize = t_cellSize;
        baselinePos = t_baselinePos;
        maxCharWidth = t_maxCharWidth;
        bpp = t_bpp;
        flags = t_flags;
    }

    CGLP::CGLP(BinaryTools::BinaryReader *br, bool isNewVersion)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        cellWidth = br->readByte();
        cellHeight = br->readByte();
        cellSize = br->readUInt16();
        baselinePos = br->readSByte();
        maxCharWidth = br->readByte();
        bpp = br->readByte();
        flags = br->readByte();

        bitmapPtr = br->getPosition();
    }

    void CGLP::validateSignature()
    {
        if (magic != 0x43474C50U && magic != 0x504C4743U) throw std::runtime_error("Invalid CGLP signature!!!");
        else return;
    }

    void CGLP::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker, std::vector<uint8_t *> *imgs)
    {
        linker->incLookupValue("blockCount", 1);
        bw->write(magic);
        linker->addPatchAddr(bw->getPosition(), "glyphLength");
        bw->write(length);
        uint32_t ptr = bw->getPosition();
        linker->addLookupValue("ptrGlyph", ptr);
        bw->write(cellWidth);
        bw->write(cellHeight);
        bw->write(cellSize);
        bw->write(baselinePos);
        bw->write(maxCharWidth);
        bw->write(bpp);
        bw->write(flags);
        //Image data
        for (auto img = imgs->begin(); img != imgs->end(); img++)
        {
            for (uint32_t i = 0; i < cellSize; i++)
            {
                bw->write((*img)[i]);
            }
        }
        //Padding
        uint8_t padBytes = 0x4U - (bw->getPosition() % 0x4U); //Yes I know this setup for calculating padding is a bodge...deal with it
        if (padBytes != 0x4U) for (uint8_t i = 0x0U; i < padBytes; i++) bw->write((uint8_t)0x0U);
        linker->addLookupValue("glyphLength", bw->getPosition() - ptr + 0x8U);
    }

    CGLP::~CGLP()
    {
        
    }
}
