// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include <algorithm>
#include <vector>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <fmt/core.h>
#include <cstdio>

#include "binarycitrusfont.h"
#include "image/texturecodec.h"
#include "formats/CFNT/cfnt.h"
#include "formats/NFTR/finf.h"
#include "formats/RFNT/tglp.h"
#include "formats/NFTR/cglp.h"
#include "formats/NFTR/CWDH/cwdh.h"
#include "formats/NFTR/CMAP/cmap.h"
#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "formats/RFNT/rvlfontmacros.h"

namespace NintyFont::CTR
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
        //Shared with NTR
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_ENDIANNESS, "Endianness", PropertyList::PropertyType::Bool, PropertyList::ControlType::EndiannessPicker),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_CHARENC, "Char encoding", PropertyList::PropertyType::CharEncoding, PropertyList::ControlType::Label),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_LINEFEED, "Line feed", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)), //aka Leading
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_HEIGHT, "Height", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_WIDTH, "Width", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_ASCENT, "Ascent", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_BASELINE, "Baseline", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)), //aka Descending?
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_VERSION, "Version", PropertyList::PropertyType::UInt16, PropertyList::ControlType::Label, std::pair<int64_t, int64_t>(1, 0)), //Set range.first to 1 so this would show up as hex string
        //RVL-only
        new PropertyList::PropertyListEntryDescriptor(NITROFONT_PROP_RVL_IMAGEFORMAT, "Image encoding", PropertyList::PropertyType::ImageFormat, PropertyList::ControlType::Label)
    };

    std::vector<PropertyList::PropertyBase *> *drawableProperties = new std::vector<PropertyList::PropertyBase *>
    {
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(0U, "Draw Linefeed", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(1U, "Draw Ascent", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(2U, "Draw Baseline", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(3U, "Draw Char widths", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false)
    };

    BinaryCitrusFont::BinaryCitrusFont(std::string filePath)
        : FontBase()
    {
        //Various finds:
        //
        //Archived and resolved offset variations of this format also exist, like on CTR
        //RFNU - font with offsets resolved to be absolute in console memory
        //RFNA - archived font
        //
        //Purpose of HTGL is unknown, only it's magic signature is mentioned in the headers
        //
        //Font versions
        //Old version is 1.2 (1 major, 2 minor)
        //Normal version is 1.4 (1 major, 4 minor), as found in most files

        Format::CFNT *cfnt = nullptr;
        NTR::Format::FINF *finf = nullptr;
        RVL::Format::TGLP *tglp = nullptr;
        NTR::Format::CGLP *cglp = nullptr;
        std::vector<NTR::Format::CWDH *> cwdhHeaders = {};
        std::vector<NTR::Format::CharWidths *> charWidths = {};
        std::vector<NTR::Format::CMAP *> cmapHeaders = {};
        std::vector<NTR::Format::CMAPEntry *> charMaps = {};
        std::vector<QImage *> sheets = {};
        std::vector<QPixmap*> charImgs = {};

        //Open file outside the try-except so we can close it if an exception occurs
        BinaryTools::BinaryReader* br = new BinaryTools::BinaryReader(filePath, true);
        try
        {
            //Check and set correct endianness
            br->readEndiannessByte();

            //Prepare stuff for parsing
            br->setPosition(0x0);
            
            //Parsing metadata
            //Read CFNT header
            cfnt = new Format::CFNT(br);
            cfnt->validateSignature(); //This function call is essentially an internal check for both the file being correct and for code working correctly
            if (cfnt->version != 0x3000000) throw std::runtime_error(fmt::format("Unknown format version 0x{:X}", cfnt->version));
            //Read FINF header
            br->setPosition(cfnt->headerSize);
            finf = new NTR::Format::FINF(br);
            finf->validateSignature();
            //Read Glyph header
            br->setPosition(finf->ptrGlyph - 0x8U);
            if (finf->fontType == 0x0) //FONT_TYPE_GLYPH, use CGLP
            {
                //CGLP is a section that served the same purpose as TGLP on the NTR/DS.
                //All later font formats such as RFNT, CFNT and FFNT have this CGLP leftover
                //both in the font converter and the SDK headers...maybe they planned it but
                //it got replaced with TGLP and monochrome image formats it supports
                cglp = new NTR::Format::CGLP(br, true);
                cglp->validateSignature();
                //TODO: Find out if RVL supported CGLP fonts or not, if it did then implement it here
                throw std::runtime_error("You've found a CGLP RVL font! Aborting because IDK how to handle the file...");
            }
            else if (finf->fontType == 0x1)
            {
                tglp = new RVL::Format::TGLP(br);
                tglp->validateSignature();
            }
            else throw std::runtime_error(fmt::format("Unknown font type 0x{:X}}", finf->fontType));
            //Read and parse CWDH data
            uint32_t nextPtr = finf->ptrWidth - 0x8;;
            while (nextPtr != 0x0)
            {
                br->setPosition(nextPtr);
                NTR::Format::CWDH *cwdh = new NTR::Format::CWDH(br); //Read the header and the entries
                cwdh->validateSignature(); //Check if it's really CWDH
                cwdh->readEntries(br);
                charWidths.insert(charWidths.end(), cwdh->entries->begin(), cwdh->entries->end());
                nextPtr = cwdh->ptrNext; //Set the pointer to the next section
                cwdhHeaders.push_back(cwdh); //Append the header
            }
            //Read and parse CMAP data
            nextPtr = finf->ptrMap;
            while (nextPtr != 0x0)
            {
                br->setPosition(nextPtr - 0x8); //This is somewhat of a bodge, but it work nonetheless
                NTR::Format::CMAP *cmap = new NTR::Format::CMAP(br);
                cmap->validateSignature();
                cmap->readEntries(br);
                charMaps.insert(charMaps.end(), cmap->entries->begin(), cmap->entries->end());
                nextPtr = cmap->ptrNext;
                cmapHeaders.push_back(cmap); //Append the CMAP header to the list
            }

            //Decoding textures
            //Go to texture start offset
            br->setPosition(tglp->sheetPtr);
            //Read and decode sheets
            sheets.reserve(tglp->sheetCount);
            for (uint16_t i = 0; i < tglp->sheetCount; i++)
            {
                uint8_t *sheet_raw = Image::TextureCodec::decodeTexture(tglp->sheetFormat, br, tglp->sheetWidth, tglp->sheetHeight);
                QImage *sheet = new QImage(sheet_raw, tglp->sheetWidth, tglp->sheetHeight, QImage::Format_ARGB32);
                sheets.push_back(sheet);
                //sheet->save(QString::fromStdString(fmt::format("cfnt_dec_sheet_{}.png", i)), "PNG");
            }
            //Pull sheets apart into individual glyph images
            charImgs.reserve(charMaps.size());
            for (uint32_t i = 0; i < charMaps.size(); i++) //Get glyph count from charMaps, cause there's no better way to do that
            {
                //Do some math to figure out where we should start reading the pixels
                uint32_t currentSheet = i / (tglp->cellsPerRow * tglp->cellsPerColumn);
                uint32_t i2 = i - (currentSheet * tglp->cellsPerRow * tglp->cellsPerColumn);
                uint32_t currentRow = i2 / tglp->cellsPerRow;
                uint32_t currentColumn = i2 - (currentRow * tglp->cellsPerRow);
                uint32_t startX = currentColumn * (tglp->cellWidth + 1);
                uint32_t startY = currentRow * (tglp->cellHeight + 1);
                //Initialize the QPixmap
                QPixmap *charImg = new QPixmap();
                //Copy pixels
                charImg->convertFromImage(sheets.at(currentSheet)->copy(QRect(startX, startY, (tglp->cellWidth + 1), (tglp->cellHeight + 1))));
                //Append Bitmap to list
                charImgs.push_back(charImg);
            }

            //Sorting CMAP entries
            std::sort(charMaps.begin(), charMaps.end(), [] (NTR::Format::CMAPEntry *a, NTR::Format::CMAPEntry *b) -> bool
            {
                return a->index < b->index;
            });

            if (charMaps.size() != charWidths.size()) throw std::runtime_error("Mismatched number of charMaps and charWidths!");

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
                    throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Unknown BCFNT glyph encoding!", __FILE__, __LINE__));
            }
            (*glyphPropDescriptors)[NITROGLYPH_PROP_CODE]->valueRange = range;

            //Fill in some other stuff
            fontPath = filePath;
            cellSize = std::pair(tglp->cellWidth, tglp->cellHeight);
            sheetSize = std::pair(tglp->sheetWidth, tglp->sheetHeight);
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
                new PropertyList::Property<uint8_t>(fontPropDescriptors->at(NITROFONT_PROP_BASELINE), tglp->baselinePos),
                new PropertyList::Property<uint16_t>(fontPropDescriptors->at(NITROFONT_PROP_VERSION), cfnt->version),
                new PropertyList::Property<ImageFormats>(fontPropDescriptors->at(NITROFONT_PROP_RVL_IMAGEFORMAT), (ImageFormats)Image::TextureCodec::returnGeneralTextureType(tglp->sheetFormat))
            };
        }
        catch (std::runtime_error())
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
        delete cfnt;
        delete finf;
        delete tglp;
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
        for (auto i = sheets.begin(); i != sheets.end(); i++)
        {
            delete *i;
        }
        sheets.clear();
    }

    void BinaryCitrusFont::saveBinaryFont(std::string filePath)
    {
        //Format::setRvlFontImageFormat(fontProperties, (uint8_t)ImageFormats::RGB565); //Lazy bodge for testing

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

        recalculateSheetInfo(); //CalcTGLPInfo, CalcTextureSize

        //Stitch all glyph images into sheets for encoding
        uint32_t cellsPerRow = (uint32_t)(sheetSize.first / (cellSize.first + 1));
        uint32_t cellsPerColumn = (uint32_t)(sheetSize.second / (cellSize.second + 1));
        uint32_t cellsPerSheet = cellsPerRow * cellsPerColumn;
        uint32_t numSheets = (uint32_t)(std::ceil((double)encodedGlyphs.size() / (double)cellsPerSheet));
        std::vector<QImage *> sheetImgs(numSheets);
        for (auto c = encodedGlyphs.begin(); c != encodedGlyphs.end(); c++)
        {
            //Do some math to figure out where we should start plotting the pixels
            uint32_t i = NTR::Format::getGlyphIndex((*c)->props);
            uint32_t currentSheet = i / (cellsPerRow * cellsPerColumn);
            uint32_t i2 = i - (currentSheet * cellsPerRow * cellsPerColumn);
            uint32_t currentRow = i2 / cellsPerRow;
            uint32_t currentColumn = i2 - (currentRow * cellsPerRow);
            uint32_t startX = currentColumn * (cellSize.first + 1);
            uint32_t startY = currentRow * (cellSize.second + 1);
            //Check if the bitmap we're about to write to exists, and, if not, initialize it
            if (sheetImgs[currentSheet] == nullptr)
            {
                sheetImgs[currentSheet] = new QImage(sheetSize.first, sheetSize.second, QImage::Format_ARGB32);
                sheetImgs[currentSheet]->fill(Qt::transparent);
            }
            //Plot glyph image onto the sheet
            QPainter paint = QPainter(sheetImgs[currentSheet]);
            paint.drawPixmap(startX, startY, *(*c)->pixmap);
        }
        //Encode the images
        std::vector<std::vector<uint8_t> *> sheets(numSheets);
        for (uint32_t i = 0; i < sheetImgs.size(); i++)
        {
            //sheetImgs[i]->save(QString::fromStdString(fmt::format("enc_sheet_{}.png", i)), "PNG"); //Debug thingy to test the splicer output
            sheets[i] = Image::TextureCodec::encodeTexture(Image::TextureCodec::returnPlatformTextureType(RVL::Format::getRvlFontImageFormat(fontProperties)), sheetImgs[i]->bits(), sheetSize.first, sheetSize.second);
        }

        //Set up CWDH data
        std::vector<NTR::Format::CharWidths *> *widthEntries = NTR::Format::CharWidths::createWidthEntries(&encodedGlyphs);

        //Set up CMAP data
        //Generate Direct enties
        std::vector<std::pair<Glyph *, Glyph *>> *directEntries = NTR::Format::CMAP::createDirectEntries(&encodedGlyphs);
        //Generate Table entries
        std::vector<std::vector<NTR::Format::CMAPEntry *> *> *tableEntries = NTR::Format::CMAP::createTableEntries(&encodedGlyphs);
        //Generate Scan entries, basically everything that didn't go elsewhere goes here
        std::vector<std::vector<NTR::Format::CMAPEntry *> *> *scanEntries = NTR::Format::CMAP::createScanEntries(&encodedGlyphs); //This list is either gonna be of size 1 or empty

        BinaryTools::BlockLinker *linker = new BinaryTools::BlockLinker(); //Create a block linker, this will write all the pointers and lengths for us

        //Create blocks
        Format::CFNT cfnt = Format::CFNT();
        NTR::Format::FINF finf = NTR::Format::FINF(0x20, 0x1, NTR::Format::getFontLinefeed(fontProperties), 0U/*Hardcoded altCharIndex to 0*/,
                                                   widthEntries->at(0)->copy()/*Also hardcoded to those of the first glyph*/,
                                                   NTR::Format::getFontCharEncoding(fontProperties), NTR::Format::getFontHeight(fontProperties),
                                                   NTR::Format::getFontWidth(fontProperties), NTR::Format::getFontAscent(fontProperties), 0x464E4946U);
        RVL::Format::TGLP tglp = RVL::Format::TGLP(cellSize.first, cellSize.second, NTR::Format::getFontBaseline(fontProperties), maxCharWidth, sheets[0]->size(),
                sheets.size(), Image::TextureCodec::returnPlatformTextureType(RVL::Format::getRvlFontImageFormat(fontProperties)),
                cellsPerRow, cellsPerColumn, sheetSize.first, sheetSize.second, 0x504C4754U);

        std::vector<NTR::Format::CWDH *> cwdhHeaders = { new NTR::Format::CWDH(widthEntries, 0x48445743U) };
        std::vector<NTR::Format::CMAP *> cmapHeaders(0);
        for (auto entry = directEntries->begin(); entry != directEntries->end(); entry++)
        {
            cmapHeaders.push_back(new NTR::Format::CMAP(*entry, 0x50414D43U));
        }
        for (auto entry = tableEntries->begin(); entry != tableEntries->end(); entry++)
        {
            cmapHeaders.push_back(new NTR::Format::CMAP(0x1U, *entry, 0x50414D43U));
        }
        for (auto entry = scanEntries->begin(); entry != scanEntries->end(); entry++)
        {
            cmapHeaders.push_back(new NTR::Format::CMAP(0x2U, *entry, 0x50414D43U));
        }

        //Delete the old file
        std::remove(filePath.c_str()); //Lets hope this doesn't fail on Windows with non-ASCII characters
        //Create file and write data
        BinaryTools::BinaryWriter *bw = new BinaryTools::BinaryWriter(filePath, NTR::Format::getFontEndianness(fontProperties));
        cfnt.serialize(bw, linker);
        finf.serialize(bw, linker);
        tglp.serialize(bw, linker, &sheets, 0x80); //CTR aligns texture data to 0x80 bytes instead of 0x10 like the Wii does
        linker->addLookupValue("ptrWidth", bw->getPosition());
        for (auto entry = cwdhHeaders.begin(); entry != cwdhHeaders.end(); entry++)
        {
            (*entry)->serialize(bw, linker);
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
        for (auto entry = sheetImgs.begin(); entry != sheetImgs.end(); entry++)
        {
            delete *entry;
        }
        for (auto entry = sheets.begin(); entry != sheets.end(); entry++)
        {
            delete *entry;
        }
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

    //This function is from decompiled C# code, so it might not make a lot of
    //sense. I'll have to tidy it up later.
    void BinaryCitrusFont::recalculateSheetInfo(int32_t pixels)
    {
        int32_t num1 = (cellSize.first + 1) * (cellSize.second + 1) * glyphs.size();
        int32_t num2 = 0x7FFFFFFF; //Maximum value of int32_t
        for (int index1 = 32; index1 <= 1024; index1 *= 2)
        {
            int num3 = index1 - 2;
            if (num3 >= (int32_t)cellSize.first)
            {
                for (int index2 = 32; index2 <= 1024; index2 *= 2)
                {
                    if (pixels == 0 || index1 * index2 == pixels)
                    {
                        int num4 = index2 - 2;
                        if (num4 >= (int32_t)cellSize.second)
                        {
                            int divider = (num3 + 1) / (cellSize.first + 1) * ((num4 + 1) / (cellSize.second + 1));
                            int num5 = std::ceil(glyphs.size() / (double)divider);
                            int num6 = num3 * num4 * num5 - num1;
                            if (num6 < num2)
                            {
                                num2 = num6;
                                sheetSize.first = index1;
                                sheetSize.second = index2;
                            }
                        }
                    }
                }
            }
        }
        if (num2 == 0x7FFFFFFF) throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Invalid sheet pixels!", __FILE__, __LINE__));
    }

    std::vector<PropertyList::PropertyListEntryDescriptor *> *BinaryCitrusFont::getGlyphPropertyDescriptors()
    {
        return glyphPropDescriptors;
    }

    std::vector<PropertyList::PropertyBase *> *BinaryCitrusFont::getPropertiesList()
    {
        return fontProperties;
    }

    std::pair<uint32_t, uint32_t> BinaryCitrusFont::getGlyphImageSize()
    {
        return cellSize;
    }

    bool BinaryCitrusFont::canCreateCopyGlyphs()
    {
        return true;
    }

    bool BinaryCitrusFont::canDeleteGlyphs()
    {
        return true;
    }

    Glyph *BinaryCitrusFont::createEmptyGlyph()
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

    std::vector<PropertyList::PropertyBase *> *BinaryCitrusFont::getDrawablePropertiesList()
    {
        return drawableProperties;
    }

    void BinaryCitrusFont::drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns)
    {
        //Linefeed
        if (((PropertyList::Property<bool> *)(drawableProperties->at(0)))->value)
        {
            painter->setPen(QPen(QColor::fromRgb(255, 0, 0, 255), 2));
            for (uint32_t i = 0; i < rows; i++)
            {
                painter->drawLine(0,
                                  (i*cellSize.second) + NTR::Format::getFontLinefeed(fontProperties),
                                  cellSize.first*columns, (i*cellSize.second) + NTR::Format::getFontLinefeed(fontProperties));
            }
        }

        //Ascent
        if (((PropertyList::Property<bool> *)(drawableProperties->at(1)))->value)
        {
            painter->setPen(QPen(QColor::fromRgb(0, 255, 0, 255), 2));
            for (uint32_t i = 0; i < rows; i++)
            {
                painter->drawLine(0,
                                  ((i+1)*cellSize.second) - NTR::Format::getFontAscent(fontProperties),
                                  cellSize.first*columns, ((i+1)*cellSize.second) - NTR::Format::getFontAscent(fontProperties));
            }
        }

        //Baseline
        if (((PropertyList::Property<bool> *)(drawableProperties->at(2)))->value)
        {
            painter->setPen(QPen(QColor::fromRgb(0, 0, 255, 255), 2));
            for (uint32_t i = 0; i < rows; i++)
            {
                painter->drawLine(0,
                                  (i*cellSize.second) + NTR::Format::getFontBaseline(fontProperties),
                                  cellSize.first*columns, (i*cellSize.second) + NTR::Format::getFontBaseline(fontProperties));
            }
        }

        //Widths
        if (((PropertyList::Property<bool> *)(drawableProperties->at(3)))->value)
        {;
            for (uint32_t i = 0; i < glyphs.size(); i++)
            {
                uint32_t j = (uint32_t)(i % columns);
                uint32_t x1 = j * cellSize.first;
                uint32_t x2 = x1 + NTR::Format::getGlyphCharWidth(glyphs[i]->props) + 2;
                bool tooWide = x1 + cellSize.first < x2;

                painter->setPen(QPen(QColor::fromRgb(255, 255, 0, 255), 2));
                painter->drawLine(x1, ((int32_t)(i/columns) * cellSize.second) + 1, x1, ((int32_t)(i/columns + 1) * cellSize.second) - 1);
                if (tooWide) continue;
                painter->setPen(QPen(QColor::fromRgb(255, 255, 0, 127), 2));
                painter->drawLine(x2, ((int32_t)(i/columns) * cellSize.second) + 1, x2, ((int32_t)(i/columns + 1) * cellSize.second) - 1);
            }
        }
    }

    CharEncodings BinaryCitrusFont::getStdCharEncoding(void)
    {
        return (CharEncodings)NTR::Format::getFontCharEncoding(fontProperties);
    }

    bool BinaryCitrusFont::identifyFile(uint8_t *bytes)
    {
        uint8_t magicBE[] = {0x43, 0x46, 0x4E, 0x54}; //Although this says BE, it's LE since they though magic signatures shouldn't be backwards on LE systems
        uint8_t magicLE[] = {0x54, 0x4E, 0x46, 0x43};
        return BinaryTools::BinaryReader::bytearraycmp(magicBE, bytes, 4) || BinaryTools::BinaryReader::bytearraycmp(magicLE, bytes, 4);
    }

    std::string BinaryCitrusFont::returnFileTypeString()
    {
        return "NW4C font files (*.bcfnt *.cfnt)";
    }

    std::string BinaryCitrusFont::returnFileExtensionString()
    {
        return "*.bcfnt *.cfnt";
    }

    BinaryCitrusFont::~BinaryCitrusFont()
    {
        for (auto prop = fontProperties->begin(); prop != fontProperties->end(); prop++)
        {
            delete *prop;
        }
        delete fontProperties;
    }
}
