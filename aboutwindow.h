// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <QDialog>

namespace NintyFont::GUI
{
    class AboutWindow : public QDialog
    {
    public:
        //Ctor
        AboutWindow(QWidget *parent = nullptr);
        //Dtor
        ~AboutWindow();
    };
}
