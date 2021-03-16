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
        entriesOffset = br->getPosition();
    }

    void WID1::validateSignature()
    {
        if (magic != 0x57494431U) throw std::runtime_error("Invalid WID1 signature!!!");
        else return;
    }

    void WID1::readEntries(BinaryTools::BinaryReader *br)
    {
        entries = new std::vector<WID1Entry *>(0);
        if (entriesOffset == 0U) return; //The case if the first contructor was chosen
        br->setPosition(entriesOffset);
        std::vector<WID1Entry *> tempWidths;
        //Read all the entries
        for (uint16_t i = startCode; i <= endCode; i++)
            entries->push_back(new WID1Entry(br));
        entries->shrink_to_fit();
    }

    void WID1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    WID1::~WID1()
    {
        
    }
}
