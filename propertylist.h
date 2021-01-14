// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes property lists
// for building the GUI dynamically to have
// only the properties we need for a given
// format
#pragma once

#include <cstdint>
#include <string>

namespace NintyFont::PropertyList
{
    enum class PropertyType : uint8_t
    {
        Bool,
        Byte,
        SByte,
        UInt16,
        Int16,
        UInt32,
        Int32,
        UInt64,
        Int64,
        StdString,
        ImageFormat,
        CharEncoding,
        StdPairUInt32UInt32,
        Separator
    };
    enum class ControlType : uint8_t
    {
        None,
        SpinBox,
        HexSpinBox,
        Label,
        BoolPicker,
        EndiannessPicker,
        CodePointPicker
    };
    class PropertyListEntryDescriptor //This is a separate class for use with glyphs, where each one has the same kinds properties, so we don't want 10 gazillion copies of the same data
    {
    public:
        //Fields
        uint32_t index; //Property index in the list. Just so we don't have to pass the index around all the time
        std::string name; //Property name
        PropertyType propType; //Property data type
        ControlType ctrlType; //Property GUI control type
        std::pair<int64_t, int64_t> valueRange; //Range of acceptable values (only for numeric types), <min, max>
        //Ctor
        PropertyListEntryDescriptor(uint32_t index, std::string name, PropertyType propType, ControlType ctrlType, std::pair<int64_t, int64_t> valueRange = std::pair<int64_t, int64_t>(0, 0));
    };

    class PropertyBase
    {
    public:
        //Fields
        PropertyListEntryDescriptor *descriptor;
        virtual ~PropertyBase();
    };

    template <typename T> class Property : public PropertyBase
    {
    public:
        //Fields
        T value; //Property value
        //Ctors
        Property(PropertyListEntryDescriptor *descriptor, T value)
            : PropertyBase()
        {
            this->descriptor = descriptor;
            this->value = value;
        }
        Property(uint32_t index, std::string name, T value, PropertyType propType, ControlType ctrlType, std::pair<int64_t, int64_t> valueRange = std::pair<int64_t, int64_t>(0, 0))
            : PropertyBase()
        {
            descriptor = new PropertyListEntryDescriptor(index, name, propType, ctrlType, valueRange);
            this->value = value;
        }
        //Dtor
        ~Property() { }
    };
}
