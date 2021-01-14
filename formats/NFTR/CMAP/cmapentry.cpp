// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source code for CMAPEntry
#include "cmapentry.h"

namespace NintyFont::NTR::Format
{
    CMAPEntry::CMAPEntry(uint16_t t_code, uint16_t t_index)
    {
        code = t_code;
        index = t_index; //aka "gryphIndex" XD
    }

    CMAPEntry::CMAPEntry(BinaryTools::BinaryReader *br)
    {
        code = br->readUInt16();
        index = br->readUInt16();
    }

    void CMAPEntry::serialize(BinaryTools::BinaryWriter *bw)
    {
        bw->write(code);
        bw->write(index);
    }

    CMAPEntry::~CMAPEntry()
    {
        
    }
}
