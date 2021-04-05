// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for TGLP class
#include "tglp.h"

namespace NintyFont::RVL::Format
{
    TGLP::TGLP(uint8_t t_cellWidth, uint8_t t_cellHeight, uint8_t t_baselinePos,
         uint8_t t_maxCharWidth, uint32_t t_sheetSize, uint16_t t_sheetCount, uint16_t t_sheetFormat,
         uint16_t t_cellsPerRow, uint16_t t_cellsPerColumn, uint16_t t_sheetWidth, uint16_t t_sheetHeight, uint32_t t_magic, uint32_t t_length,
         uint32_t t_sheetPtr)
    {
        magic = t_magic;
        length = t_length;
        cellWidth = t_cellWidth;
        cellHeight = t_cellHeight;
        baselinePos = t_baselinePos;
        maxCharWidth = t_maxCharWidth;
        sheetSize = t_sheetSize;
        sheetCount = t_sheetCount;
        sheetFormat = t_sheetFormat;
        cellsPerRow = t_cellsPerRow;
        cellsPerColumn = t_cellsPerColumn;
        sheetWidth = t_sheetWidth;
        sheetHeight = t_sheetHeight;
        sheetPtr = t_sheetPtr;
    }

    TGLP::TGLP(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        cellWidth = br->readByte();
        cellHeight = br->readByte();
        baselinePos = br->readByte();
        maxCharWidth = br->readByte();
        sheetSize = br->readUInt32();
        sheetCount = br->readUInt16();
        sheetFormat = br->readUInt16();
        cellsPerRow = br->readUInt16();
        cellsPerColumn = br->readUInt16();
        sheetWidth = br->readUInt16();
        sheetHeight = br->readUInt16();
        sheetPtr = br->readUInt32();
    }

    void TGLP::validateSignature()
    {
        if (magic != 0x54474C50U && magic != 0x504C4754U) throw std::runtime_error("Invalid TGLP signature!!!");
        else return;
    }

    void TGLP::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker, std::vector<util::array<uint8_t>> *sheets, uint32_t align)
    {
        linker->incLookupValue("blockCount", 1);
        bw->write(magic);
        linker->addPatchAddr(bw->getPosition(), "glyphLength");
        bw->write(length);
        uint32_t ptr = bw->getPosition();
        linker->addLookupValue("ptrGlyph", ptr);
        bw->write(cellWidth);
        bw->write(cellHeight);
        bw->write(baselinePos);
        bw->write(maxCharWidth);
        bw->write(sheetSize);
        bw->write(sheetCount);
        bw->write(sheetFormat);
        bw->write(cellsPerRow);
        bw->write(cellsPerColumn);
        bw->write(sheetWidth);
        bw->write(sheetHeight);
        linker->addPatchAddr(bw->getPosition(), "sheetPtr");
        bw->write(sheetPtr);
        //pad to the next 0x10 byte boundary
        uint32_t padBytes = align - (bw->getPosition() % align);
        if (padBytes != align) for (uint32_t i = 0x0U; i < padBytes; i++) bw->write((uint8_t)0x0U);
        //Image data
        linker->addLookupValue("sheetPtr", bw->getPosition());
        for (auto sheet = sheets->begin(); sheet != sheets->end(); sheet++)
        {
            bw->write((*sheet).arr, 0, (*sheet).size);
            // for (auto val = (*sheet)->begin(); val != (*sheet)->end(); val++)
            // {
            //     bw->write(*val);
            // }
        }
        //Padding
        padBytes = 0x4U - (bw->getPosition() % 0x4U); //Yes I know this setup for calculating padding is a bodge...deal with it
        if (padBytes != 0x4U) for (uint8_t i = 0x0U; i < padBytes; i++) bw->write((uint8_t)0x0U);
        linker->addLookupValue("glyphLength", bw->getPosition() - ptr + 0x8U);
    }

    TGLP::~TGLP()
    {
        
    }
}
