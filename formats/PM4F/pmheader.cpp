// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for PMHeader
#include "pmheader.h"

namespace NintyFont::NTR::PM4::Format
{
    PMHeader::PMHeader(BinaryTools::BinaryReader *br)
    {
        ptrImage = br->readUInt32();
        ptrWidths = br->readUInt32();
        glyphCount = br->readUInt32();
        maxWidth = br->readByte();
        maxHeight = br->readByte();
        letterCharX = br->readByte();
        letterCharY = br->readByte();
    }

    PMHeader::PMHeader(uint32_t t_glyphCount, uint8_t t_maxWidth, uint8_t t_maxHeight)
    {
        ptrImage = 0x0U;
        ptrWidths = 0x0U;
        glyphCount = t_glyphCount;
        maxWidth = t_maxWidth;
        maxHeight = t_maxHeight;
        letterCharX = 0x2U;
        letterCharY = 0x2U;
    }

    void PMHeader::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->addPatchAddr(bw->getPosition(), "ptrImage");
        bw->write(ptrImage);
        linker->addPatchAddr(bw->getPosition(), "ptrWidths");
        bw->write(ptrWidths);
        bw->write(glyphCount);
        bw->write(maxHeight);
        bw->write(maxWidth);
        bw->write(letterCharX);
        bw->write(letterCharY);
    }

    PMHeader::~PMHeader()
    {
        
    }
}
