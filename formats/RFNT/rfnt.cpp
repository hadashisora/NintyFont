// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "rfnt.h"

namespace NintyFont::RVL::Format
{
    RFNT::RFNT(BinaryTools::BinaryReader *br)
        : NFTR(br)
    {
        //Empty as everything is handled by the base constructor
    }

    RFNT::RFNT(uint16_t t_version, uint32_t t_magic)
        : NFTR(t_version, t_magic)
    {
        //Empty as everything is handled by the base constructor
    }

    void RFNT::validateSignature(void)
    {
        if (magic != 0x52464E54U && magic != 0x544E4652) throw std::runtime_error("Invalid RFNT signature!!!");
        else return;
    }

    void RFNT::serialize(BinaryTools::BinaryWriter *bw, BinaryTools::BlockLinker *linker)
    {
        NFTR::serialize(bw, linker);
    }
}
