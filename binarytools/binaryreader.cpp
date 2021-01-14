// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for BinaryReader class
#include <fmt/core.h>

#include "binaryreader.h"
#include "endianness.h"

#define open_err "Couldn't open file for reading."

namespace BinaryTools
{
    BinaryReader::BinaryReader(std::string_view path, bool isLE)
        : std::ifstream(path.data(), std::ios::in | ios_base::binary)
    {
        setEndianness(isLE);
    }

    void BinaryReader::readEndiannessByte(size_t offset)
    {
        setPosition(offset);
        uint8_t bom = readByte();
        bool isLE;
        if (bom == 0xFE) isLE = false;
        else if (bom == 0xFF) isLE = true;
        else throw std::runtime_error(fmt::format("Error {}:{}: {}: Unknown endianness byte 0x{:02X} at 0x{:04X}!", __FILE__, __LINE__, __PRETTY_FUNCTION__, bom, offset));
        setEndianness(isLE);
        setPosition(0x0);
    }

    void BinaryReader::setEndianness(bool isLE)
    {
        flipBytes = (Endianness::isSystemLittleEndian() != isLE);
    }

    bool BinaryReader::isLittleEndian()
    {
        if (Endianness::isSystemLittleEndian())
        {
            if (!flipBytes) return ENDIANNESS_LITTLE;
            else return ENDIANNESS_BIG;
        }
        else
        {
            if (!flipBytes) return ENDIANNESS_BIG;
            else return ENDIANNESS_LITTLE;
        }
    }

    size_t BinaryReader::getTrueFileSize()
    {
        size_t currPos = std::ifstream::tellg();
        std::ifstream::seekg(0, std::ios_base::seekdir::_S_end);
        size_t result = (size_t)std::ifstream::tellg();
        std::ifstream::seekg(currPos);
        return result;
    }

    uint8_t BinaryReader::readByte()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        uint8_t result = 0;
        std::ifstream::read((char *)&result, 1);
        return result;
    }

    uint8_t BinaryReader::peekByte()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        uint8_t result = 0;
        std::ifstream::read((char *)&result, 1);
        advancePosition(-1);
        return result;
    }

    int8_t BinaryReader::readSByte()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        int8_t result = 0;
        std::ifstream::read((char *)&result, 1);
        return (int8_t &)result;
    }

    char BinaryReader::readChar()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        uint8_t result = 0;
        std::ifstream::read((char *)&result, 1);
        return (char &)result;
    }

    int16_t BinaryReader::readInt16()
    {
        return (int16_t)readUInt16();
    }

    uint16_t BinaryReader::readUInt16()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        uint8_t temp[2] = {0, 0};
        std::ifstream::read((char *)temp, 2);
        if (!flipBytes) return *(uint16_t *)temp;
        return (uint16_t)((uint16_t)temp[0] << 8U | (uint16_t)temp[1]);
    }

    int32_t BinaryReader::readInt32()
    {
        return (int32_t)readUInt32();
    }

    uint32_t BinaryReader::readUInt32()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        uint8_t temp[4] = {0, 0, 0, 0};
        std::ifstream::read((char *)temp, 4);
        if (!flipBytes) return *(uint32_t *)temp;
        return (uint32_t)((uint32_t)temp[0] << 24U | (uint32_t)temp[1] << 16U | (uint32_t)temp[2] << 8U | (uint32_t)temp[3]);
    }

    uint32_t BinaryReader::peekUInt32()
    {
        auto result = readUInt32();
        advancePosition(-4);
        return result;
    }

    int64_t BinaryReader::readInt64()
    {
        return (int64_t)readUInt64();
    }

    uint64_t BinaryReader::readUInt64()
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);
        uint8_t temp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        std::ifstream::read((char *)temp, 8);
        if (!flipBytes) return *(uint64_t *)temp;
        return (uint64_t)((uint64_t)temp[0] << 56 | (uint64_t)temp[1] << 48 | (uint64_t)temp[2] << 40 | (uint64_t)temp[3] << 32 | (uint64_t)temp[4] << 24 | (uint64_t)temp[5] << 16 | (uint64_t)temp[6] << 8 | (uint64_t)temp[7]);
    }

    float BinaryReader::readSingle()
    {
        uint32_t val = readUInt32();
        return (float &)val;
    }

    double BinaryReader::readDouble()
    {
        uint64_t val = readUInt64();
        return (double &)val;
    }

    uint8_t *BinaryReader::readBytes(size_t count)
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);

        if (count == 0) return nullptr;

        uint8_t* result = new uint8_t[count];

        std::ifstream::read((char *)result, count);

        return result;
    }

    void BinaryReader::readBytes(uint8_t *buffer, size_t count)
    {
        if (!std::ifstream::is_open()) throw std::runtime_error(open_err);

        if (count == 0) return;

        std::ifstream::read((char *)buffer, count);

        return;
    }

    bool BinaryReader::bytearraycmp(uint8_t *a, uint8_t *b, size_t length)
    {
        for (unsigned i = 0; i < length; i++)
        {
            if (*a != *b) return false;
            a++;
            b++;
        }
        return true;
    }

    size_t BinaryReader::getPosition()
    {
        return std::ifstream::tellg();
    }

    void BinaryReader::setPosition(size_t pos)
    {
        seekg(pos);
    }

    const char *BinaryReader::readCString()
    {
        size_t start = tellg();
        size_t length = 0;
        while (readByte() != 0) length++;
        length++;
        char *str = new char[length];
        seekg(start);
        read(str, length);
        return str;
    }

    void BinaryReader::advancePosition(int32_t delta)
    {
        seekg((int32_t)std::ifstream::tellg() + delta);
    }

    BinaryReader::~BinaryReader()
    {
        BinaryReader::close();
    }

    void BinaryReader::close()
    {
        std::ifstream::close();
    }
}
