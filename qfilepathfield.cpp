// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#include "qfilepathfield.h"

namespace NintyFont::GUI
{
    QFilePathField::QFilePathField(std::string t_browseDlgTitle, std::string t_fileExtensionText)
        : QWidget()
    {
        browseDlgTitle = t_browseDlgTitle;
        fileExtensionText = t_fileExtensionText;
        pathField = new QTextEdit(this);
        browseButton = new QPushButton(this);

        QHBoxLayout *layout = new QHBoxLayout(this);
        setLayout(layout);

        layout->addWidget(pathField);
        layout->addWidget(browseButton);

        connect(browseButton, &QPushButton::click, this, &QFilePathField::browseButtonClickEvent);
    }

    void QFilePathField::browseButtonClickEvent()
    {
        QFileDialog dlg = QFileDialog(this, QString::fromStdString(browseDlgTitle), "", QString::fromStdString(fileExtensionText));
        dlg.setFileMode(QFileDialog::ExistingFile);
        if (!dlg.exec()) return; //Abort if user hasn't selected a file
        pathField->setText(dlg.selectedFiles()[0]);
    }

    QFilePathField::~QFilePathField()
    {
        
    }
}
