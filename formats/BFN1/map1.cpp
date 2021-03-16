// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "map1.h"

namespace NintyFont::DOL::Format
{
    MAP1::MAP1(BinaryTools::BinaryReader *br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        mappingMethod = br->readUInt16();
        startCode = br->readUInt16();
        endCode = br->readUInt16();
        entriesOffset = br->getPosition();
    }

    void MAP1::validateSignature()
    {
        if (magic != 0x4D415031U) throw std::runtime_error("Invalid MAP1 signature!!!");
        else return;
    }

    const static uint16_t halftofull[95] = //This was originally a string, but since this file is in UTF-8, not Shift-JIS, we'll have to keep it as a ushort array
    {
        //　！”＃＄％＆’（）＊＋，－．／０１２３４５６７８９：；＜＝＞？＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ［￥］＾＿‘ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ｛｜｝～
        0x8140, 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8195, 0x8166, 0x8169, 0x816A, 0x8196, 0x817B, 0x8143, 0x817C, 0x8144, 0x815E,
        0x824F, 0x8250, 0x8251, 0x8252, 0x8253, 0x8254, 0x8255, 0x8256, 0x8257, 0x8258, 0x8146, 0x8147, 0x8183, 0x8181, 0x8184, 0x8148,
        0x8197, 0x8260, 0x8261, 0x8262, 0x8263, 0x8264, 0x8265, 0x8266, 0x8267, 0x8268, 0x8269, 0x826A, 0x826B, 0x826C, 0x826D, 0x826E,
        0x826F, 0x8270, 0x8271, 0x8272, 0x8273, 0x8274, 0x8275, 0x8276, 0x8277, 0x8278, 0x8279, 0x816D, 0x818F, 0x816E, 0x814F, 0x8151,
        0x8165, 0x8281, 0x8282, 0x8283, 0x8284, 0x8285, 0x8286, 0x8287, 0x8288, 0x8289, 0x828A, 0x828B, 0x828C, 0x828D, 0x828E, 0x828F,
        0x8290, 0x8291, 0x8292, 0x8293, 0x8294, 0x8295, 0x8296, 0x8297, 0x8298, 0x8299, 0x829A, 0x816F, 0x8162, 0x8170, 0x8160
    };

    uint16_t convertSjis(uint16_t param_1, uint16_t *param_2)
    {
        //Decompiled from WW
        uint16_t iVar2 = (param_1 & 0xffU) - 0x40;
        if (0x3f < iVar2) 
        {
            iVar2 = (param_1 & 0xffU) - 0x41;
        }
        uint16_t uVar1 = 0x31c;
        if (param_2 != nullptr)
        {
            uVar1 = *param_2;
        }
        return iVar2 + ((param_1 >> 8 & 0xff) - 0x88) * 0xbc + -0x5e + uVar1;
    }

    void MAP1::readEntries(BinaryTools::BinaryReader *br)//, bool convHalfToFull)
    {
#define h2fCheck(v) convHalfToFull && v >= 0x20 && v <= 0x7F
        entries = new std::vector<NTR::Format::CMAPEntry *>();
        if (entriesOffset == 0) return;
        br->setPosition(entriesOffset);
        uint16_t temp = 0U;
        switch (mappingMethod) 
        {
            case 0: //Direct
                //temp = br->readUInt16(); //indexOffset
                for (uint16_t i = startCode; i <= endCode; i++)
                {
                    uint16_t c = i;
                    //if (h2fCheck(c)) c = halftofull[i - 0x20];
                    entries->push_back(new NTR::Format::CMAPEntry(c, c - startCode));
                    temp++;
                }
                break;
            case 1: //IDK, probably broken, just based on WW decomp
            {
                uint16_t *thing = nullptr;
                uint16_t thing2;
                if (br->readUInt16() == 1) 
                {
                    thing2 = br->readUInt16();
                    thing = &thing2;
                }
                for (uint16_t i = startCode; i <= endCode; i++)
                {
                    uint16_t c = i;
                    //if (h2fCheck(c)) c = halftofull[i - 0x20];
                    entries->push_back(new NTR::Format::CMAPEntry(c, convertSjis(c, thing)));
                }
                break;
            }
            case 2: //Table
            temp = br->readUInt16();
                for (uint16_t i = startCode; i <= endCode; i++)
                {
                    temp = br->readUInt16(); //index
                    //if (temp != 0xFFFFU) 
                    uint16_t c = i;
                    //if (h2fCheck(c)) c = halftofull[i - 0x20];
                    entries->push_back(new NTR::Format::CMAPEntry(c, temp));
                }
                break;
            case 3: //Scan
                temp = br->readUInt16(); //count
                for (uint16_t i = 0; i < temp; i++)
                {
                    uint16_t c = br->readUInt16();
                    uint16_t idx = br->readUInt16();
                    //if (h2fCheck(c)) c = halftofull[c - 0x20];
                    entries->push_back(new NTR::Format::CMAPEntry(c, idx));
                }
                break;
        }
    }

    void MAP1::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {

    }

    MAP1::~MAP1()
    {
        
    }
}
