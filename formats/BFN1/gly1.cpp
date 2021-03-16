// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "gly1.h"

namespace NintyFont::DOL::Format
{
    GLY1::GLY1(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        startCode = br->readUInt16();
        endCode = br->readUInt16();
        cellWidth = br->readUInt16();
        cellHeight = br->readUInt16();
        sheetSize = br->readUInt32();
        sheetFormat = br->readUInt16();
        cellsPerRow = br->readUInt16();
        cellsPerColumn = br->readUInt16();
        sheetWidth = br->readUInt16();
        sheetHeight = br->readUInt16();
        br->advancePosition(0x2U); //Skip two bytes of padding
        sheetOffset = br->getPosition();
    }

    void GLY1::validateSignature()
    {
        if (magic != 0x474C5931U) throw std::runtime_error("Invalid GLY1 signature!!!");
        else return;
    }

    void GLY1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->incLookupValue("blockCount", 1);
        bw->write(magic);
        linker->addPatchAddr(bw->getPosition(), "glyphLength");
        bw->write(length);
        bw->write(startCode);
        bw->write(endCode);
        bw->write(cellWidth);
        bw->write(cellHeight);
        bw->write(sheetSize);
        bw->write(sheetFormat);
        bw->write(cellsPerRow);
        bw->write(cellsPerColumn);
        bw->write(sheetWidth);
        bw->write(sheetHeight);
        bw->write((uint16_t)0x0000);
        linker->addPatchAddr(bw->getPosition(), "sheetPtr");
        bw->write(sheetOffset);
    }

    GLY1::~GLY1()
    {
        
    }
}
