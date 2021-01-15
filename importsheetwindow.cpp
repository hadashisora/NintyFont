// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for ImportSheetWindow GUI class
#include "importsheetwindow.h"

namespace NintyFont::GUI
{
    ImportSheetWindow::ImportSheetWindow(GlobalStuffs *t_globals, QWidget *t_parent)
        : QDialog(t_parent)
    {
        globals = t_globals;

        //Create GUI elements
        QFormLayout *layout = new QFormLayout(this);
        formatSelector = new QComboBox();
        formatSelector->addItems({ "FontBFN1/JutResFont", "NFTR", "Pocket Monsters gen 4 font", "RFNT", "CFNT", "FFNT (3DS/CTR)", "FFNT (WiiU/Cafe)", "FFNT (NX/Switch)" });
        imagePathField = new QFilePathField("Open font sheet", "Image file (*.bmp *.png);;Portable Network Graphics (*.png);;Bit MaP (*.bmp);;Any file (*)");
        xlorPathField = new QFilePathField("Open letter order", "Xml Letter ORder (*.xlor);;Extensible Markup Language (*.xml);;Any file (*)");
        createButton = new QPushButton("Create");
        //Add elements to layout
        layout->addWidget(formatSelector);
        layout->addRow("Font sheet file", imagePathField);
        layout->addRow("Letter order file", xlorPathField);
        layout->addWidget(createButton);
    }

    int ImportSheetWindow::exec()
    {
        return QDialog::Accepted;
    }

    ImportSheetWindow::~ImportSheetWindow()
    {
        
    }
}
