// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "endiannesspicker.h"

namespace NintyFont::GUI::Controls
{
    EndiannessPicker::EndiannessPicker(std::vector<PropertyList::PropertyBase *> **t_propList, PropertyList::PropertyListEntryDescriptor *t_descriptor, GlobalStuffs *t_globals, QWidget *t_parent)
        : BoolPicker(t_propList, t_descriptor, t_globals, t_parent) 
    { 

    }

    void EndiannessPicker::updateDisplay()
    {
        if (isChecked())
        {
            setText("LE");
        }
        else
        {
            setText("BE");
        }
    }

    EndiannessPicker::~EndiannessPicker()
    {
        
    }
}
