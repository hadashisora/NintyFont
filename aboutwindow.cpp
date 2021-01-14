// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include "aboutwindow.h"

namespace NintyFont::GUI
{
    AboutWindow::AboutWindow(QWidget *parent)
        : QDialog(parent)
    {
        setWindowTitle("About NintyFont");
        setModal(false);
        QVBoxLayout *layout = new QVBoxLayout(this);
        QPlainTextEdit *txtedit = new QPlainTextEdit("NintyFont - Nintendo binary font editor\nCopyleft TheDzeraora 2018-2021\nThis software is provided under the GNU General Public License v3, see license.txt for details\n\n---Credits---\nTempus, RoadrunnerWMC\n    BRFNTify-Next - GUI layout and most of GUI code\n    TPLLib - DOL/RLV texture codec\ngdkchan, kwsch\n    Ohana3DS-Rebirth - CTR texture decoders\nCitra emulator for being a go-to reference on how to CMake\nTheDzeraora\n    Rewrting the original BRFNTify-Next to C++ as well as improving it to support more than one font format; binary IO; font format handling (NFTR, RFNT, PM gen 4, JUTResFont, CFNT); BFTTF/BFOTF crypto", this);
        txtedit->setReadOnly(true);
        layout->addWidget(txtedit);
        setLayout(layout);
        setMinimumWidth(512);
    }

    AboutWindow::~AboutWindow()
    {

    }
}
