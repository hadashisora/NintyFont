// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "formats.h"
#include "BFN1/jutresfont.h"
#include "NFTR/nitrofontresource.h"
#include "RFNT/binaryrevolutionfont.h"
#include "PM4F/pocketmonstersgen4font.h"
#include "CFNT/binarycitrusfont.h"

FontFormatDescriptor fontDescriptors[NUM_FORMATS] //The descriptors should really be moved to the font format headers sometime in the future
{
    {&openFontFileTemplate<NintyFont::DOL::JUTResFont>, &NintyFont::DOL::JUTResFont::identifyFile, &NintyFont::DOL::JUTResFont::returnFileExtensionString, &NintyFont::DOL::JUTResFont::returnFileTypeString},
    {&openFontFileTemplate<NintyFont::NTR::NitroFontResource>, &NintyFont::NTR::NitroFontResource::identifyFile, &NintyFont::NTR::NitroFontResource::returnFileExtensionString, &NintyFont::NTR::NitroFontResource::returnFileTypeString},
    {&openFontFileTemplate<NintyFont::RVL::BinaryRevolutionFont>, &NintyFont::RVL::BinaryRevolutionFont::identifyFile, &NintyFont::RVL::BinaryRevolutionFont::returnFileExtensionString, &NintyFont::RVL::BinaryRevolutionFont::returnFileTypeString},
    {&openFontFileTemplate<NintyFont::NTR::PM4::PocketMonstersGen4Font>, &NintyFont::NTR::PM4::PocketMonstersGen4Font::identifyFile, &NintyFont::NTR::PM4::PocketMonstersGen4Font::returnFileExtensionString, &NintyFont::NTR::PM4::PocketMonstersGen4Font::returnFileTypeString},
    {&openFontFileTemplate<NintyFont::CTR::BinaryCitrusFont>, &NintyFont::CTR::BinaryCitrusFont::identifyFile, &NintyFont::CTR::BinaryCitrusFont::returnFileExtensionString, &NintyFont::CTR::BinaryCitrusFont::returnFileTypeString}
};

// NintyFont::FontBase *(* const fontConstructors[NUM_FORMATS])(std::string)
// {
//     &openFontFileTemplate<NintyFont::DOL::JUTResFont>,
//     &openFontFileTemplate<NintyFont::NTR::NitroFontResource>,
//     &openFontFileTemplate<NintyFont::RVL::BinaryRevolutionFont>,
//     &openFontFileTemplate<NintyFont::NTR::PM4::PocketMonstersGen4Font>
// };

// bool (* const fontIndetificators[NUM_FORMATS])(uint8_t *)
// {
//     &NintyFont::DOL::JUTResFont::identifyFile,
//     &NintyFont::NTR::NitroFontResource::identifyFile,
//     &NintyFont::RVL::BinaryRevolutionFont::identifyFile,
//     &NintyFont::NTR::PM4::PocketMonstersGen4Font::identifyFile
// };

// std::string (* const fontExtensions[NUM_FORMATS])(void)
// {
//     &NintyFont::DOL::JUTResFont::returnFileExtensionString,
//     &NintyFont::NTR::NitroFontResource::returnFileExtensionString,
//     &NintyFont::RVL::BinaryRevolutionFont::returnFileExtensionString,
//     &NintyFont::NTR::PM4::PocketMonstersGen4Font::returnFileExtensionString
// };

// std::string (* const fontFileTypes[NUM_FORMATS])(void)
// {
//     &NintyFont::DOL::JUTResFont::returnFileTypeString,
//     &NintyFont::NTR::NitroFontResource::returnFileTypeString,
//     &NintyFont::RVL::BinaryRevolutionFont::returnFileTypeString,
//     &NintyFont::NTR::PM4::PocketMonstersGen4Font::returnFileTypeString
// };
