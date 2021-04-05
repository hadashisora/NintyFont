// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes structure
// of TGLP (Texture GLyPh) block binary data
// as well as functions to process it
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "sizedarray.h"

namespace NintyFont::RVL::Format
{
    class TGLP
    {
    public:
        //Ctors
        TGLP(uint8_t cellWidth, uint8_t cellHeight, uint8_t baselinePos,
             uint8_t maxCharWidth, uint32_t sheetSize, uint16_t sheetCount, uint16_t sheetFormat,
             uint16_t cellsPerRow, uint16_t cellsPerColumn, uint16_t sheetWidth, uint16_t sheetHeight, uint32_t magic = 0x54474C50U, uint32_t length = 0x0U,
             uint32_t sheetPtr = 0x0U);
        TGLP(BinaryTools::BinaryReader *br);
        //Dtor
        virtual ~TGLP();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker, std::vector<util::array<uint8_t>> *sheets, uint32_t align = 0x10);
        //Fields
        uint32_t magic; //Should always be 0x54474C50, TGLP in ASCII
        uint32_t length; //TGLP section length in bytes
        uint8_t cellWidth; //Glyph cell width, in pixels
        uint8_t cellHeight; //Glyph cell height, in pixels
        uint8_t baselinePos; //Baseline position
        uint8_t maxCharWidth; //Maximum character width
        uint32_t sheetSize; //Single sheet size in bytes
        uint16_t sheetCount; //Sheet count
        uint16_t sheetFormat; //Sheet image format
        uint16_t cellsPerRow; //Glyph cells per row
        uint16_t cellsPerColumn; //Glyph cells per column
        uint16_t sheetWidth; //Sheet width, in pixels
        uint16_t sheetHeight; //Sheet height, in pixels
        uint32_t sheetPtr; //Sheet data pointer
    };
}
