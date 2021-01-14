// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for bool picker GUI control
// class for the dynamic property system
#pragma once

#include <cstdint>
#include <QObject>
#include <QtWidgets>

#include "control.h"
#include "propertylist.h"
#include "unicodenames.h"
#include "membercallback.h"

namespace NintyFont::GUI::Controls
{
    class BoolPicker : public QPushButton, public Control
    {
    private:
        std::vector<PropertyList::PropertyBase *> **propList; //Pointer to a pointer to property list of the selected glyph. Should be set to nullptr if no glyph is selected (or more than one is selected)
        PropertyList::PropertyListEntryDescriptor *descriptor;
        MemberCallback *toggleCallback;
        //Private methods
        void updateProperty();
    public:
        //Ctor
        BoolPicker(std::vector<PropertyList::PropertyBase *> **propList, PropertyList::PropertyListEntryDescriptor *descriptor, MemberCallback *toggleCallback = nullptr, QWidget *parent = nullptr);
        //Dtor
        virtual ~BoolPicker();
        //Public methods
        void update(void) override;
        void lockControl(void) override;
        void unlockControl(void) override;
        virtual void updateDisplay(void);
    };
}
