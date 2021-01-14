// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// 3DS texture codec source file based
// on Ohana3DS-Rebirth's TextureCodec.cs by gdkchan and kwsch
#include "texturecodec.h"

namespace NintyFont::CTR::Image
{
    uint8_t* TextureCodec::decodeTexture(uint8_t texFmt, BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        switch (texFmt)
        {
            case (uint8_t)CtrTexFormat::RGBA8888:
                return decodeRGBA8888(br, width, height);
            case (uint8_t)CtrTexFormat::RGB888:
                return decodeRGB888(br, width, height);
            case (uint8_t)CtrTexFormat::RGBA5551:
                return decodeRGBA5551(br, width, height);
            case (uint8_t)CtrTexFormat::RGB565:
                return decodeRGB565(br, width, height);
            case (uint8_t)CtrTexFormat::RGBA4444:
                return decodeRGBA4444(br, width, height);
            case (uint8_t)CtrTexFormat::LA88:
                return decodeLA88(br, width, height);
            case (uint8_t)CtrTexFormat::HL8:
                return decodeHL8(br, width, height);
            case (uint8_t)CtrTexFormat::L8:
                return decodeL8(br, width, height);
            case (uint8_t)CtrTexFormat::A8:
                return decodeA8(br, width, height);
            case (uint8_t)CtrTexFormat::LA44:
                return decodeLA44(br, width, height);
            case (uint8_t)CtrTexFormat::L4:
                return decodeL4(br, width, height);
            case (uint8_t)CtrTexFormat::A4:
                return decodeA4(br, width, height);
            case (uint8_t)CtrTexFormat::ETC1:
                return decodeETC1(br, width, height);
            case (uint8_t)CtrTexFormat::ETC1A4:
                return decodeETC1A4(br, width, height);
            default:
                throw std::runtime_error("Unknown CTR texture format!");
                //return nullptr;
        }
    }

    std::vector<uint8_t> *TextureCodec::encodeTexture(uint8_t texFmt, uint8_t *data, uint16_t width, uint16_t height)
    {
        return nullptr;
        // switch (texFmt)
        // {
        //     case (uint8_t)CtrTexFormat::RGBA8888:
        //         return encodeRGBA8888(br, width, height);
        //     case (uint8_t)CtrTexFormat::RGB888:
        //         return decodeRGB888(br, width, height);
        //     case (uint8_t)CtrTexFormat::RGBA5551:
        //         return decodeRGBA5551(br, width, height);
        //     case (uint8_t)CtrTexFormat::RGB565:
        //         return decodeRGB565(br, width, height);
        //     case (uint8_t)CtrTexFormat::RGBA4444:
        //         return decodeRGBA4444(br, width, height);
        //     case (uint8_t)CtrTexFormat::LA88:
        //         return decodeLA88(br, width, height);
        //     case (uint8_t)CtrTexFormat::HL8:
        //         return decodeHL8(br, width, height);
        //     case (uint8_t)CtrTexFormat::L8:
        //         return decodeL8(br, width, height);
        //     case (uint8_t)CtrTexFormat::A8:
        //         return decodeA8(br, width, height);
        //     case (uint8_t)CtrTexFormat::LA44:
        //         return decodeLA44(br, width, height);
        //     case (uint8_t)CtrTexFormat::L4:
        //         return decodeL4(br, width, height);
        //     case (uint8_t)CtrTexFormat::A4:
        //         return decodeA4(br, width, height);
        //     case (uint8_t)CtrTexFormat::ETC1:
        //         return decodeETC1(br, width, height);
        //     case (uint8_t)CtrTexFormat::ETC1A4:
        //         return decodeETC1A4(br, width, height);
        //     default:
        //         throw std::runtime_error("Unimplemented CTR texture format!");
        //         //return nullptr;
        // }
    }

    uint8_t TextureCodec::returnGeneralTextureType(uint8_t texFmt)
    {
        switch (texFmt)
        {
            case (uint8_t)CtrTexFormat::RGBA8888:
                return (uint8_t)ImageFormats::RGBA8;
            case (uint8_t)CtrTexFormat::RGB888:
                return (uint8_t)ImageFormats::RGB8;
            case (uint8_t)CtrTexFormat::RGBA5551:
                return (uint8_t)ImageFormats::RGB5A1;
            case (uint8_t)CtrTexFormat::RGB565:
                return (uint8_t)ImageFormats::RGB565;
            case (uint8_t)CtrTexFormat::RGBA4444:
                return (uint8_t)ImageFormats::RGBA4;;
            case (uint8_t)CtrTexFormat::LA88:
                return (uint8_t)ImageFormats::L8A;
            case (uint8_t)CtrTexFormat::HL8:
                return (uint8_t)ImageFormats::HL8;
            case (uint8_t)CtrTexFormat::L8:
                return (uint8_t)ImageFormats::L8;
            case (uint8_t)CtrTexFormat::A8:
                return (uint8_t)ImageFormats::A8;
            case (uint8_t)CtrTexFormat::LA44:
                return (uint8_t)ImageFormats::L4A;
            case (uint8_t)CtrTexFormat::L4:
                return (uint8_t)ImageFormats::L4;
            case (uint8_t)CtrTexFormat::A4:
                return (uint8_t)ImageFormats::A4;
            case (uint8_t)CtrTexFormat::ETC1:
                return (uint8_t)ImageFormats::ETC1;
            case (uint8_t)CtrTexFormat::ETC1A4:
                return (uint8_t)ImageFormats::ETC1A4;
            default:
                throw std::runtime_error("Unsupported CTR texture format!");
                //return 0xFF;
        }
    }

