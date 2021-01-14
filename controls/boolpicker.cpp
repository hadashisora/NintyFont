// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for BoolPicker GUI control class
#include "boolpicker.h"

#include <fmt/core.h>

namespace NintyFont::GUI::Controls
{
    BoolPicker::BoolPicker(std::vector<PropertyList::PropertyBase *> **t_propList, PropertyList::PropertyListEntryDescriptor *t_descriptor, MemberCallback *t_toggleCallback, QWidget *parent)
        : QPushButton(parent)
        , Control()
    {
        propList = t_propList;
        descriptor = t_descriptor;
        toggleCallback = t_toggleCallback;

        connect(this, &QPushButton::toggled, this, &BoolPicker::updateProperty);
        setCheckable(true);
    }

    void BoolPicker::update()
    {
        lockControl();
        if (descriptor->propType == PropertyList::PropertyType::Bool)
        {
            PropertyList::Property<bool> *vprop = (PropertyList::Property<bool> *)(*propList)->at(descriptor->index);
            setChecked(vprop->value);
        }
        else
        {
            throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Wrong data type for EndiannessPicker!", __FILE__, __LINE__));
        }
        updateDisplay();
        unlockControl();
    }

    void BoolPicker::updateProperty()
    { 
        if (descriptor->propType == PropertyList::PropertyType::Bool)
        {
            PropertyList::Property<bool> *vprop = (PropertyList::Property<bool> *)(*propList)->at(descriptor->index);
            vprop->value = isChecked();
        }
        else
        {
            throw std::runtime_error(fmt::format("NintyFont runtime error in {} at line {}: Wrong data type for EndiannessPicker!", __FILE__, __LINE__));
        }
        doMemberCallback(toggleCallback);
        updateDisplay();
    }

    void BoolPicker::updateDisplay()
    {
        if (isChecked())
        {
            setText("On");
        }
        else
        {
            setText("Off");
        }
    }

    void BoolPicker::lockControl()
    {
        setEnabled(false);
        blockSignals(true);
    }

    void BoolPicker::unlockControl()
    {
        setEnabled(true);
        blockSignals(false);
    }

    BoolPicker::~BoolPicker()
    {
        
    }
}
