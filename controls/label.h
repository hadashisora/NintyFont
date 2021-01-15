// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for a label control GUI class
// for the dynamic property system
#pragma once

#include <cstdint>
#include <QtWidgets>

#include "globalstuffs.h"
#include "control.h"
#include "propertylist.h"

namespace NintyFont::GUI::Controls
{
    class Label : public QLabel, public Control
    {
    private:
        std::vector<PropertyList::PropertyBase *> **propList;
        PropertyList::PropertyListEntryDescriptor *descriptor;
        GlobalStuffs *globals;
        bool isHex;
    public:
        //Ctor
        Label(std::vector<PropertyList::PropertyBase *> **propList, PropertyList::PropertyListEntryDescriptor *descriptor, GlobalStuffs *globals, bool isHex = false, QWidget *parent = nullptr);
        //Dtor
        ~Label();
        //Public methods
        void update(void) override;
        void lockControl(void) override;
        void unlockControl(void) override;
    };
}
