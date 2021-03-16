// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "inf1.h"

namespace NintyFont::DOL::Format
{
    INF1::INF1(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        fontType = br->readUInt16();
        ascent = br->readUInt16();
        descent = br->readUInt16();
        width = br->readUInt16();
        leading = br->readUInt16();
        defaultCharCode = br->readUInt16();
        br->advancePosition(0xC); //Skip the padding
    }

    void INF1::validateSignature()
    {
        if (magic != 0x494E4631U) throw std::runtime_error("Invalid INF1 signature!!!");
        else return;
    }

    void INF1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    INF1::~INF1()
    {
        
    }
}
