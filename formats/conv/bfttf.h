// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2018-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for BFTTF/BFOTF encryptor/decryptor
// static class. Mostly ported from my BFTTFutil,
// making that tool obsolete
#pragma once

#include <cstdint>
#include <string>

namespace NintyFont
{
    class BFTTF
    {
    public:
        //Enums
        enum class Platform
        {
            None,
            NX,
            CAFE,
            WIN
        };

        enum class Result
        {
            OK,
            WRONG_FILE,
            INVALID_FILE,
            ERR
        };
        //Static methods
        static Result decrypt(std::string inpath, std::string outpath);
        static Result encrypt(std::string inpath, std::string outpath, Platform platform);
    };
}
