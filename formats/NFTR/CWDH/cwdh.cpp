// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for CWDH class
#include <fmt/core.h>

#include "cwdh.h"

namespace NintyFont::NTR::Format
{
    CWDH::CWDH(std::vector<CharWidths *> *t_entries)
    {
        magic = 0x43574448U;
        length = 0x0U;
        indexBegin = 0x0U;
        indexEnd = t_entries->size() - 1;
        ptrNext = 0x0U;
        entries = t_entries;
    }

    CWDH::CWDH(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        indexBegin = br->readUInt16();
        indexEnd = br->readUInt16();
        ptrNext = br->readUInt32();
        entriesOffset = (uint32_t)br->getPosition();
    }

    CWDH::~CWDH()
    {
        for (auto entry = entries->begin(); entry != entries->end(); entry++)
        {
            delete *entry;
            *entry = nullptr;
        }
        delete entries;
    }

    void CWDH::validateSignature()
    {
        if (magic != 0x43574448U && magic != 0x48445743U) throw std::runtime_error("Invalid CWDH signature!!!");
        else return;
    }

    void CWDH::readEntries(BinaryTools::BinaryReader *br)
    {
        entries = new std::vector<CharWidths *>(0);
        if (entriesOffset == 0U) return; //The case if the first contructor was chosen
        br->setPosition(entriesOffset);
        std::vector<CharWidths *> tempWidths;
        //Read all the entries
        for (uint16_t i = indexBegin; i <= indexEnd; i++)
            entries->push_back(new CharWidths(br));
        entries->shrink_to_fit();
    }

    void CWDH::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->incLookupValue("blockCount", 1U);
        linker->incLookupValue("CWDH", 1);
        uint32_t startpos = bw->getPosition();
        uint32_t sectionnum = linker->getLookupValue("CWDH");
        linker->addLookupValue(fmt::format("CWDH{}", sectionnum), startpos);
        bw->write(magic);
        linker->addPatchAddr(bw->getPosition(), fmt::format("CWDHLen{}", sectionnum));
        bw->write(length);
        if (sectionnum == 1) linker->addLookupValue("ptrWidth", bw->getPosition());
        bw->write(indexBegin);
        bw->write(indexEnd);
        linker->addPatchAddr(bw->getPosition(), fmt::format("CWDH{}", sectionnum + 1));
        bw->write(ptrNext);
        for (auto entry = entries->begin(); entry != entries->end(); entry++)
        {
            (*entry)->serialize(bw);
        }
        //Padding
        uint8_t padBytes = 0x4U - (bw->getPosition() % 0x4U); //Yes I know this setup for calculating padding is a bodge...deal with it
        if (padBytes != 0x4U) for (uint8_t i = 0x0U; i < padBytes; i++) bw->write((uint8_t)0x0U);
        linker->addLookupValue(fmt::format("CWDHLen{}", sectionnum), bw->getPosition() - startpos);
    }
}
