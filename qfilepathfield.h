// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <QtWidgets>
#include <QHBoxLayout>

namespace NintyFont::GUI
{
    class QFilePathField : public QWidget
    {
    public:
        //Ctor
        QFilePathField(std::string browseDlgTitle = "Open file", std::string fileExtensionText = "Any file (*)");
        //Dtor
        ~QFilePathField();
        //Methods
        void browseButtonClickEvent(void);
        //Fields
        std::string browseDlgTitle;
        std::string fileExtensionText;
        QTextEdit *pathField;
        QPushButton *browseButton;
    };
}
