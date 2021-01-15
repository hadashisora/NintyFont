// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header files describes TextureCodec class
// for decoding/encoding textures from various
// platforms
// Yes, I know the way I have the same class
// declared in different namespaces differently
// is a bodge, deal with it
#pragma once

#include <cstdint>

#include "binarytools/binaryreader.h"
#include "binarytools/bitreader.h"
#include "formats/fontbase.h"

namespace NintyFont::NTR::Image
{
    class TextureCodec
    {
    public:
        static uint8_t *decodeBitmap(uint8_t bpp, BinaryTools::BitReader *br, uint16_t width, uint16_t height);
        static void encodeBitmap(uint8_t *argb, uint8_t *dst, uint8_t bpp, uint16_t width, uint16_t height);
    };
}

namespace NintyFont::RVL::Image
{
    class TextureCodec
    {
    public:
        static uint8_t *decodeTexture(uint8_t texFmt, BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeTexture(uint8_t texFmt, uint8_t *data, uint16_t width, uint16_t height);
        static uint8_t returnGeneralTextureType(uint8_t texFmt);
        static uint8_t returnPlatformTextureType(uint8_t generalFmt);
        static uint8_t *decodeI4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeI8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeIA4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeIA8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGB565(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGB5A3(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGBA8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeI4(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeI8(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeIA4(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeIA8(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGB565(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGB5A3(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGBA8(uint8_t *argbBuf, uint16_t width, uint16_t height);
        enum class DolRvlTexFmts : uint8_t
        {
            I4 = 0x0,
            I8 = 0x1,
            IA4 = 0x2,
            IA8 = 0x3,
            RGB565 = 0x4,
            RGB5A3 = 0x5,
            RGBA8 = 0x6,
            C4 = 0x8,
            C8 = 0x9,
            C14X2 = 0xA,
            CMPR = 0xE,
        };
    };
}

namespace NintyFont::CTR::Image
{
    class TextureCodec
    {
    public:
        static uint8_t *decodeTexture(uint8_t texFmt, BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeTexture(uint8_t texFmt, uint8_t *data, uint16_t width, uint16_t height);
        static uint8_t returnGeneralTextureType(uint8_t texFmt);
        static uint8_t returnPlatformTextureType(uint8_t generalFmt);
        static uint8_t *decodeRGBA8888(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGB888(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGBA5551(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGB565(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeRGBA4444(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeLA88(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        //static uint8_t *decodeHL8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        //static uint8_t *decodeL8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeA8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeLA44(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        //static uint8_t *decodeL4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static uint8_t *decodeA4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        //static uint8_t *decodeETC1(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        //static uint8_t *decodeETC1A4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGBA8888(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGB888(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGBA5551(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGB565(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeRGBA4444(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeLA88(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeA8(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeLA44(uint8_t *argbBuf, uint16_t width, uint16_t height);
        static std::vector<uint8_t> *encodeA4(uint8_t *argbBuf, uint16_t width, uint16_t height);
        enum class CtrTexFormat : uint8_t
        {
            RGBA8888 = 0,
            RGB888 = 1,
            RGBA5551 = 2,
            RGB565 = 3,
            RGBA4444 = 4,
            LA88 = 5,
            HL8 = 6,
            L8 = 7,
            A8 = 8,
            LA44 = 9,
            L4 = 10,
            A4 = 11,
            ETC1 = 12,
            ETC1A4 = 13
        };
    };
}
