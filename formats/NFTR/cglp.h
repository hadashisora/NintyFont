// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for binary CGLP data (Charater? GLyPh)
// While not seen used in anything other than NFTR,
// is is mentioned in NW headers for newer formats
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"

namespace NintyFont::NTR::Format
{
    class CGLP
    {
    public:
        //Ctors
        CGLP(uint8_t t_cellWidth, uint8_t t_cellHeight, uint16_t t_cellSize, int8_t t_baselinePos, uint8_t t_maxCharWidth, uint8_t t_bpp, uint8_t t_flags);
        CGLP(BinaryTools::BinaryReader *br, bool isNewVersion);
        //Dtor
        ~CGLP();
        //Methods
        virtual void validateSignature(void);
        virtual void serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker, std::vector<uint8_t *> *imgs);
        //Fields
        uint32_t bitmapPtr; //Here just for keeping track of where the image data is located, not actually present in the file
        uint32_t magic; //Section magic, should always be 0x43474C50, CGLP in ASCII
        uint32_t length; //Length of the section including bitmap data itself in bytes
        uint8_t cellWidth; //Glyph cell width, in pixels
        uint8_t cellHeight; //Glyph cell height, in pixels
        uint16_t cellSize; //IDK even, maybe cell size in bytes?
        int8_t baselinePos; //Baseline position
        uint8_t maxCharWidth; //Maximum character width
        uint8_t bpp; //Bitmap Bits Per Pixel
        uint8_t flags; //Flags, see the enum below. Called "reserved" since RVL SDK
        //Enums
        enum fontGlyphFlags : uint8_t
        {
            verticalWriting = (1 << 0),
            rot0Deg = (0 << 1),
            rot90Deg = (1 << 1),
            rot180Deg = (2 << 1),
            rot270Deg = (3 << 1),
            rotMask = (3 << 1)
        };
    };
}
