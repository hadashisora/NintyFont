// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes a GUI window class,
// which handles importing sheets along with XLOR (TBD)
#pragma once

#include <QtWidgets>

#include "formats/fontbase.h"
#include "qfilepathfield.h"

namespace NintyFont::GUI
{
    class ImportSheetWindow : public QDialog
    {
    private:
        //Fields
        FontBase *font;
        QComboBox *formatSelector;
        QFilePathField *imagePathField;
        QFilePathField *xlorPathField;
        QPushButton *createButton;
    public:
        //Ctor
        ImportSheetWindow(FontBase *font, QWidget *parent = nullptr);
        //Dtor
        ~ImportSheetWindow();
        //Methods
        int exec(void) override;
        void formatSelectionChangedEvent(void);
        void createButtonClickEvent(void);
        //Fields
    };
}
