// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source for Label GUI control class
#include "label.h"

#include <fmt/core.h>

namespace NintyFont::GUI::Controls
{
    Label::Label(std::vector<PropertyList::PropertyBase *> **t_propList, PropertyList::PropertyListEntryDescriptor *t_descriptor, GlobalStuffs *t_globals, bool t_isHex, QWidget *t_parent)
        : QLabel(t_parent)
        , Control()
    {
        globals = t_globals;
        propList = t_propList;
        descriptor = t_descriptor;
        isHex = t_isHex;
    }

    void Label::update()
    {
        if ((*propList) != nullptr)
        {
            std::string text;
            switch (descriptor->propType)
            {
                case PropertyList::PropertyType::Byte:
                {
                    PropertyList::Property<uint8_t> *vprop = (PropertyList::Property<uint8_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::SByte:
                {
                    PropertyList::Property<int8_t> *vprop = (PropertyList::Property<int8_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::UInt16:
                {
                    PropertyList::Property<uint16_t> *vprop = (PropertyList::Property<uint16_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Int16:
                {
                    PropertyList::Property<int16_t> *vprop = (PropertyList::Property<int16_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::UInt32:
                {
                    PropertyList::Property<uint32_t> *vprop = (PropertyList::Property<uint32_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Int32:
                {
                    PropertyList::Property<int32_t> *vprop = (PropertyList::Property<int32_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::UInt64:
                {
                    PropertyList::Property<uint64_t> *vprop = (PropertyList::Property<uint64_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Int64:
                {
                    PropertyList::Property<int64_t> *vprop = (PropertyList::Property<int64_t> *)(*propList)->at(descriptor->index);
                    if (isHex) text = fmt::format("0x{:X}", vprop->value);
                    else text = fmt::format("{}", vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Bool:
                {
                    PropertyList::Property<bool> *vprop = (PropertyList::Property<bool> *)(*propList)->at(descriptor->index);
                    if (vprop->value) text = "True";
                    else text = "False";
                    break;
                }
                case PropertyList::PropertyType::ImageFormat:
                {
                    PropertyList::Property<ImageFormats> *vprop = (PropertyList::Property<ImageFormats> *)(*propList)->at(descriptor->index);
                    text = globals->font->getImageFormatString(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::CharEncoding:
                {
                    PropertyList::Property<CharEncodings> *vprop = (PropertyList::Property<CharEncodings> *)(*propList)->at(descriptor->index);
                    text = globals->font->getCharEncodingString(vprop->value);
                    break;
                }
                default:
                    throw std::runtime_error("Wrong code point type!");
                    break;
            }
            setText(QString::fromStdString(text));
        }
    }

    void Label::lockControl()
    {

    }

    void Label::unlockControl()
    {

    }

    Label::~Label()
    {

    }
}
