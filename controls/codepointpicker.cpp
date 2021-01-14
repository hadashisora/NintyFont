// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

//Source file for CodePointPicker GUI control class
#include "codepointpicker.h"

namespace NintyFont::GUI::Controls
{
    CodePointPicker::CodePointPicker(std::vector<PropertyList::PropertyBase *> **t_propList, PropertyList::PropertyListEntryDescriptor *t_descriptor, UnicodeNames *t_unicode, MemberCallback *t_toggleCallback, QWidget *t_parent)
        : QFormLayout(t_parent)
        , Control()
    {
        unicode = t_unicode;
        propList = t_propList;
        descriptor = t_descriptor;
        toggleCallback = t_toggleCallback;

        QFont glyphFont;
        glyphFont.setPointSize(22);
        //QFont glyphNameFont;
        //glyphNameFont.setPointSizeF(glyphNameFont.pointSize() * 0.95);

        glyphLabel = new QLabel();
        glyphLabel->setFont(glyphFont);

        glyphNameLabel = new QLabel();
        //glyphNameLabel->setFont(glyphNameFont);
        glyphNameLabel->setWordWrap(true);

        codePointEdit = new QHexSpinBox();
        codePointEdit->setRange(descriptor->valueRange.first, descriptor->valueRange.second);
        connect(codePointEdit, QOverload<int32_t>::of(&QHexSpinBox::valueChanged), this, &CodePointPicker::updateProperty);

        addRow("Code point:", codePointEdit);
        addRow("Character:", glyphLabel);
        addRow(glyphNameLabel);

        lockControl();
    }

    void CodePointPicker::update()
    {
        if ((*propList) != nullptr)
        {
            lockControl();
            //This whole mess needs to be replaced with a text encoding converted from whatever encoding the font uses to utf-8
            uint16_t code; //ushort for now, but we'll have to somehow make this better later
            switch (descriptor->propType)
            {
                case PropertyList::PropertyType::Byte:
                {
                    PropertyList::Property<uint8_t> *vprop = (PropertyList::Property<uint8_t> *)(*propList)->at(descriptor->index);
                    code = vprop->value;
                    break;
                }
                case PropertyList::PropertyType::UInt16:
                {
                    PropertyList::Property<uint16_t> *vprop = (PropertyList::Property<uint16_t> *)(*propList)->at(descriptor->index);
                    code = vprop->value;
                    break;
                }
                case PropertyList::PropertyType::UInt32:
                    throw std::runtime_error("This code point type is not supported yet!");
                    break;
                case PropertyList::PropertyType::UInt64:
                    //This is probably never gonna be a thing since QSpinBox is limited to int32_t for it's value
                    throw std::runtime_error("This code point type is not supported yet!");
                    break;
                default:
                    throw std::runtime_error("Wrong code point type!");
                    break;
            }
            updateGlyphNameLabels(code);
            codePointEdit->setValue(code);
            unlockControl();
        }
        else
        {
            lockControl();
        }
    }

    void CodePointPicker::updateProperty()
    {
        switch (descriptor->propType)
        {
            case PropertyList::PropertyType::Byte:
            {
                PropertyList::Property<uint8_t> *vprop = (PropertyList::Property<uint8_t> *)(*propList)->at(descriptor->index);
                vprop->value = codePointEdit->value();
                break;
            }
            case PropertyList::PropertyType::UInt16:
            {
                PropertyList::Property<uint16_t> *vprop = (PropertyList::Property<uint16_t> *)(*propList)->at(descriptor->index);
                vprop->value = codePointEdit->value();
                break;
            }
            case PropertyList::PropertyType::UInt32:
            {
                PropertyList::Property<uint32_t> *vprop = (PropertyList::Property<uint32_t> *)(*propList)->at(descriptor->index);
                vprop->value = codePointEdit->value();
                break;
            }
            case PropertyList::PropertyType::UInt64:
            {
                PropertyList::Property<uint64_t> *vprop = (PropertyList::Property<uint64_t> *)(*propList)->at(descriptor->index);
                vprop->value = codePointEdit->value();
                break;
            }
            default:
                throw std::runtime_error("Wrong code point type!");
                break;
        }
        doMemberCallback(toggleCallback);
        updateGlyphNameLabels(codePointEdit->value());
    }

    void CodePointPicker::updateGlyphNameLabels(uint16_t code)
    {
        const uint16_t chr[] = {code, 0x0000};
        glyphLabel->setText(QString::fromUtf16(chr));
        glyphNameLabel->setText(QString::fromStdString(unicode->getCharNameFromUnicodeCodepoint(code)));
    }

    void CodePointPicker::lockControl()
    {
        codePointEdit->setEnabled(false);
        codePointEdit->blockSignals(true);
    }

    void CodePointPicker::unlockControl()
    {
        codePointEdit->setEnabled(true);
        codePointEdit->blockSignals(false);
    }

    CodePointPicker::~CodePointPicker()
    {
        delete glyphLabel;
        delete glyphNameLabel;
        delete codePointEdit;
    }
}
