// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes an extension
// of Qt's QSpinBox class to display values
// in hexadecimal
#pragma once

#include <cstdint>
#include <QtWidgets>
#include <QObject>

namespace NintyFont::GUI
{
    class QHexSpinBox : public QSpinBox
    {
    public:
        //Ctor
        QHexSpinBox(QString t_format = "{:04X}");
        //Dtor
        ~QHexSpinBox();
        //Methods
        QString textFromValue(int32_t val) const override;
        //Fields
        QString format;
    };
}
