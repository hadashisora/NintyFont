// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for a binary block linker class,
// which allows us to take note of certain items
// and locations when writing binary data using BinaryWriter
// in order to later return to the needed locations and
// patch the proper values in
// This de-clutters the binary writing code from
// having to calculate offsets, which can be prone
// to spaghetti code and errors
#pragma once

#include <cstdint>
#include <map>

#include "binarywriter.h"

namespace BinaryTools
{
    class BlockLinker
    {
    private:
        //LUTs
        std::map<std::string, uint32_t> valueLookupTable;
        std::map<uint32_t, std::string> patchLookupTable;
        std::map<uint32_t, std::string> shortPatchLookupTable;
    public:
        //Ctor
        BlockLinker();
        //Dtor
        ~BlockLinker();
        //Methods
        void makeBlockLink(BinaryWriter *bw); //aka MakeBlcokLink() XD
        void addLookupValue(std::string lookup, uint32_t value);
        void incLookupValue(std::string lookup, uint32_t inc);
        uint32_t getLookupValue(std::string lookup);
        void addPatchAddr(uint32_t address, std::string lookup);
        void addShortPatchAddr(uint32_t address, std::string lookup);
    };
}
