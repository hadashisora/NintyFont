// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "cfnt.h"

namespace NintyFont::CTR::Format
{
    CFNT::CFNT(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        BOM = br->readUInt16();
        headerSize = br->readUInt16();
        version = br->readUInt32();
        fileSize = br->readUInt32();
        dataBlocks = br->readUInt16();
        reserved = br->readUInt16();
    }

    CFNT::CFNT(uint32_t t_version, uint32_t t_magic)
    {
        magic = t_magic;
        BOM = 0xFEFFU;
        headerSize = 0x0;
        version = t_version;
        fileSize = 0x0U;
        dataBlocks = 0x0U;
        reserved = 0x0U;
    }

    void CFNT::validateSignature(void)
    {
        if (magic != 0x43464E54U && magic != 0x544E4643U) throw std::runtime_error("Invalid CFNT signature!!!");
        else return;
    }

    void CFNT::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->addLookupValue("ptrFont", bw->getPosition());
        bw->write(magic);
        bw->write(BOM);
        linker->addShortPatchAddr(bw->getPosition(), "ptrInfo");
        bw->write(headerSize);
        bw->write(version);
        linker->addPatchAddr(bw->getPosition(), "fileSize");
        bw->write(fileSize);
        linker->addShortPatchAddr(bw->getPosition(), "blockCount");
        bw->write(dataBlocks);
        bw->write(reserved);
    }

    CFNT::~CFNT()
    {

    }
}
