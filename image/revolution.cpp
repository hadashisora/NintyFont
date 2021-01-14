// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// DOL/RVL texture encoders/decoders from
// TPLLib by Tempus and RoadrunnerWMC, ported to C++
#include "texturecodec.h"

namespace NintyFont::RVL::Image
{
    uint8_t* TextureCodec::decodeTexture(uint8_t texFmt, BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        switch (texFmt)
        {
            case (uint8_t)DolRvlTexFmts::I4:
                return decodeI4(br, width, height);
            case (uint8_t)DolRvlTexFmts::I8:
                return decodeI8(br, width, height);
            case (uint8_t)DolRvlTexFmts::IA4:
                return decodeIA4(br, width, height);
            case (uint8_t)DolRvlTexFmts::IA8:
                return decodeIA8(br, width, height);
            case (uint8_t)DolRvlTexFmts::RGB565:
                return decodeRGB565(br, width, height);
            case (uint8_t)DolRvlTexFmts::RGB5A3:
                return decodeRGB5A3(br, width, height);
            case (uint8_t)DolRvlTexFmts::RGBA8:
                return decodeRGBA8(br, width, height);
            case (uint8_t)DolRvlTexFmts::C4: //Intentional fall-through to unimplemented message for all items below
            case (uint8_t)DolRvlTexFmts::C8:
            case (uint8_t)DolRvlTexFmts::CMPR:
                throw std::runtime_error("Unimplemented RVL texture format!");
                break;
            default:
                return nullptr;
        }
    }

    std::vector<uint8_t> *TextureCodec::encodeTexture(uint8_t texFmt, uint8_t *data, uint16_t width, uint16_t height)
    {
        switch (texFmt)
        {
            case (uint8_t)DolRvlTexFmts::I4:
                return encodeI4(data, width, height);
            case (uint8_t)DolRvlTexFmts::I8:
                return encodeI8(data, width, height);
            case (uint8_t)DolRvlTexFmts::IA4:
                return encodeIA4(data, width, height);
            case (uint8_t)DolRvlTexFmts::IA8:
                return encodeIA8(data, width, height);
            case (uint8_t)DolRvlTexFmts::RGB565:
                return encodeRGB565(data, width, height);
            case (uint8_t)DolRvlTexFmts::RGB5A3:
                return encodeRGB5A3(data, width, height);
            case (uint8_t)DolRvlTexFmts::RGBA8:
                return encodeRGBA8(data, width, height);
            case (uint8_t)DolRvlTexFmts::C4: //Intentional fall-through to unimplemented message for all items below
            case (uint8_t)DolRvlTexFmts::C8:
            case (uint8_t)DolRvlTexFmts::CMPR:
                throw std::runtime_error("Unimplemented RVL texture format!");
                break;
            default:
                return nullptr;
        }
    }

    uint8_t TextureCodec::returnGeneralTextureType(uint8_t texFmt)
    {
        switch (texFmt)
        {
            case (uint8_t)DolRvlTexFmts::I4:
                return (uint8_t)ImageFormats::L4;
            case (uint8_t)DolRvlTexFmts::I8:
                return (uint8_t)ImageFormats::L8;
            case (uint8_t)DolRvlTexFmts::IA4:
                return (uint8_t)ImageFormats::L4A;
            case (uint8_t)DolRvlTexFmts::IA8:
                return (uint8_t)ImageFormats::L8A;
            case (uint8_t)DolRvlTexFmts::RGB565:
                return (uint8_t)ImageFormats::RGB565;
            case (uint8_t)DolRvlTexFmts::RGB5A3:
                return (uint8_t)ImageFormats::RGB5A3;
            case (uint8_t)DolRvlTexFmts::RGBA8:
                return (uint8_t)ImageFormats::RGBA8;
            case (uint8_t)DolRvlTexFmts::C4: //Intentional fall-through to unimplemented message for all items below
            case (uint8_t)DolRvlTexFmts::C8:
            case (uint8_t)DolRvlTexFmts::CMPR:
                throw std::runtime_error("Unimplemented RVL texture format!");
                break;
            default:
                return 0xFF;
        }
    }

