// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2018-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include <cstdio>

#include "bfttf.h"
#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/endianness.h"

namespace NintyFont
{
    BFTTF::Result BFTTF::decrypt(std::string inpath, std::string outpath)
    {
        BinaryTools::BinaryReader *br = new BinaryTools::BinaryReader(inpath, false); //These are always BE
        uint32_t magic = br->readUInt32();
        uint32_t key = 0;
        switch (magic)
        {
            case 0x36F81A1E: //NX
                key = 1231165446U;
                break;
            case 0xF368DEC1: //Cafe
                key = 2364726489U;
                break;
            case 0xD99B871A: //Win32
                key = 2785117442U;
                break;
            default:
                br->close();
                return Result::WRONG_FILE;
        }

        uint32_t test1 = 0x36F81A1E ^ 1231165446U;
        uint32_t test2 = test1 ^ 1231165446U;

        uint32_t trueLen = br->getTrueFileSize();
        if (trueLen <= 8) 
        {
            br->close();
            return Result::INVALID_FILE;
        }
        uint32_t length = br->readUInt32() ^ key;
        if (trueLen < length) 
        {
            br->close();
            return Result::INVALID_FILE;
        }

        BinaryTools::BinaryWriter *bw = new BinaryTools::BinaryWriter(outpath, false);
        for (uint32_t i = 0; i < length; i += 4)
        {
            bw->write(br->readUInt32() ^ key);
        }

        delete bw;
        delete br;

        return Result::OK;
    }

    BFTTF::Result BFTTF::encrypt(std::string inpath, std::string outpath, Platform platform)
    {
        uint32_t key = 0;
        switch (platform)
        {
            case Platform::NX:
                key = 1231165446U;
                break;
            case Platform::CAFE:
                key = 2364726489U;
                break;
            case Platform::WIN:
                key = 2785117442U;
                break;
            default:
                return Result::ERR;
        }

        BinaryTools::BinaryReader *br = new BinaryTools::BinaryReader(inpath, false); //Always BE
        BinaryTools::BinaryWriter *bw = new BinaryTools::BinaryWriter(outpath, false);

        uint32_t inSize = br->getTrueFileSize();

        bw->write((uint32_t)(2140799512U ^ key));
        bw->write((uint32_t)((uint32_t)inSize ^ key));

        for (uint32_t i = 0; i < inSize; i += 4)
        {
            bw->write((uint32_t)(br->readUInt32() ^ key));
        }

        delete bw;
        delete br;

        return Result::OK;
    }
}
