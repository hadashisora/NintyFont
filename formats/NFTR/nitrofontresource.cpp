// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for NitroFontResource
// font format class
#include "nitrofontresource.h"
#include "nftr.h"
#include "finf.h"
#include "cglp.h"
#include "CWDH/cwdh.h"
#include "CMAP/cmap.h"
#include "image/texturecodec.h"
#include "binarytools/bitreader.h"
#include "ntrfontmacros.h"

#include <algorithm>
#include <vector>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <fmt/core.h>
#include <cstdio>

namespace NintyFont::NTR
{
    std::vector<PropertyList::PropertyListEntryDescriptor *> *glyphPropDescriptors = new std::vector<PropertyList::PropertyListEntryDescriptor *>
    {
        new PropertyList::PropertyListEntryDescriptor(NITROGLYPH_PROP_INDEX, "Index", PropertyList::PropertyType::UInt16, PropertyList::ControlType::None),
        new PropertyList::PropertyListEntryDescriptor(NITROGLYPH_PROP_CODE, "Code point", PropertyList::PropertyType::UInt16, PropertyList::ControlType::CodePointPicker),
        new PropertyList::PropertyListEntryDescriptor(NITROGLYPH_PROP_LEFT, "Left", PropertyList::PropertyType::SByte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(-0x7F, 0x7F)),
        new PropertyList::PropertyListEntryDescriptor(NITROGLYPH_PROP_GLYPHWIDTH, "Glyph width", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROGLYPH_PROP_CHARWIDTH, "Char width", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF))
    };

    std::vector<PropertyList::PropertyListEntryDescriptor *> *fontPropDescriptors = new std::vector<PropertyList::PropertyListEntryDescriptor *>
    {
        //Shared with the derived formats
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_ENDIANNESS, "Endianness", PropertyList::PropertyType::Bool, PropertyList::ControlType::EndiannessPicker),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_CHARENC, "Char encoding", PropertyList::PropertyType::CharEncoding, PropertyList::ControlType::Label),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_LINEFEED, "Line feed", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)), //aka Leading
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_HEIGHT, "Height", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_WIDTH, "Width", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_ASCENT, "Ascent", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_BASELINE, "Baseline", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)), //aka Descending?
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_VERSION, "Version", PropertyList::PropertyType::UInt16, PropertyList::ControlType::Label, std::pair<int64_t, int64_t>(1, 0)), //Set range.first to 1 so this would show up as hex string
        //NTR-only
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_NTR_BPP, "Image bit depth", PropertyList::PropertyType::Byte, PropertyList::ControlType::Label),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_NTR_VERTICAL, "Vertical", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_NTR_ROTATION, "Rotation", PropertyList::PropertyType::Byte, PropertyList::ControlType::Label)
    };

    std::vector<PropertyList::PropertyBase *> *drawableProperties = new std::vector<PropertyList::PropertyBase *>
    {
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(0U, "Draw Linefeed", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(1U, "Draw Ascent", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(2U, "Draw Baseline", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(3U, "Draw Char widths", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false)
    };

    bool isValidBlockOffset(uint32_t offset, uint32_t blockMagic, uint32_t fileLen, BinaryTools::BinaryReader *br);

    NitroFontResource::NitroFontResource(std::string filePath)
            : FontBase()
    {
        Format::NFTR *nftr = nullptr;
        Format::FINF *finf = nullptr;;
        Format::CGLP *cglp = nullptr;
        std::vector<Format::CWDH *> cwdhHeaders = {};
        std::vector<Format::CharWidths *> charWidths = {};
        std::vector<Format::CMAP *> cmapHeaders = {};
        std::vector<Format::CMAPEntry *> charMaps = {};
        std::vector<QPixmap *> charImgs = {};

        //Open file outside the try-except so we can close it if an exception occurs
        BinaryTools::BinaryReader* br = new BinaryTools::BinaryReader(filePath, true);
        try
        {
            //Check and set correct endianness
            br->readEndiannessByte();
            size_t trueFileSize = br->getTrueFileSize(); //This has to exist thanks to GameFreak

            //Prepare stuff for parsing
            br->setPosition(0x0);
            

            //Parsing metadata
            //Read NFTR header
            nftr = new Format::NFTR(br);
            nftr->validateSignature(); //This function call is essentially an internal check for both the file being correct and for code working correctly
            //if (nftr->version != 0x104) throw std::runtime_error(fmt::format("Unknown format version 0x{:X}", nftr->version));
            //Read FINF header
            br->setPosition(nftr->ptrInfo);
            finf = new Format::FINF(br);
            finf->validateSignature();
            //Read Glyph header
            br->setPosition(finf->ptrGlyph - 0x8U);
            if (finf->fontType == 0x0) //FONT_TYPE_GLYPH, use CGLP
            {
                cglp = new Format::CGLP(br, nftr->version > 0x1000);
                cglp->validateSignature();
            }
            else if (finf->fontType == 0x1)
            {
                throw std::runtime_error("You've found a TGLP NTR font! Aborting because IDK how to handle the file...");
            }
            else throw std::runtime_error(fmt::format("Unknown font type 0x{:X}}", finf->fontType));
            //Read and parse CWDH data
            uint32_t nextPtr = finf->ptrWidth - 0x8;;
            while (isValidBlockOffset(nextPtr, 0x43574448U, trueFileSize, br))
            {
                NTR::Format::CWDH *cwdh = new NTR::Format::CWDH(br); //Read the header and the entries
                cwdhHeaders.push_back(cwdh); //Append the header
                cwdh->validateSignature(); //Check if it's really CWDH
                cwdh->readEntries(br);
                charWidths.insert(charWidths.end(), cwdh->entries->begin(), cwdh->entries->end());
                nextPtr = cwdh->ptrNext - 0x8; //Set the pointer to the next section
            }
            //Read and parse CMAP data
            nextPtr = finf->ptrMap - 0x8;
            while (isValidBlockOffset(nextPtr, 0x434D4150U, trueFileSize, br))
            {
                NTR::Format::CMAP *cmap = new NTR::Format::CMAP(br);
                cmapHeaders.push_back(cmap); //Append the CMAP header to the list
                cmap->validateSignature();
                cmap->readEntries(br);
                charMaps.insert(charMaps.end(), cmap->entries->begin(), cmap->entries->end());
                nextPtr = cmap->ptrNext - 0x8;
            }

            //Decoding images
            //Check for the font being from PM BW/B2W2, cause those have a messed up format and need additional measures
            bool isPocketMonstersFont = false;
            if (cglp->cellSize != ceil((double)cglp->cellWidth * (double)cglp->cellHeight / (double)(8.0 / (double)cglp->bpp))) isPocketMonstersFont = true;
            //Read the bitmap thingy and pull it apart into glyph images
            //std::vector<Format::CharWidths *> charWidths2 = {}; //For BW/B2W2
            if (isPocketMonstersFont)
            {
                for (auto thing = cwdhHeaders.begin(); thing != cwdhHeaders.end(); thing++)
                {
                    delete *thing;
                    *thing = nullptr;
                }
                charWidths.clear();
                cwdhHeaders.clear();
            }
            charMaps.reserve(charMaps.size());
            BinaryTools::BitReader *bitr = new BinaryTools::BitReader(br);
            for (uint32_t i = 0; i < charMaps.size(); i++) //Get glyph count from charMaps, cause there's no better way to do that
            {
                auto thing = cglp->bitmapPtr + (cglp->cellSize * i);
                br->setPosition(cglp->bitmapPtr + (cglp->cellSize * i));
                if (isPocketMonstersFont) charWidths.push_back(new Format::CharWidths(br));
                bitr->update();
                QPixmap *charImg = new QPixmap();
                charImgs.push_back(charImg);
                charImg->convertFromImage(QImage(Image::TextureCodec::decodeBitmap(cglp->bpp, bitr, cglp->cellWidth, cglp->cellHeight), cglp->cellWidth, cglp->cellHeight, QImage::Format_ARGB32));
                //charImg->save(QString::fromStdString(fmt::format("dec_nftr_bitmap_{}.png", i)), "PNG");
            }
            delete bitr;

            //Sorting CMAP entries
            std::sort(charMaps.begin(), charMaps.end(), [] (NTR::Format::CMAPEntry *a, NTR::Format::CMAPEntry *b) -> bool
            {
                return a->index < b->index;
            });

            // Merge any entries which define a glyph with the same index, since apparently some fonts have duped entries across different CMAPs
            for (auto e = charMaps.begin(); e != std::prev(charMaps.end()); e++)
            {
                for (auto e2 = std::next(e); e2 != charMaps.end();)
                {
                    if ((*e)->index == (*e2)->index)
                    {
                        if ((*e)->code != (*e2)->code) throw std::runtime_error("Error: same glyph redefined with different code point!!");

                        e2 = charMaps.erase(e2);
                        continue;
                    }
                    e2++;
                }
            }

            if (charMaps.size() != charWidths.size()) //throw std::runtime_error("Mismatched number of charMaps and charWidths!");
            {
                while (charMaps.size() != charWidths.size())
                {
                    charWidths.push_back(finf->defaultWidths->copy());
                }
            }

            //Put all the data into Glyph
            glyphs = std::vector<Glyph *>(charMaps.size());
            for (uint32_t i = 0; i < charMaps.size(); i++)
            {
                std::vector<PropertyList::PropertyBase *> *props = new std::vector<PropertyList::PropertyBase *>
                {
                    new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_INDEX], 0U),
                    new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_CODE], charMaps[i]->code),
                    new PropertyList::Property<int8_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_LEFT], charWidths[i]->left),
                    new PropertyList::Property<uint8_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_GLYPHWIDTH], charWidths[i]->glyphWidth),
                    new PropertyList::Property<uint8_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_CHARWIDTH], charWidths[i]->charWidth)
                };
                glyphs[i] = new Glyph(props, charImgs[i]);
            }

            //Set the glyph code point bounds
            std::pair<int64_t, int64_t> range;
            switch(finf->encoding)
            {
                case (uint8_t)CharEncodings::Num:
                case (uint8_t)CharEncodings::UTF16:
                case (uint8_t)CharEncodings::ShiftJIS:
                    range = std::pair<int64_t, int64_t>(0x0, 0xFFFE);
                    break;
                case (uint8_t)CharEncodings::UTF8:
                case (uint8_t)CharEncodings::CP1252:
                    range = std::pair<int64_t, int64_t>(0x0, 0xFF);
                    break;
                default:
                    throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Unknown NFTR glyph encoding!", __FILE__, __LINE__));
            }
            (*glyphPropDescriptors)[NITROGLYPH_PROP_CODE]->valueRange = range;

            //Fill in some other stuff
            fontPath = filePath;
            cellSize = std::pair(cglp->cellWidth, cglp->cellHeight);
            edited = false;

            //Fill in the property list
            fontProperties = new std::vector<PropertyList::PropertyBase *>
            {
                new PropertyList::Property<bool>(fontPropDescriptors->at(NITROFONT_PROP_ENDIANNESS), br->isLittleEndian()),
                new PropertyList::Property<CharEncodings>(fontPropDescriptors->at(NITROFONT_PROP_CHARENC), (CharEncodings)finf->encoding),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_LINEFEED), finf->lineFeed),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_HEIGHT), finf->height),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_WIDTH), finf->width),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_ASCENT), finf->ascent),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_BASELINE), cglp->baselinePos),
                new PropertyList::Property<uint16_t>(fontPropDescriptors->at(NITROFONT_PROP_VERSION), nftr->version),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_NTR_BPP), cglp->bpp),
                new PropertyList::Property<bool>(fontPropDescriptors->at(NITROFONT_PROP_NTR_VERTICAL), cglp->flags),
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_NTR_ROTATION), cglp->flags)
            };

            if (nftr->version <= 0x1000)
            {
                fontPropDescriptors->at(NITROFONT_PROP_HEIGHT)->ctrlType = PropertyList::ControlType::None;
                fontPropDescriptors->at(NITROFONT_PROP_WIDTH)->ctrlType = PropertyList::ControlType::None;
                fontPropDescriptors->at(NITROFONT_PROP_ASCENT)->ctrlType = PropertyList::ControlType::None;
            }

            if (isPocketMonstersFont)
            {
                fontPropDescriptors->push_back(new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_NTR_GAMEFREAK, "PM BW/B2W2 font", PropertyList::PropertyType::Bool, PropertyList::ControlType::None));
                fontProperties->push_back(new PropertyList::Property<bool>(fontPropDescriptors->at(NITROFONT_PROP_NTR_GAMEFREAK), true));
            }
        }
        catch (std::runtime_error e)
        {
            for (auto i = charImgs.begin(); i != charImgs.end(); i++)
            {
                delete *i;
            }
            for (auto i = glyphs.begin(); i != glyphs.end(); i++)
            {
                (*i)->pixmap = nullptr;
                delete *i;
            }
            glyphs = std::vector<Glyph *>(0);
        }

        //Clean up the mess
        delete br;
        delete nftr;
        delete finf;
        delete cglp;
        //Width and map entries don't need to be deleted here, since dtors for their respective headers delete the entries already
        for (auto thing = cwdhHeaders.begin(); thing != cwdhHeaders.end(); thing++)
        {
            delete *thing;
        }
        for (auto thing = cmapHeaders.begin(); thing != cmapHeaders.end(); thing++)
        {
            delete *thing;
        }
    }

    inline bool getGamefreak(std::vector<PropertyList::PropertyBase *> *props)
    {
        return ((PropertyList::Property<bool> *)(props->at(NITROFONT_PROP_NTR_GAMEFREAK)))->value;
    }

    inline void setGamefreak(std::vector<PropertyList::PropertyBase *> *props, bool val)
    {
        ((PropertyList::Property<bool> *)(props->at(NITROFONT_PROP_NTR_GAMEFREAK)))->value = val;
    }

    void NitroFontResource::saveBinaryFont(std::string filePath)
    {
        //Prepare stuff for exporting
        std::list<Glyph *> encodedGlyphs(glyphs.begin(), glyphs.end()); //We need a std::list for the CMAP creation functions to work properly
        //Sort glyphs by code point
        encodedGlyphs.sort([](Glyph *a, Glyph *b) {
            return (NTR::Format::getGlyphCode(a->props) < NTR::Format::getGlyphCode(b->props));
        });

        //Set glyph indexes, while also looking for the widest glyph
        uint8_t maxCharWidth = 0;
        uint16_t index = 0;
        for (auto i = encodedGlyphs.begin(); i != encodedGlyphs.end(); i++)
        {
            NTR::Format::setGlyphIndex((*i)->props, index);
            uint8_t temp = NTR::Format::getGlyphCharWidth((*i)->props);
            if (temp > maxCharWidth) maxCharWidth = temp;
            index++;
        }

        bool isGF = false; 
        try { getGamefreak(fontProperties); }
        catch (std::out_of_range) { }

        size_t cellBytes = ceil((double)(cellSize.first * cellSize.second * Format::getNtrFontBpp(fontProperties))/8.0);
        if (isGF) cellBytes += 3;

        //Set up CWDH data
        std::vector<NTR::Format::CharWidths *> *widthEntries = NTR::Format::CharWidths::createWidthEntries(&encodedGlyphs);

        //Encode the images
        std::vector<uint8_t *> *sheets = new std::vector<uint8_t *>();
        for (auto i = encodedGlyphs.begin(); i != encodedGlyphs.end(); i++)
        {
            uint8_t *cell = new uint8_t[cellBytes];
            for (uint16_t c = 0; c < cellBytes; c++)
            {
                cell[c] = 0x0; //Too lazy to memfil
            }
            sheets->push_back(cell);
            if (!isGF) 
            {
                Image::TextureCodec::encodeBitmap((*i)->pixmap->toImage().bits(), cell, Format::getNtrFontBpp(fontProperties), cellSize.first, cellSize.second);
            }
            else
            {
                *cell = Format::getGlyphLeft((*i)->props);
                *(cell + 1) = Format::getGlyphGlyphWidth((*i)->props);
                *(cell + 2) = Format::getGlyphCharWidth((*i)->props);
                Image::TextureCodec::encodeBitmap((*i)->pixmap->toImage().bits(), cell + 3, Format::getNtrFontBpp(fontProperties), cellSize.first, cellSize.second);
            }
        }

        //Set up CMAP data
        //Generate Direct enties
        std::vector<std::pair<Glyph *, Glyph *>> *directEntries = NTR::Format::CMAP::createDirectEntries(&encodedGlyphs);
        //Generate Table entries
        std::vector<std::vector<NTR::Format::CMAPEntry *> *> *tableEntries = NTR::Format::CMAP::createTableEntries(&encodedGlyphs);
        //Generate Scan entries, basically everything that didn't go elsewhere goes here
        std::vector<std::vector<NTR::Format::CMAPEntry *> *> *scanEntries = NTR::Format::CMAP::createScanEntries(&encodedGlyphs); //This list is either gonna be of size 1 or empty

        BinaryTools::BlockLinker *linker = new BinaryTools::BlockLinker(); //Create a block linker, this will write all the pointers and lengths for us

        //Create blocks
        //Format::RFNT rfnt = Format::RFNT(NTR::Format::getFontVersion(fontProperties));
        Format::NFTR nftr = Format::NFTR(NTR::Format::getFontVersion(fontProperties));
        size_t finfSize = 0;
        if (!(NTR::Format::getFontWidth(fontProperties) == 0xFF && NTR::Format::getFontHeight(fontProperties) == 0xFF && NTR::Format::getFontAscent(fontProperties) == 0xFF))
            finfSize = 0x20;
        else finfSize = 0x1C;
        NTR::Format::FINF finf = NTR::Format::FINF(finfSize, 0x0, NTR::Format::getFontLinefeed(fontProperties), 0U/*Hardcoded altCharIndex to 0*/,
                                                   widthEntries->at(0)->copy()/*Also hardcoded to those of the first glyph*/,
                                                   NTR::Format::getFontCharEncoding(fontProperties), NTR::Format::getFontHeight(fontProperties),
                                                   NTR::Format::getFontWidth(fontProperties), NTR::Format::getFontAscent(fontProperties));
        
        Format::CGLP cglp = Format::CGLP(cellSize.first, cellSize.second, cellBytes, Format::getFontBaseline(fontProperties), maxCharWidth, Format::getNtrFontBpp(fontProperties), Format::getNtrFontRotation(fontProperties));

        std::vector<NTR::Format::CWDH *> cwdhHeaders = { new NTR::Format::CWDH(widthEntries) };
        std::vector<NTR::Format::CMAP *> cmapHeaders(0);
        for (auto entry = directEntries->begin(); entry != directEntries->end(); entry++)
        {
            cmapHeaders.push_back(new NTR::Format::CMAP((*entry)));
        }
        for (auto entry = tableEntries->begin(); entry != tableEntries->end(); entry++)
        {
            cmapHeaders.push_back(new NTR::Format::CMAP(0x1U, *entry));
        }
        for (auto entry = scanEntries->begin(); entry != scanEntries->end(); entry++)
        {
            cmapHeaders.push_back(new NTR::Format::CMAP(0x2U, *entry));
        }

        //Delete the old file
        std::remove(filePath.c_str()); //Lets hope this doesn't fail on Windows with non-ASCII characters
        //Create file and write data
        BinaryTools::BinaryWriter *bw = new BinaryTools::BinaryWriter(filePath, NTR::Format::getFontEndianness(fontProperties));
        nftr.serialize(bw, linker);
        finf.serialize(bw, linker);
        cglp.serialize(bw, linker, sheets);
        linker->addLookupValue("ptrWidth", bw->getPosition());
        if (1)
        {
            for (auto entry = cwdhHeaders.begin(); entry != cwdhHeaders.end(); entry++)
            {
                (*entry)->serialize(bw, linker);
            }
        }
        else
        {
            linker->addLookupValue("ptrWidth", 0x10);
        }
        linker->addLookupValue("ptrMap", bw->getPosition() + 0x8U);
        for (auto entry = cmapHeaders.begin(); entry != cmapHeaders.end(); entry++)
        {
            (*entry)->serialize(bw, linker);
        }
        linker->addLookupValue("fileSize", bw->getPosition());
        linker->makeBlockLink(bw);

        //Clean up after ourselves
        delete bw;
        delete linker;
        for (auto entry = sheets->begin(); entry != sheets->end(); entry++)
        {
            delete *entry;
        }
        delete sheets;
        for (auto entry = cwdhHeaders.begin(); entry != cwdhHeaders.end(); entry++)
        {
            delete *entry;
        }
        for (auto entry = cmapHeaders.begin(); entry != cmapHeaders.end(); entry++)
        {
            delete *entry;
        }
        delete directEntries;
        delete tableEntries;
        delete scanEntries;

        return;
    }

    std::vector<PropertyList::PropertyListEntryDescriptor *> *NitroFontResource::getGlyphPropertyDescriptors()
    {
        return glyphPropDescriptors;
    }

    std::vector<PropertyList::PropertyBase *> *NitroFontResource::getPropertiesList()
    {
        return fontProperties;
    }

    std::pair<uint32_t, uint32_t> NitroFontResource::getGlyphImageSize()
    {
        return cellSize;
    }

    bool NitroFontResource::canCreateCopyGlyphs()
    {
        return true;
    }

    bool NitroFontResource::canDeleteGlyphs()
    {
        return true;
    }

    bool isValidBlockOffset(uint32_t offset, uint32_t blockMagic, uint32_t fileLen, BinaryTools::BinaryReader *br)
    {
        //This mess of a function is needed in case the user load a PM font, cause GF messed with the format a bit
        if (offset == 0) return false;
        if (offset > fileLen - 4U) return false;
        br->setPosition(offset);
        auto thing = br->peekUInt32();
        if (thing != blockMagic) return false;
        return true;
    }

    Glyph *NitroFontResource::createEmptyGlyph()
    {
        std::vector<PropertyList::PropertyBase *> *props = new std::vector<PropertyList::PropertyBase *>
        {
            new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_INDEX], 0U),
            new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_CODE], 0U),
            new PropertyList::Property<int8_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_LEFT], 0),
            new PropertyList::Property<uint8_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_GLYPHWIDTH], 0U),
            new PropertyList::Property<uint8_t>((*glyphPropDescriptors)[NITROGLYPH_PROP_CHARWIDTH], 0U)
        };
        Glyph *newglyph = new Glyph(props, new QPixmap(cellSize.first, cellSize.second));
        newglyph->pixmap->fill(Qt::transparent);
        return newglyph;
    }

    std::vector<PropertyList::PropertyBase *> *NitroFontResource::getDrawablePropertiesList()
    {
        return drawableProperties;
    }

    void NitroFontResource::drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns)
    {

    }

    CharEncodings NitroFontResource::getStdCharEncoding(void)
    {
        return (CharEncodings)NTR::Format::getFontCharEncoding(fontProperties);
    }

    bool NitroFontResource::identifyFile(uint8_t *bytes)
    {
        uint8_t magicBE[] = {0x4E, 0x46, 0x54, 0x52}; //In case this somehow exists. Technically it's possible, practically - I dunno
        uint8_t magicLE[] = {0x52, 0x54, 0x46, 0x4E};
        return BinaryTools::BinaryReader::bytearraycmp(magicBE, bytes, 4) || BinaryTools::BinaryReader::bytearraycmp(magicLE, bytes, 4);
    }

    std::string NitroFontResource::returnFileTypeString()
    {
        return "NITRO SDK font (*.nftr *.rtfn)";
    }

    std::string NitroFontResource::returnFileExtensionString()
    {
        return "*.nftr *.rtfn";
    }

    NitroFontResource::~NitroFontResource()
    {
        for (auto prop = fontProperties->begin(); prop != fontProperties->end(); prop++)
        {
            delete *prop;
        }
        delete fontProperties;
        //These are just in case we opened a PM version of the font and need to fix things up
        fontPropDescriptors->at(NITROFONT_PROP_HEIGHT)->ctrlType = PropertyList::ControlType::SpinBox;
        fontPropDescriptors->at(NITROFONT_PROP_WIDTH)->ctrlType = PropertyList::ControlType::SpinBox;
        fontPropDescriptors->at(NITROFONT_PROP_ASCENT)->ctrlType = PropertyList::ControlType::SpinBox;
    }
}