    uint8_t TextureCodec::returnPlatformTextureType(uint8_t generalFmt)
    {
        switch (generalFmt)
        {
            case (uint8_t)ImageFormats::L4:
                return (uint8_t)DolRvlTexFmts::I4;
            case (uint8_t)ImageFormats::L8:
                return (uint8_t)DolRvlTexFmts::I8;
            case (uint8_t)ImageFormats::L4A:
                return (uint8_t)DolRvlTexFmts::IA4;
            case (uint8_t)ImageFormats::L8A:
                return (uint8_t)DolRvlTexFmts::IA8;
            case (uint8_t)ImageFormats::RGB565:
                return (uint8_t)DolRvlTexFmts::RGB565;
            case (uint8_t)ImageFormats::RGB5A3:
                return (uint8_t)DolRvlTexFmts::RGB5A3;
            case (uint8_t)ImageFormats::RGBA8:
                return (uint8_t)DolRvlTexFmts::RGBA8;
            default:
                return (uint8_t)DolRvlTexFmts::RGBA8; //In case it's something unknown, default to RGBA8
        }
    }

    uint8_t* TextureCodec::decodeI4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 8)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 8)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 8; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 8; xpixel += 2)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t data = br->readByte();
                        uint8_t newpixel = (uint8_t)((data >> 4) * 0x11);

                        argbBuf[(((ypixel * width) + xpixel) * 4) + 0] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = newpixel;
                        if (newpixel == 0U) argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = 0x0U;
                        else argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = 0xFFU;

                        newpixel = (uint8_t)((data & 0x0F) * 0x11);

                        argbBuf[(((ypixel * width) + xpixel) * 4) + 4] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 5] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 6] = newpixel;
                        if (newpixel == 0U) argbBuf[(((ypixel * width) + xpixel) * 4) + 7] = 0x0U;
                        else argbBuf[(((ypixel * width) + xpixel) * 4) + 7] = 0xFFU;
                    }
                }
            }
        }
        return argbBuf;
    }

    uint8_t* TextureCodec::decodeI8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile +=8)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 8; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t newpixel = br->readByte();

                        argbBuf[(((ypixel * width) + xpixel) * 4) + 0] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = newpixel;
                        if (newpixel == 0U) argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = 0x0U;
                        else argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = 0xFFU;
                    }
                }
            }
        }
        return argbBuf;
    }

    uint8_t* TextureCodec::decodeIA4(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 8)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 8; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t data = br->readByte();
                        uint8_t alpha = (uint8_t)((data >> 4) * 0x11);
                        uint8_t newpixel = (uint8_t)((data & 0x0F) * 0x11);

                        argbBuf[(((ypixel * width) + xpixel) * 4) + 0] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = alpha;
                    }
                }
            }
        }
        return argbBuf;
    }

    uint8_t* TextureCodec::decodeIA8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t newpixel = br->readByte();

                        uint8_t alpha = br->readByte();

                        argbBuf[((ypixel * width) + xpixel) * 4] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = newpixel;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = alpha;
                    }
                }
            }
        }
        return argbBuf;
    }

    uint8_t* TextureCodec::decodeRGB565(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t data = br->readByte();

                        uint8_t blue = (uint8_t)((data & 0x1F) * 255 / 0x1F);

                        uint8_t green1 = (uint8_t)(data >> 5);
                        data = br->readByte();
                        uint8_t green2 = (uint8_t)(data & 0x7);

                        uint8_t green = (uint8_t)((green1 << 3) | (green2));

                        uint8_t red = (uint8_t)((data >> 3) * 255 / 0x1F);

                        uint8_t alpha = 0xFF;

                        argbBuf[(((ypixel * width) + xpixel) * 4) + 0] = blue;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = green;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = red;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = alpha;
                    }
                }
            }
        }
        return argbBuf;
    }

    uint8_t* TextureCodec::decodeRGB5A3(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint16_t newpixel = br->readUInt16();

                        uint32_t blue, green, red, alpha;
                        if ((newpixel & 0x8000) == 0x8000) //RGB555
                        {
                            uint32_t blue5 = (newpixel >> 10) & 0x1F;
                            uint32_t green5 = (newpixel >> 5) & 0x1F;
                            uint32_t red5 = newpixel & 0x1F;
                            blue = blue5 * 0x8;
                            green = green5 * 0x8;
                            red = red5 * 0x8;
                            alpha = 0xFF;
                        }
                        else //RGB4A3
                        {
                            uint32_t alpha3 = newpixel >> 12;
                            uint32_t blue4 = (newpixel >> 8) & 0xF;
                            uint32_t green4 = (newpixel >> 4) & 0xF;
                            uint32_t red4 = newpixel & 0xF;

                            alpha = alpha3 * 0x20;
                            blue = (blue4 * 0x11);
                            green = (green4 * 0x11);
                            red = (red4 * 0x11);
                        }

                        argbBuf[(((ypixel * width) + xpixel) * 4) + 0] = (uint8_t)red;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = (uint8_t)green;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = (uint8_t)blue;
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = (uint8_t)alpha;
                    }
                }
            }
        }
        return argbBuf;
    }

    uint8_t* TextureCodec::decodeRGBA8(BinaryTools::BinaryReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* argbBuf = new uint8_t[width * height * 4];
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                uint8_t alpha[16];
                uint8_t red[16];
                uint8_t green[16];
                uint8_t blue[16];

                try
                {
                    //Read Alpha and Red
                    for (int32_t i = 0; i < 16; i++)
                    {
                        alpha[i] = br->readByte();
                        red[i] = br->readByte();
                    }
                    //Read Green and Blue
                    for (int32_t i = 0; i < 16; i++)
                    {
                        green[i] = br->readByte();
                        blue[i] = br->readByte();
                    }
                }
                catch (std::exception())
                {
                    continue;
                }


                int32_t j = 0;
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 0] = blue[j];
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 1] = green[j];
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 2] = red[j];
                        argbBuf[(((ypixel * width) + xpixel) * 4) + 3] = alpha[j];
                        j++;
                    }
                }
            }
        }
        return argbBuf;
    }

    std::vector<uint8_t> *TextureCodec::encodeI4(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        std::vector<uint8_t> *i4Buf = new std::vector<uint8_t>((uint32_t)std::ceil(0.5F * width * height));
        int32_t i = 0;
        for (uint16_t ytile = 0; ytile < height; ytile += 8)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 8)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 8; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 8; xpixel += 2)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t blue = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        uint8_t red = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        float floatpixel = (((red + green + blue) / 3.0F) * (alpha / 255.0F)) / 17.0F;
                        uint8_t newpixel = std::round(floatpixel);

                        (*i4Buf)[i] = newpixel << 4;

                        blue = argbBuf[(((ypixel * width) + xpixel) * 4) + 4];
                        green = argbBuf[(((ypixel * width) + xpixel) * 4) + 5];
                        red = argbBuf[(((ypixel * width) + xpixel) * 4) + 6];
                        alpha = argbBuf[(((ypixel * width) + xpixel) * 4) + 7];
                        floatpixel = (((red + green + blue) / 3.0F) * (alpha / 255.0F)) / 17.0F;
                        newpixel = (uint8_t)std::round(floatpixel) & 0x0F;

                        (*i4Buf)[i] |= newpixel;
                        i++;
                    }
                }
            }
        }
        return i4Buf;
    }

    std::vector<uint8_t> *TextureCodec::encodeI8(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        std::vector<uint8_t> *i8Buf = new std::vector<uint8_t>(width * height);
        int32_t i = 0;
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 8)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 8; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t blue   = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green  = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        uint8_t red    = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha  = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        float floatpixel = ((red + green + blue) / 3.0F) * (alpha / 255.0F);
                        uint8_t newpixel = std::round(floatpixel);

                        (*i8Buf)[i] = newpixel;
                        i++;
                    }
                }
            }
        }
        return i8Buf;
    }

    std::vector<uint8_t> *TextureCodec::encodeIA4(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        std::vector<uint8_t> *ia4Buf = new std::vector<uint8_t>(width * height);
        int32_t i = 0;
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 8)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 8; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t blue   = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green  = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        uint8_t red    = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha  = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        float floatpixel = alpha / 17.0F;
                        uint8_t newpixel = std::round(floatpixel);
                        (*ia4Buf)[i] = newpixel << 4;

                        floatpixel = ((red + green + blue) / 3.0F) / 17.0F;
                        newpixel = std::round(floatpixel);
                        (*ia4Buf)[i] |= newpixel;

                        i++;
                    }
                }
            }
        }
        return ia4Buf;
    }

    std::vector<uint8_t> *TextureCodec::encodeIA8(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        std::vector<uint8_t> *ia8Buf = new std::vector<uint8_t>(width * height * 2);
        int32_t i = 0;
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        if (xpixel >= width || ypixel >= height) continue;

                        uint8_t blue   = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green  = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        uint8_t red    = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha  = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        float floatpixel = (red + green + blue) / 3.0F;
                        uint8_t newpixel = std::round(floatpixel);
                        (*ia8Buf)[i] = newpixel;
                        i++;

                        (*ia8Buf)[i] = alpha;
                        i++;
                    }
                }
            }
        }
        return ia8Buf;
    }

    std::vector<uint8_t> *TextureCodec::encodeRGB565(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        //This is broken...
        std::vector<uint8_t> *rgbBuf = new std::vector<uint8_t>(width * height * 2);
        int32_t i = 0;
        const double convRB = ((double)(1 << 8) - 1.0) / ((double)(1 << 5) - 1.0);
        const double convG = ((double)(1 << 8) - 1.0) / ((double)(1 << 6) - 1.0);
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        uint8_t blue   = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green  = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        uint8_t red    = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha  = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        uint16_t newpixelB = std::round(((double)blue * ((double)alpha / 255.0)) / convRB);
                        uint16_t newpixelG = std::round(((double)green * ((double)alpha / 255.0)) / convG);
                        uint16_t newpixelR = std::round(((double)red * ((double)alpha / 255.0)) / convRB);
                        uint16_t newpixel = newpixelR << 11 | newpixelG << 5 | newpixelB;
                        (*rgbBuf)[i] = newpixel >> 8;
                        i++;

                        (*rgbBuf)[i] = newpixel & 0xFF;
                        i++;
                    }
                }
            }
        }
        return rgbBuf;
    }

    std::vector<uint8_t> *TextureCodec::encodeRGB5A3(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        //This is also broken...
        std::vector<uint8_t> *rgbBuf = new std::vector<uint8_t>(width * height * 2);
        int32_t i = 0;
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        uint8_t blue   = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green  = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        uint8_t red    = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha  = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        uint16_t newpixel;
                        if (alpha < 238) //RGB4A3
                        {
                            uint8_t newpixelB = std::round(blue / 17.0);
                            uint8_t newpixelG = std::round(green / 17.0);
                            uint8_t newpixelR = std::round(red / 17.0);
                            uint8_t newpixelA = std::round(alpha / 7.0);
                            newpixel = (newpixelA << 12) | (newpixelR << 8) | (newpixelG << 4) | newpixelB;
                        }
                        else
                        {
                            uint8_t newpixelB = std::round(blue / 31.0);
                            uint8_t newpixelG = std::round(green / 31.0);
                            uint8_t newpixelR = std::round(red / 31.0);
                            newpixel = 0x8000 | (newpixelR << 10) | (newpixelG << 5) | newpixelB;
                        }
                        (*rgbBuf)[i] = newpixel >> 8;
                        i++;

                        (*rgbBuf)[i] = newpixel & 0xFF;
                        i++;
                    }
                }
            }
        }
        return rgbBuf;
    }

    std::vector<uint8_t> *TextureCodec::encodeRGBA8(uint8_t *argbBuf, uint16_t width, uint16_t height)
    {
        std::vector<uint8_t> *rgbBuf = new std::vector<uint8_t>(width * height * 4);
        int32_t i = 0;
        for (uint16_t ytile = 0; ytile < height; ytile += 4)
        {
            for (uint16_t xtile = 0; xtile < width; xtile += 4)
            {
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        uint8_t red    = argbBuf[(((ypixel * width) + xpixel) * 4) + 2];
                        uint8_t alpha  = argbBuf[(((ypixel * width) + xpixel) * 4) + 3];
                        (*rgbBuf)[i] = alpha;
                        i++;

                        (*rgbBuf)[i] = red;
                        i++;
                    }
                }
                for (uint16_t ypixel = ytile; ypixel < ytile + 4; ypixel++)
                {
                    for (uint16_t xpixel = xtile; xpixel < xtile + 4; xpixel++)
                    {
                        uint8_t blue   = argbBuf[(((ypixel * width) + xpixel) * 4) + 0];
                        uint8_t green  = argbBuf[(((ypixel * width) + xpixel) * 4) + 1];
                        (*rgbBuf)[i] = green;
                        i++;

                        (*rgbBuf)[i] = blue;
                        i++;
                    }
                }
            }
        }
        return rgbBuf;
    }
}
