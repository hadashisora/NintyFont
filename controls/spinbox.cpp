// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

//Source code for SpinBox GUI control class
#include "spinbox.h"

namespace NintyFont::GUI::Controls
{
    SpinBox::SpinBox(std::vector<PropertyList::PropertyBase *> **t_propList, PropertyList::PropertyListEntryDescriptor *t_descriptor, MemberCallback *t_toggleCallback, QWidget *t_parent)
        : QSpinBox(t_parent)
        , Control()
    {
        propList = t_propList;
        descriptor = t_descriptor;
        toggleCallback = t_toggleCallback;

        connect(this, QOverload<int32_t>::of(&QSpinBox::valueChanged), this, &SpinBox::updateProperty);
        //QSpinBox::connect(this, &SpinBox::updateProperty);

        setRange(descriptor->valueRange.first, descriptor->valueRange.second);

        lockControl();
    }

    void SpinBox::update()
    {
        if ((*propList) != nullptr)
        {
            lockControl();
            switch (descriptor->propType)
            {
                case PropertyList::PropertyType::Byte:
                {
                    //qDebug() << QString::fromStdString(descriptor->name) << ": " << propList[descriptor->index];
                    PropertyList::Property<uint8_t> *vprop = (PropertyList::Property<uint8_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::SByte:
                {
                    PropertyList::Property<int8_t> *vprop = (PropertyList::Property<int8_t> *)(*propList)->at(descriptor->index);
                    setValue((int32_t)vprop->value);
                    break;
                }
                case PropertyList::PropertyType::UInt16:
                {
                    PropertyList::Property<uint16_t> *vprop = (PropertyList::Property<uint16_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Int16:
                {
                    PropertyList::Property<int16_t> *vprop = (PropertyList::Property<int16_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::UInt32:
                {
                    PropertyList::Property<uint32_t> *vprop = (PropertyList::Property<uint32_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Int32:
                {
                    PropertyList::Property<int32_t> *vprop = (PropertyList::Property<int32_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::UInt64:
                {
                    PropertyList::Property<uint64_t> *vprop = (PropertyList::Property<uint64_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                case PropertyList::PropertyType::Int64:
                {
                    PropertyList::Property<int64_t> *vprop = (PropertyList::Property<int64_t> *)(*propList)->at(descriptor->index);
                    setValue(vprop->value);
                    break;
                }
                default:
                    throw std::runtime_error("Wrong code point type!");
                    break;
            }
            unlockControl();
        }
        else
        {
            lockControl();
        }
    }

    void SpinBox::lockControl()
    {
        setEnabled(false);
        blockSignals(true);
    }

    void SpinBox::unlockControl()
    {
        setEnabled(true);
        blockSignals(false);
    }

    void SpinBox::updateProperty()
    {
        if ((*propList) == nullptr) return;

        switch (descriptor->propType)
        {
            case PropertyList::PropertyType::Byte:
            {
                PropertyList::Property<uint8_t> *vprop = (PropertyList::Property<uint8_t> *)(*propList)->at(descriptor->index);
                vprop->value = (uint8_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::SByte:
            {
                PropertyList::Property<int8_t> *vprop = (PropertyList::Property<int8_t> *)(*propList)->at(descriptor->index);
                vprop->value = (int8_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::UInt16:
            {
                PropertyList::Property<uint16_t> *vprop = (PropertyList::Property<uint16_t> *)(*propList)->at(descriptor->index);
                vprop->value = (uint16_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::Int16:
            {
                PropertyList::Property<int16_t> *vprop = (PropertyList::Property<int16_t> *)(*propList)->at(descriptor->index);
                vprop->value = (int16_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::UInt32:
            {
                PropertyList::Property<uint32_t> *vprop = (PropertyList::Property<uint32_t> *)(*propList)->at(descriptor->index);
                vprop->value = (uint32_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::Int32:
            {
                PropertyList::Property<int32_t> *vprop = (PropertyList::Property<int32_t> *)(*propList)->at(descriptor->index);
                vprop->value = (int32_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::UInt64:
            {
                PropertyList::Property<uint64_t> *vprop = (PropertyList::Property<uint64_t> *)(*propList)->at(descriptor->index);
                vprop->value = (uint64_t)QSpinBox::value();
                break;
            }
            case PropertyList::PropertyType::Int64:
            {
                PropertyList::Property<int64_t> *vprop = (PropertyList::Property<int64_t> *)(*propList)->at(descriptor->index);
                vprop->value = (int64_t)QSpinBox::value();
                break;
            }
            default:
                throw std::runtime_error("Wrong code point type!");
                break;
        }
        doMemberCallback(toggleCallback);
    }

    SpinBox::~SpinBox()
    {
        
    }
}
