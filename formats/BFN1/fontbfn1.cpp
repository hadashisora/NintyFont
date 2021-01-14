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
        length = br->readUInt32();
        dataBlocks = br->readUInt32();
        br->advancePosition(0x10U); //Skip the 0x10 bytes of null padding
    }

    void FONTbfn1::validateSignature()
    {

    }

    void FONTbfn1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    FONTbfn1::~FONTbfn1()
    {
        
    }
}
