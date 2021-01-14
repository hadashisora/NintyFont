// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Header file for virtual base class for GUI
// controls for the dynamic property system
#pragma once

#include "propertylist.h"

namespace NintyFont::GUI::Controls
{
    class Control
    {
    public:
        virtual void update(void) = 0;
        virtual void lockControl(void) = 0;
        virtual void unlockControl(void) = 0;
        virtual ~Control() = 0;
    };

    inline Control::~Control() { } //https://stackoverflow.com/a/630960
}
