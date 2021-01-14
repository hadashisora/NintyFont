// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for NFTR class
#include "nftr.h"

namespace NintyFont::NTR::Format
{
    NFTR::NFTR(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        BOM = br->readUInt16();
        version = br->readUInt16();
        fileSize = br->readUInt32();
        ptrInfo = br->readUInt16();
        dataBlocks = br->readUInt16();
    }

    NFTR::NFTR(uint16_t t_version, uint32_t t_magic)
    {
        magic = t_magic;
        BOM = 0xFEFFU;
        version = t_version;
        fileSize = 0x0U;
        ptrInfo = 0x0U;
        dataBlocks = 0x0U;
    }

    void NFTR::validateSignature()
    {
        if (magic != 0x4E465452U && magic != 0x5254464EU) throw std::runtime_error("Invalid NFTR signature!!!");
        else return;
    }

    void NFTR::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->addLookupValue("ptrFont", bw->getPosition());
        //linker->incLookupValue("blockCount", 1U);
        bw->write(magic);
        bw->write(BOM);
        bw->write(version);
        linker->addPatchAddr(bw->getPosition(), "fileSize");
        bw->write(fileSize);
        linker->addShortPatchAddr(bw->getPosition(), "ptrInfo");
        bw->write(ptrInfo);
        linker->addShortPatchAddr(bw->getPosition(), "blockCount");
        bw->write(dataBlocks);
    }

    NFTR::~NFTR()
    {

    }
}
