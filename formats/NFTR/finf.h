// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for FINF (Font INFo) binary
// data class
// Just as with NFTR header, this has
// been reused by all newer NW formats
// to date without any changes (not
// even fourCC magic)
// Somewhat based on INF1 structure
// from BFN1/JUTResFont
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "CWDH/charwidths.h"

namespace NintyFont::NTR::Format
{
    class FINF
    {
    public:
        //Ctors
        FINF(uint32_t length, uint8_t fontType, uint8_t lineFeed, uint16_t defaultCharIndex, CharWidths *widths,
             uint8_t encoding, uint8_t height = 0xFFU, uint8_t width = 0xFFU, uint8_t ascent = 0xFFU, uint32_t magic = 0x46494E46U);
        FINF(BinaryTools::BinaryReader *br);
        //Dtors
        virtual ~FINF();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker);
        //Fields
        uint32_t magic; //Should always be 0x46494E46, FINF in ASCII
        uint32_t length; //FINF section length in bytes
        uint8_t fontType; //Font type, 0x0 - Bitmap, 0x1 - TGLP
        uint8_t lineFeed; //Line feed, == leading
        uint16_t defaultCharIndex; //Index of default character, used for when a program requests a character that doesn't exist in the font
        CharWidths *defaultWidths; //Default character widths, ? fallback for characters that don't have width information
        uint8_t encoding; //Font encoding, 0x0 - UTF-8, 0x1 - UTF-16, 0x2 - ShiftJIS, 0x3 - CP1252
        uint32_t ptrGlyph; //TGLP section data pointer (TGLP + 0x8)
        uint32_t ptrWidth; //CWDH section data pionter (CWDH + 0x8)
        uint32_t ptrMap; //CMAP section data pointer (CMAP + 0x8)
        uint8_t height; //Font height, v1.2+ and all later revisions only
        uint8_t width; //Font width, v1.2+ and all later revisions only
        uint8_t ascent; //Ascent, v1.2+ and all later revisions only
        uint8_t padding; //Padding byte, v1.2+ and all later revisions only
    };
}
