// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for PocketMonstersGen4Font
// font format class
#include "pocketmonstersgen4font.h"
#include "pmheader.h"
#include "binarytools/binaryreader.h"
#include "binarytools/binarywriter.h"
#include "binarytools/blocklinker.h"
#include "binarytools/bitreader.h"

namespace NintyFont::NTR::Image
{
    extern uint8_t flipBitOrder(uint8_t bits, uint8_t bpp); //"extern" our way to laziness
}

namespace NintyFont::NTR::PM4
{
    std::vector<PropertyList::PropertyListEntryDescriptor *> *glyphPropDescriptors = new std::vector<PropertyList::PropertyListEntryDescriptor *>
    {
        new PropertyList::PropertyListEntryDescriptor(0U, "Width", PropertyList::PropertyType::Byte, PropertyList::ControlType::SpinBox, std::pair<int64_t, int64_t>(0x0, 0xFF))
    };

    std::vector<PropertyList::PropertyListEntryDescriptor *> *fontPropDescriptors = new std::vector<PropertyList::PropertyListEntryDescriptor *> { };

    std::vector<PropertyList::PropertyBase *> *drawableProperties = new std::vector<PropertyList::PropertyBase *>
    {
        new PropertyList::Property<bool>(new PropertyList::PropertyListEntryDescriptor(0U, "Draw Widths", PropertyList::PropertyType::Bool, PropertyList::ControlType::BoolPicker), false)
    };

    std::vector<PropertyList::PropertyBase *> *fontProperties = new std::vector<PropertyList::PropertyBase *> { };

    PocketMonstersGen4Font::PocketMonstersGen4Font(std::string filePath)
        : FontBase()
    {
        //Open file outside the try-except so we can close it if an exception occurs
        BinaryTools::BinaryReader *br = new BinaryTools::BinaryReader(filePath, true);
        try
        {
            //Prepare stuff for parsing
            Format::PMHeader *header = nullptr;
            std::vector<QPixmap *> charImgs;
            std::vector<uint8_t> charWidths;

            //Parsing metadata
            //Read file header
            header = new Format::PMHeader(br);
            //Read widths
            br->setPosition(header->ptrWidths);
            for (uint32_t i = 0U; i < header->glyphCount; i++)
            {
                charWidths.push_back(br->readByte());
            }

            //Reading and decoding images
            br->setPosition(header->ptrImage);
            for (uint32_t i = 0U; i < header->glyphCount; i++)
            {
                QPixmap *charImg = new QPixmap();
                uint16_t *pixelData = (uint16_t *)br->readBytes(64U); //Hardcoded value
                uint8_t *argbBuf = new uint8_t[16U * 16U * 4U];
                uint8_t *grayscaleBuf = new uint8_t[16U * 16U];
                for (uint32_t y = 0U; y < 16U; y++) //This is also hardcoded
                {
                    for (uint32_t x = 0U; x < 16U; x++) //So is this
                    {
                        uint32_t index = (x / 8 + (y / 8) * 2) * 8 + y % 8;
                        uint32_t shift = (x % 8) * 2;
                        uint8_t value = Image::flipBitOrder((pixelData[index] >> (14 - shift)) & 0x0003, 2) * 0x55;
                        grayscaleBuf[y * 16U + x] = value;
                        argbBuf[(((y * 16U) + x) * 4) + 0] = value;
                        argbBuf[(((y * 16U) + x) * 4) + 1] = value;
                        argbBuf[(((y * 16U) + x) * 4) + 2] = value;
                        argbBuf[(((y * 16U) + x) * 4) + 3] = 0xFFU;
                    }
                }
                charImg->convertFromImage(QImage(argbBuf, 16U, 16U,  QImage::Format_ARGB32));
                charImgs.push_back(charImg);
            }

            //Put all the data into Glyph
            glyphs = std::vector<Glyph *>(header->glyphCount);
            for (uint32_t i = 0; i < header->glyphCount; i++)
            {
                std::vector<PropertyList::PropertyBase *> *props = new std::vector<PropertyList::PropertyBase *>
                {
                    new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[0U], charWidths[i])
                };
                glyphs[i] = new Glyph(props, charImgs[i]);
            }

            //Fill in some other stuff
            fontPath = filePath;
            edited = false;

            //Clean up the mess
            br->close();
            delete header;
        }
        catch (std::runtime_error())
        {
            glyphs = std::vector<Glyph *>(0);
        }

