// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for BlockLinker class
#include "blocklinker.h"

#include <fmt/core.h>
#include <QDebug>

namespace BinaryTools
{
    BlockLinker::BlockLinker()
    {

    }

    BlockLinker::~BlockLinker()
    {

    }

    void BlockLinker::makeBlockLink(BinaryWriter *bw)
    {
        for (auto patch = patchLookupTable.begin(); patch != patchLookupTable.end(); patch++)
        {
            bw->setPosition((*patch).first);
            qDebug() << QString::fromStdString(fmt::format("Patching uint {} at 0x{:X}", (*patch).second, bw->getPosition()));
            bw->write(getLookupValue((*patch).second));
        }
        for (auto patch = shortPatchLookupTable.begin(); patch != shortPatchLookupTable.end(); patch++)
        {
            bw->setPosition((*patch).first);
            qDebug() << QString::fromStdString(fmt::format("Patching ushort {} at 0x{:X}", (*patch).second, bw->getPosition()));
            auto val = getLookupValue((*patch).second);
            bw->write((uint16_t)val);
        }
    }

    void BlockLinker::addLookupValue(std::string lookup, uint32_t value)
    {
        valueLookupTable[lookup] = value;
    }

    void BlockLinker::incLookupValue(std::string lookup, uint32_t inc)
    {
        try
        {
            valueLookupTable.at(lookup) += inc;
        }
        catch (std::out_of_range)
        {
            valueLookupTable[lookup] = inc;
        }
    }

    uint32_t BlockLinker::getLookupValue(std::string lookup)
    {
        uint32_t result;
        try
        {
            result = valueLookupTable.at(lookup);
        }
        catch (std::out_of_range)
        {
            qDebug() << QString::fromStdString(fmt::format("Couldn't find value for key {} in valueLookup!", lookup));
            result = 0x0U;
        }
        return result;
    }

    void BlockLinker::addPatchAddr(uint32_t address, std::string lookup)
    {
        patchLookupTable[address] = lookup;
    }

    void BlockLinker::addShortPatchAddr(uint32_t address, std::string lookup)
    {
        shortPatchLookupTable[address] = lookup;
    }
}
