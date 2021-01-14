// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source code for BitReader class
#include "bitreader.h"

namespace BinaryTools
{
    BitReader::BitReader(BinaryReader *br)
    {
        this->br = br;
        bytePosition = br->getPosition();
        bitPosition = bytePosition * 8U;
        buffer = br->readByte();
    }

    bool BitReader::isByteAligned()
    {
        return (bitPosition % 8 == 0 ? true : false);
    }

    uint8_t BitReader::readBits(uint8_t count)
    {
        uint8_t result = 0U;
        for (uint8_t i = count - 1; i >= 0U && i < count; i--)
        {
            result |= readBit() << i;
        }
        return result;
    }

    uint8_t BitReader::readBitsBackwards(uint8_t count)
    {
        uint8_t result = 0U;
        for (uint8_t i = 0; i < count; i++)
        {
            result |= readBit() << i;
        }
        return result;
    }

    uint8_t BitReader::readBitsNormalized(uint8_t count)
    {
        return round(((double)readBits(count) / (double)((1 << count) - 1)) * 255.0);
    }

    uint8_t BitReader::readBitsNormalizedBackwards(uint8_t count)
    {
        return round(((double)readBitsBackwards(count) / (double)((1 << count) - 1)) * 255.0);
    }

    uint8_t BitReader::readBit()
    {
        uint64_t bytepos = bitPosition / 8; //Calculate which byte we should be reading
        if (bytepos != bytePosition) //If it's not the same as already read, then update buffer and positions
        {
            bytePosition = bytepos; //Update byte position
            br->setPosition(bytepos); //Set position
            buffer = br->readByte();
        }
        uint8_t value = (buffer >> (7 - (bitPosition % 8))) & 1U;
        bitPosition++; //Increment the bit postion for the next read
        return value;
    }

    void BitReader::update()
    {
        bytePosition = br->getPosition();
        bitPosition = bytePosition * 8U;
        buffer = br->readByte();
    }

    void BitReader::updatePosition(uint64_t pos)
    {
        br->setPosition(pos);
        bytePosition = pos;
        bitPosition = bytePosition * 8U;
        buffer = br->readByte();
    }

    BitReader::~BitReader()
    {
        //br doesn't belong to us, so we don't delete it
    }
}
