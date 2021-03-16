// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "jutresfont.h"
#include "image/texturecodec.h"
#include "fontbfn1.h"
#include "inf1.h"
#include "gly1.h"
#include "wid1.h"
#include "map1.h"
#include "formats/NFTR/CWDH/charwidths.h"
#include "formats/NFTR/CMAP/cmapentry.h"

#include <algorithm>
#include <vector>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <fmt/core.h>

namespace NintyFont::DOL
{
    std::vector<PropertyList::PropertyListEntryDescriptor *> *glyphPropDescriptors = new std::vector<PropertyList::PropertyListEntryDescriptor *>
    {
        new PropertyList::PropertyListEntryDescriptor(0U, "Index", PropertyList::PropertyType::UInt16, PropertyList::ControlType::None),
        new PropertyList::PropertyListEntryDescriptor(1U, "Code point", PropertyList::PropertyType::UInt16, PropertyList::ControlType::CodePointPicker),
        new PropertyList::PropertyListEntryDescriptor(2U, "Left", PropertyList::PropertyType::SByte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(-0x7F, 0x7F)),
        new PropertyList::PropertyListEntryDescriptor(3U, "Glyph width", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF)),
    };

    std::vector<PropertyList::PropertyListEntryDescriptor *> *fontPropDescriptors = new std::vector<PropertyList::PropertyListEntryDescriptor *>
    {
        new PropertyList::PropertyListEntryDescriptor(0U, "Char encoding", PropertyList::PropertyType::CharEncoding, PropertyList::ControlType::Label),
        new PropertyList::PropertyListEntryDescriptor(1U, "Ascent", PropertyList::PropertyType::UInt16, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFFFF)),
        new PropertyList::PropertyListEntryDescriptor(2U, "Descent", PropertyList::PropertyType::UInt16, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFFFF)),
        new PropertyList::PropertyListEntryDescriptor(3U, "Width", PropertyList::PropertyType::UInt16, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFFFF)),
        new PropertyList::PropertyListEntryDescriptor(4U, "Leading", PropertyList::PropertyType::UInt16, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFFFF)),
        new PropertyList::PropertyListEntryDescriptor(5U, "Fallback code", PropertyList::PropertyType::UInt16, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFFFF)),
        new PropertyList::PropertyListEntryDescriptor(6U, "Image encoding", PropertyList::PropertyType::ImageFormat, PropertyList::ControlType::Label)
    };

    std::vector<PropertyList::PropertyBase *> *drawableProperties = new std::vector<PropertyList::PropertyBase *>
    {
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(0U, "Draw Ascent", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(1U, "Draw Descent", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(2U, "Draw Leading", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false),
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(3U, "Draw Char widths", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false)
    };

    JUTResFont::JUTResFont(std::string filePath)
        : FontBase()
    {
        //FONTbfn1 - stands for Binary FoNt 1.
        //Part of the J-prefix library that is essentially the internal
        //predaccessor to the NintendoWare library.
        //Internal name is JUTResFont (JUtility Resource Font).
        //This code mainly uses hand-decompiled code from Zelda WW as reference.

        //Open file outside the try-except so we can close it if an exception occurs
        BinaryTools::BinaryReader* br = new BinaryTools::BinaryReader(filePath, false); //Open in BE by default, since there's no endianness flag for this format

        Format::FONTbfn1 *bfn1 = nullptr;
        Format::INF1 *inf1 = nullptr;
        //Format::GLY1 *gly1 = nullptr;
        std::vector<Format::GLY1 *> gly1Headers = {};
        std::vector<Format::WID1 *> wid1Headers = {};
        std::vector<Format::WID1Entry *> charWidths = {};
        std::vector<Format::MAP1 *> map1Headers = {};
        std::vector<NTR::Format::CMAPEntry *> charMaps = {};

        try
        {
            //Prepare stuff for parsing
            br->setPosition(0x0);

            //Parsing metadata
            //Read FONTbfn1 header
            bfn1 = new Format::FONTbfn1(br);
            bfn1->validateSignature(); //This function call is essentially an internal check for both the file being correct and for code working correctly

            //The format does not have pointers within itself, so we have to run a loop
            //through all the blocks, just like the actual game code does.
            //We don't do magic validation for these, since that's kinda already done
            //by the switching code.
            for (uint i = 0; i < bfn1->dataBlocks; i++)
            {
                uint32_t magic = br->peekUInt32();
                uint32_t pos = br->getPosition();
                switch (magic) //This mimmics the order in which the game code checks for these
                {
                    case 0x4D415031: //MAP1
                    {
                        Format::MAP1 *map1 = new Format::MAP1(br);
                        //bool halfToFull = false;
                        //if (inf1 != nullptr) halfToFull = (inf1->fontType == 2);
                        map1->readEntries(br);//, halfToFull);
                        charMaps.insert(charMaps.end(), map1->entries->begin(), map1->entries->end());
                        map1Headers.push_back(map1);
                        br->setPosition(map1->length + pos);
                        break;
                    }
                    case 0x57494431: //WID1
                    {
                        Format::WID1 *wid1 = new Format::WID1(br);
                        wid1->readEntries(br);
                        charWidths.insert(charWidths.end(), wid1->entries->begin(), wid1->entries->end());
                        wid1Headers.push_back(wid1);
                        br->setPosition(wid1->length + pos);
                        break;
                    }
                    case 0x494E4631: //INF1
                    {
                        inf1 = new Format::INF1(br);
                        br->setPosition(inf1->length + pos);
                        break;
                    }
                    case 0x474C5931: //GLY1
                    {
                        Format::GLY1 *gly1 = new Format::GLY1(br);
                        gly1Headers.push_back(gly1);
                        br->setPosition(gly1->length + pos);
                        break;
                    }
                    default:
                        throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Unknown FONTbfn1 block 0x{:04X} at file offset 0x{:04X}!", __FILE__, __LINE__, magic, br->getPosition()));
                        break;
                }
            }

            //Decoding textures
            //Read and decode sheets
            std::vector<QImage *> sheets(gly1Headers.size());
            std::vector<QPixmap*> charImgs(charMaps.size());
            uint16_t counter = 0;
            for (uint16_t i = 0; i < gly1Headers.size(); i++)
            {
                Format::GLY1 *hdr = gly1Headers.at(i);
                //Go to texture start offset
                br->setPosition(hdr->sheetOffset);
                uint8_t *sheet_raw = RVL::Image::TextureCodec::decodeTexture(hdr->sheetFormat, br, hdr->sheetWidth, hdr->sheetHeight);
                QImage *sheet = new QImage(sheet_raw, hdr->sheetWidth, hdr->sheetHeight, QImage::Format_ARGB32);
                sheets[i] = sheet;
                sheet->save(QString::fromStdString(fmt::format("bfn1_dec_sheet_{}.png", i)), "PNG");
                //sheets[i].Save(File.Create("tex_" + i + ".png"), System.Drawing.Imaging.ImageFormat.Png); //Debug thingy to test the decoder output

                //Pull the sheet apart into individual glyph images
                //by looping through all glyph cells.
                for (uint32_t y = 0; y < hdr->cellsPerColumn; y++)
                {
                    for (uint32_t x = 0; x < hdr->cellsPerRow; x++)
                    {
                        uint32_t startX = x * (hdr->cellWidth);
                        uint32_t startY = y * (hdr->cellHeight);
                        //Initialize the QPixmap
                        QPixmap *charImg = new QPixmap();
                        //Copy pixels
                        charImg->convertFromImage(sheet->copy(QRect(startX, startY, hdr->cellWidth, hdr->cellHeight)));
                        charImg->save(QString::fromStdString(fmt::format("bfn1_dec_cell_{}_{}x{}.png", i, x, y)), "PNG");
                        //Append Bitmap to list
                        if (counter < charImgs.size()) charImgs[counter++] = charImg;
                    }
                }
            }

            //Sorting MAP1 entries
            std::sort(charMaps.begin(), charMaps.end(), [] (NTR::Format::CMAPEntry *a, NTR::Format::CMAPEntry *b) -> bool
            {
                return a->index < b->index;
            });

            if (charMaps.size() != charWidths.size()) throw std::runtime_error("Mismatched number of charMaps and charWidths!"); //Don't check against charImgs, cause it's prone to having more because of the way things are encoded

            //Put all the data into Glyph
            glyphs = std::vector<Glyph *>(charMaps.size());
            for (uint32_t i = 0; i < charMaps.size(); i++)
            {
                std::vector<PropertyList::PropertyBase *> *props = new std::vector<PropertyList::PropertyBase *>
                {
                        new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[0U], charMaps[i]->index),
                        new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[1U], charMaps[i]->code),
                        new PropertyList::Property<int8_t>((*glyphPropDescriptors)[2U], charWidths[i]->w1),
                        new PropertyList::Property<uint8_t>((*glyphPropDescriptors)[3U], charWidths[i]->w2),
                };
                glyphs[i] = new Glyph(props, charImgs[i]);
            }

            //Set the glyph code point bounds
            std::pair<int64_t, int64_t> range;
            switch(inf1->fontType)
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
                    throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Unknown JUTResFont glyph encoding!", __FILE__, __LINE__));
            }
            (*glyphPropDescriptors)[1U]->valueRange = range;

            //Fill in some other stuff
            //endianness = br->isLittleEndian;
            fontPath = filePath;
            //platform = (uint8_t)Platforms::Revolution;
            //charEncoding = finf->encoding;
            //imageFormat = Image::TextureCodec::returnGeneralTextureType(tglp->sheetFormat);
            cellSize = std::pair(gly1Headers.at(0U)->cellWidth, gly1Headers.at(0U)->cellHeight);
            sheetSize = std::pair(gly1Headers.at(0U)->sheetWidth, gly1Headers.at(0U)->sheetHeight);
            //lineFeed = finf->lineFeed;
            //defaultCharIndex = finf->defaultCharIndex;
            //defaultWidth = finf->defaultWidths;
            //height = finf->height;
            //width = finf->width;
            //ascent = finf->ascent;
            //baselinePos = tglp->baselinePos;
            //maxCharWidth = tglp->maxCharWidth;
            //version = rfnt->version;
            edited = false;

            //Fill in the property list
            fontProperties = new std::vector<PropertyList::PropertyBase *>
            {
                    new PropertyList::Property<CharEncodings>(fontPropDescriptors->at(0U), (CharEncodings)inf1->fontType),
                    new PropertyList::Property<uint16_t>(fontPropDescriptors->at(1U), inf1->ascent),
                    new PropertyList::Property<uint16_t>(fontPropDescriptors->at(2U), inf1->descent),
                    new PropertyList::Property<uint16_t>(fontPropDescriptors->at(3U), inf1->width),
                    new PropertyList::Property<uint16_t>(fontPropDescriptors->at(4U), inf1->leading),
                    new PropertyList::Property<uint16_t>(fontPropDescriptors->at(5U), inf1->defaultCharCode),
                    new PropertyList::Property<ImageFormats>(fontPropDescriptors->at(6U), (ImageFormats)RVL::Image::TextureCodec::returnGeneralTextureType(gly1Headers.at(0U)->sheetFormat))
            };

            
        }
        catch (std::runtime_error())
        {
            glyphs = std::vector<Glyph *>(0);
        }

        //Clean up the mess
        delete br;
        delete bfn1;
        delete inf1;
        for (auto thing = gly1Headers.begin(); thing != gly1Headers.end(); thing++)
        {
            delete *thing;
        }
        for (auto thing = wid1Headers.begin(); thing != wid1Headers.end(); thing++)
        {
            delete *thing;
        }
        for (auto thing = map1Headers.begin(); thing != map1Headers.end(); thing++)
        {
            delete *thing;
        }
        //Width and map entries don't need to be deleted here, since dtors for their respective headers delete the entries already
    }

    void JUTResFont::saveBinaryFont(std::string filePath)
    {

    }

    std::vector<PropertyList::PropertyListEntryDescriptor *> *JUTResFont::getGlyphPropertyDescriptors()
    {
        return glyphPropDescriptors;
    }

    std::vector<PropertyList::PropertyBase *> *JUTResFont::getPropertiesList()
    {
        return fontProperties;
    }

    std::pair<uint32_t, uint32_t> JUTResFont::getGlyphImageSize()
    {
        return cellSize;
    }

    bool JUTResFont::canCreateCopyGlyphs()
    {
        return true;
    }

    bool JUTResFont::canDeleteGlyphs()
    {
        return true;
    }

    Glyph *JUTResFont::createEmptyGlyph(void)
    {
        return nullptr;
    }

    std::vector<PropertyList::PropertyBase *> *JUTResFont::getDrawablePropertiesList(void)
    {
        return drawableProperties;
    }

    void JUTResFont::drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns)
    {

    }

    CharEncodings JUTResFont::getStdCharEncoding(void)
    {
        return ((PropertyList::Property<CharEncodings> *)(fontProperties->at(0)))->value;
    }

    bool JUTResFont::identifyFile(uint8_t *bytes)
    {
        uint8_t magic[] = {0x46, 0x4F, 0x4E, 0x54, 0x62, 0x66, 0x6E, 0x31};
        return BinaryTools::BinaryReader::bytearraycmp(magic, bytes, 8);
    }

    std::string JUTResFont::returnFileTypeString()
    {
        return "JUTResFont (*.bfn)";
    }

    std::string JUTResFont::returnFileExtensionString()
    {
        return "*.bfn";
    }

    JUTResFont::~JUTResFont()
    {

    }
}
