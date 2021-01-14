// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for BinaryReader class
// This allows for easier reading of
// binary data from files in both LE and BE
// (platform endianness independent thanks to
// the Endianness helper class)
// The API is mostly similar to that of System.IO.BinaryReader
// in Microsoft's .NET Framework
#pragma once

#include <fstream>
#include <cstdint>

namespace BinaryTools
{
class BinaryReader : private std::ifstream
    {
    private:
        bool flipBytes; //False if CPU endianness and file endianness match, otherwise true
    public:
        //Constructor
        BinaryReader(std::string_view path, bool isLE = true);
        //Destructor
        ~BinaryReader();
        //Methods
        void close(void);
        void readEndiannessByte(size_t offset = 0x4);
        void setEndianness(bool isLE);
        bool isLittleEndian(void);
        size_t getPosition(void);
        size_t getTrueFileSize(void);
        void setPosition(size_t pos);
        void advancePosition(int32_t delta);
        bool readBoolean(void);
        uint8_t readByte(void);
        uint8_t peekByte(void);
        int8_t readSByte(void);
        char readChar(void);
        int16_t readInt16(void);
        uint16_t readUInt16(void);
        int32_t readInt32(void);
        uint32_t readUInt32(void);
        uint32_t peekUInt32(void);
        int64_t readInt64(void);
        uint64_t readUInt64(void);
        float readSingle(void);
        double readDouble(void);
        std::string readString(void);
        const char *readCString(void);
        char *readChars(size_t count);
        uint8_t *readBytes(size_t count);
        void readBytes(uint8_t *buffer, size_t count);
        static bool bytearraycmp(uint8_t *a, uint8_t *b, size_t length);
    };
}
