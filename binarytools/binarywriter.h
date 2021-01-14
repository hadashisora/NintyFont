// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for BinaryWriter class
// This allows one to easily write binary
// data to files both in LE and BE (platform
// endianness doesn't matter thanks to Endianness
// helper class)
// The API is mostly similar to that of System.IO.BinaryWriter
// in Microsoft's .NET Framework
#pragma once

#include <fstream>
#include <vector>
#include <cstdint>

namespace BinaryTools
{
    class BinaryWriter : std::ofstream
    {
    private:
        bool flipBytes; //False if CPU endianness and file endianness match, otherwise true
    public:
        //Ctor
        BinaryWriter(std::string path, bool t_isLittleEndian = true);
        //Dtor
        ~BinaryWriter();
        //Methods
        void close(void);
        void setEndianness(bool isLE);
        bool isLittleEndian(void);
        size_t getPosition(void);
        void setPosition(size_t pos);
        void write(uint8_t value);
        void write(int8_t value);
        void write(uint8_t *buffer, int32_t index, int32_t count);
        void write(std::vector<uint8_t> *data);
        void write(float value);
        void write(double value);
        void write(int16_t value);
        void write(uint16_t value);
        void write(int32_t value);
        void write(uint32_t value);
        void write(int64_t value);
        void write(uint64_t value);
    };
}
