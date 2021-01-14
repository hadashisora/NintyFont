// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for CMAP binary data
// Reused for all formats since NFTR
// and somewhat based on MAP1 of BFN1
#pragma once

#include <cstdint>
#include <list>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "glyph.h"
#include "cmapentry.h"

namespace NintyFont::NTR::Format
{
    class CMAP
    {
    private:
        uint32_t entriesOffset; //Used to keep track of where to read the entries from, not in the actual data
    public:
        //Ctors
        CMAP(std::pair<Glyph *, Glyph *> maps);
        CMAP(uint16_t t_mappingMethod, std::vector<CMAPEntry *> *maps);
        CMAP(BinaryTools::BinaryReader *br);
        //Dctor
        ~CMAP();
        //Methods
        virtual void validateSignature(void);
        virtual void readEntries(BinaryTools::BinaryReader *br);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //fontcvtr methods
        static std::vector<std::pair<Glyph *, Glyph *>> *createDirectEntries(std::list<Glyph *> *glyphs);
        static std::vector<std::vector<CMAPEntry *> *> *createTableEntries(std::list<Glyph *> *glyphs);
        static std::vector<std::vector<CMAPEntry *> *> *createScanEntries(std::list<Glyph *> *glyphs);
        //Fields
        uint32_t magic; //Should always be 0x434D4150, CMAP in ASCII
        uint32_t length; //CMAP section length in bytes
        uint16_t codeBegin; //First character code
        uint16_t codeEnd; //Last character code
        uint16_t mappingMethod; //Mapping method, 0x0 - Direct, 0x1 - Table, 0x2 - Scan
        uint16_t reserved; //Reserved (this is referred to it as so even in the official SDK software)
        uint32_t ptrNext; //Pointer to the next CMAP(next CMAP offset + 0x8), 0x0 if last
        std::vector<CMAPEntry *> *entries;
    };
}
