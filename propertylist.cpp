// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This souce file contains code related to
// creating property list items(aka properties)
#include "propertylist.h"
#include "formats/fontbase.h"

namespace NintyFont::PropertyList
{
    PropertyListEntryDescriptor::PropertyListEntryDescriptor(uint32_t t_index, std::string t_name, PropertyType t_propType, ControlType t_ctrlType, std::pair<int64_t, int64_t> t_valueRange)
    {
        index = t_index;
        name = t_name;
        propType = t_propType;
        ctrlType = t_ctrlType;
        valueRange = t_valueRange;
    }

    PropertyBase::~PropertyBase()
    {
        
    }
}