    uint8_t TextureCodec::returnPlatformTextureType(uint8_t generalFmt)
    {
        switch (generalFmt)
        {
            case (uint8_t)ImageFormats::RGBA8:
                return (uint8_t)CtrTexFormat::RGBA8888;
            case (uint8_t)ImageFormats::RGB8:
                return (uint8_t)CtrTexFormat::RGB888;
            case (uint8_t)ImageFormats::RGB5A1:
                return (uint8_t)CtrTexFormat::RGBA5551;
            case (uint8_t)ImageFormats::RGB565:
                return (uint8_t)CtrTexFormat::RGB565;
            case (uint8_t)ImageFormats::RGBA4:
                return (uint8_t)CtrTexFormat::RGBA4444;
            case (uint8_t)ImageFormats::L8A:
                return (uint8_t)CtrTexFormat::LA88;
            case (uint8_t)ImageFormats::HL8:
                return (uint8_t)CtrTexFormat::HL8;
            case (uint8_t)ImageFormats::L8:
                return (uint8_t)CtrTexFormat::L8;
            case (uint8_t)ImageFormats::A8:
                return (uint8_t)CtrTexFormat::A8;
            case (uint8_t)ImageFormats::L4A:
                return (uint8_t)CtrTexFormat::LA44;
            case (uint8_t)ImageFormats::L4:
                return (uint8_t)CtrTexFormat::L4;
            case (uint8_t)ImageFormats::A4:
                return (uint8_t)CtrTexFormat::A4;
            case (uint8_t)ImageFormats::ETC1:
                return (uint8_t)CtrTexFormat::ETC1;
            case (uint8_t)ImageFormats::ETC1A4:
                return (uint8_t)CtrTexFormat::ETC1A4;
            default:
                return (uint8_t)CtrTexFormat::RGBA8888; //In case it's something unknown, default to RGBA8
        }
    }


    const uint8_t tileOrder[] = { 0, 1, 8, 9, 2, 3, 10, 11, 16, 17, 24, 25, 18, 19, 26, 27, 4, 5, 12, 13, 6, 7, 14, 15, 20, 21, 28, 29, 22, 23, 30, 31, 32, 33, 40, 41, 34, 35, 42, 43, 48, 49, 56, 57, 50, 51, 58, 59, 36, 37, 44, 45, 38, 39, 46, 47, 52, 53, 60, 61, 54, 55, 62, 63 };
    const int16_t etc1LUT[][4] = 
    { 
        { 2, 8, -2, -8 },
        { 5, 17, -5, -17 },
        { 9, 29, -9, -29 },
        { 13, 42, -13, -42 },
        { 18, 60, -18, -60 },
        { 24, 80, -24, -80 },
        { 33, 106, -33, -106 },
        { 47, 183, -47, -183 }
    };

