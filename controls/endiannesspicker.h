// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Changes BoolPicker control to
// say LE/BE instead of On/Off
#pragma once

#include "boolpicker.h"

namespace NintyFont::GUI::Controls
{
    class EndiannessPicker : public BoolPicker
    {
    public:
        //Ctor
        EndiannessPicker(std::vector<PropertyList::PropertyBase *> **propList, PropertyList::PropertyListEntryDescriptor *descriptor, GlobalStuffs *globals, QWidget *parent = nullptr);
        //Dtor
        ~EndiannessPicker();
        //Methods
        void updateDisplay(void) override;
    };
}
