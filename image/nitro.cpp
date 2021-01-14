// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source code for decoding/encoding NTR bitmaps
#include "texturecodec.h"

namespace NintyFont::NTR::Image
{
    uint8_t *TextureCodec::decodeBitmap(uint8_t bpp, BinaryTools::BitReader *br, uint16_t width, uint16_t height)
    {
        uint8_t* imgBuf = new uint8_t[width * height * 4];
        for (uint16_t y = 0; y < height; y++)
        {
            for (uint16_t x = 0; x < width; x++)
            {
                uint8_t px = br->readBitsNormalizedBackwards(bpp);
                imgBuf[((y * width + x) * 4) + 0] = px;
                imgBuf[((y * width + x) * 4) + 1] = px;
                imgBuf[((y * width + x) * 4) + 2] = px;
                if (px == 0) imgBuf[((y * width + x) * 4) + 3] = 0x0;
                else imgBuf[((y * width + x) * 4) + 3] = 0xFF;
                //imgBuf[((y * width + x) * 4) + 3] = 0xFF;
            }
        }
        return imgBuf;
    }

    uint8_t flipBitOrder(uint8_t bits, uint8_t bpp)
    {
        uint8_t result = 0;
        for (uint8_t i = 0; i < 8; i++) result |= ((bits & (1 << i)) >> i) << (7 - i);
        return result >> (8 - bpp);
    }

    void TextureCodec::encodeBitmap(uint8_t *argb, uint8_t *dst, uint8_t bpp, uint16_t width, uint16_t height)
    {
        uint8_t bitpos = 0;
        double conv = ((double)(1 << 8) - 1.0) / ((double)(1 << bpp) - 1.0);
        for (uint16_t y = 0; y < height; y++)
        {
            for (uint16_t x = 0; x < width; x++)
            {
                uint8_t red = argb[((y * width + x) * 4) + 0];
                uint8_t green = argb[((y * width + x) * 4) + 1];
                uint8_t blue = argb[((y * width + x) * 4) + 2];
                uint8_t alpha = argb[((y * width + x) * 4) + 3];
                double floatpixel = (((double)((double)red + (double)green + (double)blue) / 3.0) * ((double)alpha / 255.0)) / conv;
                uint8_t bytepx = flipBitOrder((uint8_t)std::round(floatpixel), bpp);
                //uint8_t bytepx = std::round(floatpixel);
                int8_t shift = (8 - (int8_t)bpp) - (int8_t)bitpos;
                if (shift >= 0)
                {
                    *dst |= bytepx << shift;
                }
                else 
                {
                    *dst |= bytepx >> abs(shift);
                }
                bitpos += bpp;
                if (bitpos >= 8)
                {
                    bitpos %= 8;
                    dst++;
                    if (bitpos != 0) *dst |= bytepx << (8 - (8 - bitpos));
                }
            }
        }
    }
}
