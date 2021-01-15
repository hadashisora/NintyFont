// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for CMAP class
// At the heart of the CMAP encoder is
// decompiled code from CTR fontcvtr
#include <fmt/core.h>

#include "cmap.h"
#include "formats/NFTR/ntrfontmacros.h"

namespace NintyFont::NTR::Format
{
    CMAP::CMAP(std::pair<Glyph *, Glyph *> t_entries, uint32_t t_magic)
    {
        magic = t_magic;
        length = 0x0U;
        codeBegin = getGlyphCode(t_entries.first->props);
        codeEnd = getGlyphCode(t_entries.second->props);
        mappingMethod = 0x0U; //Direct
        reserved = 0x0U;
        ptrNext = 0x0U;
        entries = new std::vector<CMAPEntry *>(1);
        (*entries)[0] = new CMAPEntry(codeBegin, getGlyphIndex(t_entries.first->props));
    }

    CMAP::CMAP(uint16_t t_mappingMethod, std::vector<CMAPEntry *> *t_entries, uint32_t t_magic)
    {
        magic = t_magic;
        length = 0x0U;
        mappingMethod = t_mappingMethod;
        reserved = 0x0U;
        ptrNext = 0x0U;
        entries = t_entries;
        switch (t_mappingMethod) 
        {
            case 0x1:
                codeBegin = (*t_entries->begin())->code;
                codeEnd = (*std::prev(t_entries->end()))->code;
                break;
            case 0x2:
                codeBegin = 0x0U;
                codeEnd = 0xFFFFU;
                break;
        }
    }

    CMAP::~CMAP()
    {
        for (auto entry = entries->begin(); entry != entries->end(); entry++)
        {
            delete *entry;
        }
        delete entries;
    }

    CMAP::CMAP(BinaryTools::BinaryReader* br)
    {
        magic = br->readUInt32();
        length = br->readUInt32();
        codeBegin = br->readUInt16();
        codeEnd = br->readUInt16();
        mappingMethod = br->readUInt16();
        reserved = br->readUInt16();
        ptrNext = br->readUInt32();
        entriesOffset = br->getPosition();
    }

    void CMAP::validateSignature()
    {
        if (magic != 0x434D4150U && magic != 0x50414D43) throw std::runtime_error("Invalid CMAP signature!!!");
        else return;
    }

    void CMAP::readEntries(BinaryTools::BinaryReader* br)
    {
        entries = new std::vector<CMAPEntry *>();
        if (entriesOffset == 0) return;
        br->setPosition(entriesOffset);
        uint16_t temp = 0U;
        switch (mappingMethod) 
        {
            case 0: //Direct
                temp = br->readUInt16(); //indexOffset
                for (uint16_t i = codeBegin; i <= codeEnd; i++)
                {
                    entries->push_back(new CMAPEntry(i, temp));
                    temp++;
                }
                break;
            case 1: //Table
                for (uint16_t i = codeBegin; i <= codeEnd; i++)
                {
                    temp = br->readUInt16(); //index
                    if (temp != 0xFFFFU) entries->push_back(new CMAPEntry(i, temp));
                }
                break;
            case 2: //Scan
                temp = br->readUInt16(); //count
                for (uint16_t i = 0; i < temp; i++) entries->push_back(new CMAPEntry(br));
                break;
        }
        entries->shrink_to_fit();
    }