    uint8_t *TextureCodec::decodeRGBA8888(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < height / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    uint8_t alpha = br->readByte();
                    uint8_t red = br->readByte();
                    uint8_t green = br->readByte();
                    uint8_t blue = br->readByte();

                    argbBuf[outputOffset + 0] = blue;
                    argbBuf[outputOffset + 1] = green;
                    argbBuf[outputOffset + 2] = red;
                    argbBuf[outputOffset + 3] = alpha;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeRGB888(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < height / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    uint8_t red = br->readByte();
                    uint8_t green = br->readByte();
                    uint8_t blue = br->readByte();

                    argbBuf[outputOffset + 0] = blue;
                    argbBuf[outputOffset + 1] = green;
                    argbBuf[outputOffset + 2] = red;
                    argbBuf[outputOffset + 3] = 0xFF;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeRGBA5551(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < height / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    uint16_t pixelData = ((uint16_t)br->readByte() | (uint16_t)br->readByte() << 8);

                    uint8_t red = ((pixelData >> 1) & 0x1f) << 3;
                    uint8_t green = ((pixelData >> 6) & 0x1f) << 3;
                    uint8_t blue = ((pixelData >> 11) & 0x1f) << 3;
                    uint8_t alpha = (pixelData & 1) * 0xff;

                    argbBuf[outputOffset + 0] = (blue | (blue >> 5));
                    argbBuf[outputOffset + 1] = (green | (green >> 5));
                    argbBuf[outputOffset + 2] = (red | (red >> 5));
                    argbBuf[outputOffset + 3] = alpha;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeRGB565(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < height / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    uint16_t pixelData = ((uint16_t)br->readByte() | (uint16_t)br->readByte() << 8);

                    uint8_t red = ((pixelData >> 1) & 0x1f) << 3;
                    uint8_t green = ((pixelData >> 5) & 0x3f) << 2;
                    uint8_t blue = ((pixelData >> 11) & 0x1f) << 3;

                    argbBuf[outputOffset + 0] = (blue | (blue >> 5));
                    argbBuf[outputOffset + 1] = (green | (green >> 6));
                    argbBuf[outputOffset + 2] = (red | (red >> 5));
                    argbBuf[outputOffset + 3] = 0xFF;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeRGBA4444(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        return nullptr; //Stubbed
    }

    uint8_t *TextureCodec::decodeLA88(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < width / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    if (x >= width || y >= height) continue;

                    uint16_t data = br->readByte();
                    uint16_t alpha = br->readByte();

                    argbBuf[outputOffset + 0] = data;
                    argbBuf[outputOffset + 1] = data;
                    argbBuf[outputOffset + 2] = data;
                    argbBuf[outputOffset + 3] = alpha;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeHL8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        return nullptr; //Stubbed
    }

    uint8_t *TextureCodec::decodeL8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        return nullptr; //Stubbed
    }

    uint8_t *TextureCodec::decodeA8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < width / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    if (x >= width || y >= height) continue;

                    uint16_t data = br->readByte();

                    argbBuf[outputOffset + 0] = 0xFF;
                    argbBuf[outputOffset + 1] = 0xFF;
                    argbBuf[outputOffset + 2] = 0xFF;
                    argbBuf[outputOffset + 3] = data;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeLA44(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < width / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel++)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    if (x >= width || y >= height) continue;

                    uint16_t data = br->readByte();
                    uint8_t newpixel = (uint8_t)((data & 0x0F) * 0x11);
                    uint8_t alpha = (uint8_t)((data >> 4) * 0x11);

                    argbBuf[outputOffset + 0] = newpixel;
                    argbBuf[outputOffset + 1] = newpixel;
                    argbBuf[outputOffset + 2] = newpixel;
                    argbBuf[outputOffset + 3] = alpha;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeL4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < width / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel += 2)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    if (x >= width || y >= height) continue;

                    uint16_t data = br->readByte();
                    uint8_t newpixel = (uint8_t)((data & 0x0F) * 0x11);

                    argbBuf[outputOffset + 0] = newpixel;
                    argbBuf[outputOffset + 1] = newpixel;
                    argbBuf[outputOffset + 2] = newpixel;
                    if (newpixel == 0U) argbBuf[outputOffset + 3] = 0x0;
                    else argbBuf[outputOffset + 3] = 0xFF;
                    argbBuf[outputOffset + 3] = 0xFF;

                    newpixel = (uint8_t)((data >> 4) * 0x11);

                    argbBuf[outputOffset + 4] = newpixel;
                    argbBuf[outputOffset + 5] = newpixel;
                    argbBuf[outputOffset + 6] = newpixel;
                    if (newpixel == 0U) argbBuf[outputOffset + 7] = 0x0;
                    else argbBuf[outputOffset + 7] = 0xFF;
                    argbBuf[outputOffset + 7] = 0xFF;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeA4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t tY = 0; tY < height / 8; tY++)
        {
            for (uint16_t tX = 0; tX < width / 8; tX++)
            {
                for (uint16_t pixel = 0; pixel < 64; pixel += 2)
                {
                    int x = tileOrder[pixel] % 8;
                    int y = (tileOrder[pixel] - x) / 8;
                    int outputOffset = ((tX * 8) + x + ((tY * 8 + y) * width)) * 4;

                    if (x >= width || y >= height) continue;

                    uint16_t data = br->readByte();
                    uint8_t newpixel = (uint8_t)((data & 0x0F) * 0x11);

                    argbBuf[outputOffset + 0] = 0xFF;
                    argbBuf[outputOffset + 1] = 0xFF;
                    argbBuf[outputOffset + 2] = 0xFF;
                    argbBuf[outputOffset + 3] = newpixel;

                    newpixel = (uint8_t)((data >> 4) * 0x11);

                    argbBuf[outputOffset + 4] = 0xFF;
                    argbBuf[outputOffset + 5] = 0xFF;
                    argbBuf[outputOffset + 6] = 0xFF;
                    argbBuf[outputOffset + 7] = newpixel;
                }
            }
        }
        return argbBuf;
    }

    uint8_t *TextureCodec::decodeETC1(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        return nullptr; //Stubbed
    }

    uint8_t *TextureCodec::decodeETC1A4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        return nullptr; //Stubbed
    }

}
