// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for a GUI spin box control
// for the dynamic glyph/font property system
#pragma once

#include <cstdint>
#include <QtWidgets>

#include "control.h"
#include "propertylist.h"
#include "membercallback.h"

namespace NintyFont::GUI::Controls
{
    class SpinBox : public QSpinBox, public Control
    {
    private:
        std::vector<PropertyList::PropertyBase *> **propList; //Pointer to a pointer to property of the selected glyph. Should be set to nullptr if no glyph is selected (or more than one is selected)
        PropertyList::PropertyListEntryDescriptor *descriptor;
        MemberCallback *toggleCallback;
        //Private methods
        void updateProperty(void);
    public:
        //Ctor
        SpinBox(std::vector<PropertyList::PropertyBase *> **propList, PropertyList::PropertyListEntryDescriptor *descriptor, MemberCallback *toggleCallback = nullptr, QWidget *parent = nullptr);
        //Dtor
        ~SpinBox();
        //Public methods
        void update(void) override;
        void lockControl(void) override;
        void unlockControl(void) override;
    };
}
