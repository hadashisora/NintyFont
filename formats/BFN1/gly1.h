// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::DOL::Format
{
    class GLY1
    {
    public:
        //Ctors
        GLY1(BinaryTools::BinaryReader *br);
        //Dtor
        ~GLY1();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic; //Should be 0x474C5931, GLY1 in ASCII
        uint32_t length; //Length of the section/block
        uint16_t startCode; //Code point of first character
        uint16_t endCode; //Code point of last character
        uint16_t cellWidth; //Glyph cell width, in pixels
        uint16_t cellHeight; //Glyph cell height, in pixels
        uint32_t sheetSize; //Sheet size in bytes
        uint16_t sheetFormat; //Sheet image format
        uint16_t cellsPerRow; //Glyph cells per row
        uint16_t cellsPerColumn; //Glyph cells per column
        uint16_t sheetWidth; //Sheet width, in pixels
        uint16_t sheetHeight; //Sheet height, in pixels
        //Two bytes of null padding

        uint32_t sheetOffset; //Not in the data, just here to keep track of it
    };
}