        delete br;
    }

    void PocketMonstersGen4Font::saveBinaryFont(std::string filePath)
    {
        //Prepare stuff for saving
        //Calculate max width and max height AND encode the images, all in one loop :)
        uint8_t maxWidth = 0U;
        uint8_t maxHeight = 0U;
        uint16_t *encodedImage = new uint16_t[32U * glyphs.size()];
        for (uint32_t i = 0; i < 32U * glyphs.size(); i++) encodedImage[i] = 0x0U;
        for (uint32_t i = 0; i < glyphs.size(); i++)
        {
            QImage thing = glyphs.at(i)->pixmap->toImage();
            uint8_t *imageData = thing.bits();
            uint32_t imgAddr = i * 32U;
            uint8_t heightcount = 0U;
            for (uint32_t y = 0U; y < 16U; y++)
            {
                uint8_t widthcount = 0U;
                for (uint32_t x = 0U; x < 16U; x++)
                {
                    uint32_t index = imgAddr + ((x / 8 + (y / 8) * 2) * 8 + y % 8);
                    uint32_t shift = (x % 8) * 2;

                    uint8_t blue = imageData[(((y * 16U) + x) * 4) + 0];
                    uint8_t green = imageData[(((y * 16U) + x) * 4) + 1];
                    uint8_t red = imageData[(((y * 16U) + x) * 4) + 2];
                    uint8_t alpha = imageData[(((y * 16U) + x) * 4) + 3];
                    double floatpixel = (((red + green + blue) / 3.0) * (alpha / 255.0)) / 85.0;
                    uint8_t value = Image::flipBitOrder((uint8_t)std::round(floatpixel), 2);
                    if (value > 0x0U) widthcount++;

                    encodedImage[index] &= ~(0xC000 >> shift);
                    encodedImage[index] |= value << (14 - shift);
                }
                if (widthcount > 0x0U) heightcount++;
                if (widthcount > maxWidth) maxWidth = widthcount;
            }
            if (heightcount > maxHeight) maxHeight = heightcount;
        }
        //Create the header
        Format::PMHeader header = Format::PMHeader(glyphs.size(), maxHeight, maxWidth);

        //Writing!
        //Create file and write data
        BinaryTools::BinaryWriter *bw = new BinaryTools::BinaryWriter(filePath, true);
        BinaryTools::BlockLinker *linker = new BinaryTools::BlockLinker(); //Create a block linker, this will write all the pointers and lengths for us
        //Write header
        header.serialize(bw, linker);
        //Write image
        linker->addLookupValue("ptrImage", bw->getPosition());
        for (uint32_t i = 0; i < 32U * glyphs.size(); i++)
        {
            bw->write(encodedImage[i]);
        }
        //Write widths
        linker->addLookupValue("ptrWidths", bw->getPosition());
        for (uint32_t i = 0; i < glyphs.size(); i++)
        {
            bw->write(((PropertyList::Property<uint8_t> *)glyphs.at(i)->props->at(0))->value);
        }
        linker->makeBlockLink(bw);

        delete bw;
        delete linker;
        delete[] encodedImage;
    }

    std::vector<PropertyList::PropertyListEntryDescriptor *> *PocketMonstersGen4Font::getGlyphPropertyDescriptors()
    {
        return glyphPropDescriptors;
    }

    std::vector<PropertyList::PropertyBase *> *PocketMonstersGen4Font::getPropertiesList()
    {
        return fontProperties;
    }

    std::pair<uint32_t, uint32_t> PocketMonstersGen4Font::getGlyphImageSize()
    {
        return std::pair<uint32_t, uint32_t>(16U, 16U);
    }

    bool PocketMonstersGen4Font::canCreateCopyGlyphs()
    {
        return true;
    }

    bool PocketMonstersGen4Font::canDeleteGlyphs()
    {
        return true;
    }

    Glyph *PocketMonstersGen4Font::createEmptyGlyph()
    {
        std::vector<PropertyList::PropertyBase *> *props = new std::vector<PropertyList::PropertyBase *>
        {
            new PropertyList::Property<uint16_t>((*glyphPropDescriptors)[0U], 0U)
        };
        Glyph *newglyph = new Glyph(props, new QPixmap(16U, 16U));
        newglyph->pixmap->fill(Qt::transparent);
        return newglyph;
    }

    std::vector<PropertyList::PropertyBase *> *PocketMonstersGen4Font::getDrawablePropertiesList()
    {
        return drawableProperties;
    }

    void PocketMonstersGen4Font::drawDrawableProperties(QPainter *painter, uint32_t rows, uint32_t columns)
    {

    }

    bool PocketMonstersGen4Font::identifyFile(uint8_t *bytes)
    {
        uint8_t magic[] = {0x10, 0x00, 0x00, 0x00};
        return BinaryTools::BinaryReader::bytearraycmp(magic, bytes, 4);
    }

    std::string PocketMonstersGen4Font::returnFileTypeString()
    {
        return "Pocket Monsters gen 4 font (*.bin *.dat *.pm4f)";
    }

    std::string PocketMonstersGen4Font::returnFileExtensionString()
    {
        return "*.bin *.dat *.pm4f";
    }

    PocketMonstersGen4Font::~PocketMonstersGen4Font()
    {
        
    }
}
