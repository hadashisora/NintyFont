// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for BinaryWriter class
#include "binarywriter.h"
#include "endianness.h"

namespace BinaryTools
{
    BinaryWriter::BinaryWriter(std::string path, bool isLE)
        : std::ofstream(path, std::ios::out | ios_base::binary)
    {
        setEndianness(isLE);
    }

    void BinaryWriter::setEndianness(bool isLE)
    {
        flipBytes = (Endianness::isSystemLittleEndian() != isLE);
    }

    bool BinaryWriter::isLittleEndian()
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

    BinaryWriter::~BinaryWriter()
    {
        BinaryWriter::close();
    }

    void BinaryWriter::close()
    {
        std::ofstream::close();
    }

    size_t BinaryWriter::getPosition()
    {
        return std::ofstream::tellp();
    }

    void BinaryWriter::setPosition(size_t pos)
    {
        seekp(pos);
    }

    void BinaryWriter::write(uint8_t value)
    {
        std::ofstream::put(value);
    }

    void BinaryWriter::write(int8_t value)
    {
        std::ofstream::put((uint8_t &)value);
    }

    void BinaryWriter::write(uint8_t *buffer, int32_t index, int32_t count)
    {
        for (int32_t i = index; i < index + count; i++)
        {
            std::ofstream::put(buffer[i]);
        }
    }

    void BinaryWriter::write(std::vector<uint8_t> *data)
    {
        for (auto i = data->begin(); i != data->end(); i++)
        {
            std::ofstream::put(*i);
        }
    }

    void BinaryWriter::write(float value)
    {
        write((uint32_t &)value);
    }

    void BinaryWriter::write(double value)
    {
        write((uint64_t &)value);
    }

    void BinaryWriter::write(int16_t value)
    {
        write((uint16_t &)value);
    }

    void BinaryWriter::write(uint16_t value)
    {
        uint8_t *temp = (uint8_t *)&value;
        if (!flipBytes)
        {
            std::ofstream::put(temp[0]);
            std::ofstream::put(temp[1]);
        }
        else
        {
            std::ofstream::put(temp[1]);
            std::ofstream::put(temp[0]);
        }
    }

    void BinaryWriter::write(int32_t value)
    {
        write((uint32_t &)value);
    }

    void BinaryWriter::write(uint32_t value)
    {
        uint8_t *temp = (uint8_t *)&value;
        if (!flipBytes)
        {
            std::ofstream::put(temp[0]);
            std::ofstream::put(temp[1]);
            std::ofstream::put(temp[2]);
            std::ofstream::put(temp[3]);
        }
        else
        {
            std::ofstream::put(temp[3]);
            std::ofstream::put(temp[2]);
            std::ofstream::put(temp[1]);
            std::ofstream::put(temp[0]);
        }
    }

    void BinaryWriter::write(int64_t value)
    {
        write((uint64_t &)value);
    }

    void BinaryWriter::write(uint64_t value)
    {
        uint8_t *temp = (uint8_t *)&value;
        if (!flipBytes)
        {
            std::ofstream::put(temp[0]);
            std::ofstream::put(temp[1]);
            std::ofstream::put(temp[2]);
            std::ofstream::put(temp[3]);
            std::ofstream::put(temp[4]);
            std::ofstream::put(temp[5]);
            std::ofstream::put(temp[6]);
            std::ofstream::put(temp[7]);
        }
        else
        {
            std::ofstream::put(temp[7]);
            std::ofstream::put(temp[6]);
            std::ofstream::put(temp[5]);
            std::ofstream::put(temp[4]);
            std::ofstream::put(temp[3]);
            std::ofstream::put(temp[2]);
            std::ofstream::put(temp[1]);
            std::ofstream::put(temp[0]);
        }
    }
}
