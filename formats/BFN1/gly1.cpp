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
        sheetSize = br->readUInt16();
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

    }

    void GLY1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    GLY1::~GLY1()
    {
        
    }
}
