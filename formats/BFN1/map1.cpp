// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "map1.h"

namespace NintyFont::DOL::Format
{
    MAP1::MAP1(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        fontType = br->readUInt16();
        startCode = br->readUInt16();
        endCode = br->readUInt16();
    }

    void MAP1::validateSignature()
    {

    }

    void MAP1::readEntries(BinaryTools::BinaryReader *br)
    {

    }

    void MAP1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    MAP1::~MAP1()
    {
        
    }
}