    void CMAP::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        linker->incLookupValue("blockCount", 1);
        linker->incLookupValue("CMAP", 1);
        uint32_t startpos = bw->getPosition();
        uint32_t sectionnum = linker->getLookupValue("CMAP");
        linker->addLookupValue(fmt::format("CMAP{}", sectionnum), startpos + 0x8);
        bw->write(magic);
        linker->addPatchAddr(bw->getPosition(), fmt::format("CMAPLen{}", sectionnum));
        bw->write(length);
        bw->write(codeBegin);
        bw->write(codeEnd);
        bw->write(mappingMethod);
        bw->write(reserved);
        linker->addPatchAddr(bw->getPosition(), fmt::format("CMAP{}", sectionnum + 1));
        bw->write(ptrNext);
        switch (mappingMethod) 
        {
            case 0: //Direct
                bw->write((*entries)[0]->index);
                break;
            case 1: //Table
                for (auto entry = entries->begin(); entry != entries->end(); entry++)
                {
                    bw->write((*entry)->index);
                }
                break;
            case 2: //Scan
                bw->write((uint16_t)entries->size());
                for (auto entry = entries->begin(); entry != entries->end(); entry++)
                {
                    (*entry)->serialize(bw);
                }
                break;
        }
        //Padding
        uint8_t padBytes = 0x4U - (bw->getPosition() % 0x4U); //Yes I know this setup for calculating padding is a bodge...deal with it
        if (padBytes != 0x4U) for (uint8_t i = 0x0U; i < padBytes; i++) bw->write((uint8_t)0x0U);
        linker->addLookupValue(fmt::format("CMAPLen{}", sectionnum), bw->getPosition() - startpos);
    }

    //Forward-declare a few functions that are needed byt the code below, but have nothing to do in the header file
    Glyph *getGlyphByCodePoint(std::list<Glyph *> *glyphs, uint16_t codePoint);
    bool getNextStride(std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> *r, std::_List_iterator<Glyph *> last, std::_List_iterator<Glyph *> pos);
    bool getPrevStride(std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> *r, std::_List_iterator<Glyph *> begin, std::_List_iterator<Glyph *> pos);

    //NOTE: The code below is a simple translation of decompiled C# code to C++,
    //so the code might not make a lot of sense and can definitely be improved. 
    //Leaving it as is for now to insure accurate results when saving files.
    std::vector<std::pair<Glyph *, Glyph *>> *CMAP::createDirectEntries(std::list<Glyph *> *glyphs)
    {
        std::vector<std::pair<Glyph *, Glyph *>> *result = new std::vector<std::pair<Glyph *, Glyph *>>();
        std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> r;
        std::_List_iterator<Glyph *> pos = (*glyphs).begin();
    begin:
        if (pos == glyphs->end()) return result;
        while(getNextStride(&r, std::prev(glyphs->end()), pos))
        {
            pos = std::next(r.second);
            if (getGlyphCode((*r.second)->props) - getGlyphCode((*r.first)->props) + 1 >= 80)
            {
                result->push_back(std::pair<Glyph *, Glyph *>(*r.first, *r.second));
                glyphs->erase(r.first, std::next(r.second));
                goto begin;
            }
        }
        return result;
    }

    std::vector<std::vector<CMAPEntry *> *> *CMAP::createTableEntries(std::list<Glyph *> *glyphs)
    {
        std::vector<std::vector<CMAPEntry *> *> *result = new std::vector<std::vector<CMAPEntry *> *>();
        std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> r1;
        std::_List_iterator<Glyph *> pos = glyphs->begin();
        while (pos != glyphs->end())
        {
            getNextStride(&r1, std::prev(glyphs->end()), pos);
            if (r1.first == glyphs->end()) break;
            int32_t num1 = getGlyphCode((*r1.second)->props) - getGlyphCode((*r1.first)->props) + 1;
            int32_t num2 = 0;
            while (true)
            {
                std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> r2;
                std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> r3;
                float num3 = 0.0F;
                float num4 = 0.0F;
                if (r1.second != glyphs->end())
                {
                    std::_List_iterator<Glyph *> next = std::next(r1.second);
                    if (getNextStride(&r2, std::prev(glyphs->end()), next))
                    {
                        int32_t num5 = getGlyphCode((*r2.second)->props) - getGlyphCode((*r2.first)->props) + 1;
                        int32_t num6 = getGlyphCode((*r2.first)->props) - getGlyphCode((*r1.second)->props) - 1;
                        int32_t num7 = num1 + num5;
                        int32_t num8 = num7 + num2 + num6;
                        num3 = (float)num7 / (float)num8;
                    }
                }
                if (r1.first != glyphs->begin())
                {
                    std::_List_iterator<Glyph *> prev = std::prev(r1.first);
                    if (getPrevStride(&r3, glyphs->begin(), prev))
                    {
                        int32_t num5 = getGlyphCode((*r3.second)->props) - getGlyphCode((*r3.first)->props) + 1;
                        int32_t num6 = getGlyphCode((*r1.first)->props) - getGlyphCode((*r3.second)->props) - 1;
                        int32_t num7 = num1 + num5;
                        int32_t num8 = num7 + num2 + num6;
                        num4 = (float)num7 / (float)num8;
                    }
                }
                if (num3 > num4)
                {
                    if (num3 >= 0.5F)
                    {
                        int32_t num5 = getGlyphCode((*r2.second)->props) - getGlyphCode((*r2.first)->props) + 1;
                        int32_t num6 = getGlyphCode((*r2.first)->props) - getGlyphCode((*r1.second)->props) - 1;
                        num1 += num5;
                        num2 += num6;
                        r1.second = r2.second;
                    }
                    else break;
                }
                else if (num4 >= 0.5F)
                {
                    int32_t num5 = getGlyphCode((*r3.second)->props) - getGlyphCode((*r3.first)->props) + 1;
                    int32_t num6 = getGlyphCode((*r1.first)->props) - getGlyphCode((*r3.second)->props) - 1;
                    num1 += num5;
                    num2 += num6;
                    r1.first = r3.first;
                }
                else break;
            }
            if (num1 < 40)
            {
                pos = std::next(r1.second);
            }
            else
            {
                uint16_t code1 = getGlyphCode((*r1.first)->props);
                uint16_t code2 = getGlyphCode((*r1.second)->props);
                int32_t length = code2 - code1 + 1;
                std::vector<CMAPEntry *> *entries = new std::vector<CMAPEntry *>();
                Glyph *glyph;
                for (int32_t offset = 0; offset < length; ++offset)
                {
                    glyph = getGlyphByCodePoint(glyphs, code1 + offset);
                    if (glyph != nullptr)
                    {
                        entries->push_back(new CMAPEntry(getGlyphCode(glyph->props), getGlyphIndex(glyph->props)));
                    }
                    else entries->push_back(new CMAPEntry(code1 + offset, 0xFFFFU));
                }
                result->push_back(entries);
                pos = std::next(r1.second);
                glyphs->erase(r1.first, std::next(r1.second));
            }
        }
        return result;
    }

    bool getNextStride(std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> *r, std::_List_iterator<Glyph *> last, std::_List_iterator<Glyph *> pos)
    {
        if (pos == std::next(last)) return false;
        uint16_t code = getGlyphCode((*pos)->props) + 1U;
        uint16_t index = getGlyphIndex((*pos)->props) + 1U;
        std::_List_iterator<Glyph *> node = pos;
        for (auto next = std::next(node); next != std::next(last) && getGlyphCode((*next)->props) == code && getGlyphIndex((*next)->props) == index; ++index)
        {
            node = next;
            next = std::next(node);
            ++code;
        }
        r->first = pos;
        r->second = node;
        return true;
    }

    bool getPrevStride(std::pair<std::_List_iterator<Glyph *>, std::_List_iterator<Glyph *>> *r, std::_List_iterator<Glyph *> begin, std::_List_iterator<Glyph *> pos)
    {
        if (pos == begin) return false;
        std::_List_iterator<Glyph *> prev1 = std::prev(pos);
        if (prev1 == begin) return false;
        uint16_t code = getGlyphCode((*prev1)->props) - 1U;
        std::_List_iterator<Glyph *> prev2;
        for (prev2 = std::prev(prev1); prev2 != begin && getGlyphCode((*prev2)->props) == code; --code)
        {
            prev2 = std::prev(prev2);
        }
        r->first = std::next(prev2);
        r->second = prev1;
        return true;
    }

    Glyph *getGlyphByCodePoint(std::list<Glyph *> *glyphs, uint16_t codePoint)
    {
        Glyph *result = nullptr;
        for(auto i = glyphs->begin(); i != glyphs->end(); i++)
        {
            if (getGlyphCode((*i)->props) == codePoint)
            {
                result = *i;
                break;
            }
        }
        return result;
    }

    std::vector<std::vector<CMAPEntry *> *> *CMAP::createScanEntries(std::list<Glyph *> *glyphs)
    {
        //This is the only method to deviate from the fontcvtr implementation for the sake of simplicity
        std::vector<std::vector<CMAPEntry *> *> *result = new std::vector<std::vector<CMAPEntry *> *>();
        std::vector<CMAPEntry *> *entries = new std::vector<CMAPEntry *>();
        for (auto glyph = glyphs->begin(); glyph != glyphs->end(); glyph++)
        {
            entries->push_back(new CMAPEntry(getGlyphCode((*glyph)->props), getGlyphIndex((*glyph)->props)));
        }
        if (!entries->empty()) result->push_back(entries);
        else delete entries;
        return result;
    }
}
