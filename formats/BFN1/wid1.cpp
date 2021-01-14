// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "wid1.h"

namespace NintyFont::DOL::Format
{
    WID1::WID1(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        startCode = br->readUInt16();
        endCode = br->readUInt16();
    }

    void WID1::validateSignature()
    {

    }

    void WID1::readEntries(BinaryTools::BinaryReader *br)
    {
        entries = new std::vector<NTR::Format::CharWidths *>(0);
//        if (entriesOffset == 0U) return; //The case if the first contructor was chosen
//        br->setPosition(entriesOffset);
//        std::vector<NTR::Format::CharWidths *> tempWidths;
//        //Read all the entries
//        for (uint16_t i = indexBegin; i <= indexEnd; i++)
//            entries->push_back(new NTR::Format::CharWidths(br));
        entries->shrink_to_fit();
    }

    void WID1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    WID1::~WID1()
    {
        
    }
}
