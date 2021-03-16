// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "fontbfn1.h"

namespace NintyFont::DOL::Format
{
    FONTbfn1::FONTbfn1(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt64();
        fileSize = br->readUInt32();
        dataBlocks = br->readUInt32();
        br->advancePosition(0x10U); //Skip the 0x10 bytes of null padding
    }

    void FONTbfn1::validateSignature()
    {
        if (magic != 0x464F4E5462666E31U) throw std::runtime_error("Invalid FONTbfn1 signature!!!");
        else return;
    }

    void FONTbfn1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->addLookupValue("ptrFont", bw->getPosition());
        //linker->incLookupValue("blockCount", 1U);
        bw->write(magic);
        linker->addPatchAddr(bw->getPosition(), "fileSize");
        bw->write(fileSize);
        linker->addPatchAddr(bw->getPosition(), "blockCount");
        bw->write(dataBlocks);
    }

    FONTbfn1::~FONTbfn1()
    {
        
    }
}
