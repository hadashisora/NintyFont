// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the source file for QHexSpinBox
#include <fmt/core.h>

#include "qhexspinbox.h"

namespace NintyFont::GUI
{
    QHexSpinBox::QHexSpinBox(QString t_format)
        : QSpinBox()
    {
        format = t_format;

        setPrefix("0x");
        setDisplayIntegerBase(16);
    }

    QString QHexSpinBox::textFromValue(int32_t val) const
    {
        return QString::fromStdString(fmt::format(format.toStdString(), val));
    }

    QHexSpinBox::~QHexSpinBox()
    {
        
    }
}
