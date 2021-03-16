// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "wid1entry.h"

namespace NintyFont::DOL::Format
{
    WID1Entry::WID1Entry(BinaryTools::BinaryReader *br)
    {
        w1 = br->readByte();
        w2 = br->readByte();
    }

    WID1Entry::~WID1Entry()
    {
        
    }
}
