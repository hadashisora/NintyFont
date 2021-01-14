// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This source file contains code related to
// handling the main window. A lot of the code
// is borrowed from BRFNTify-Next, ported to C++
#include <fmt/core.h>

#include "mainwindow.h"
#include "formats/formats.h"
#include "importsheetwindow.h"
#include "aboutwindow.h"
#include "formats/conv/bfttf.h"

namespace NintyFont::GUI
{
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
    {
        unicode = new UnicodeNames(); //Load the Unicode character names from DerivedName.txt (from ftp://ftp.unicode.org/Public/UNIDATA/UCD.zip)

        fontView = new QGraphicsScene();
        view = new ViewWidget(&font);
        view->setScene(fontView);
        connect(fontView, &QGraphicsScene::selectionChanged, view, &ViewWidget::selectionChangedEvent);
        setCentralWidget(view);
        setWindowTitle("NintyFont Dev");
        //setWindowIcon();

        fontInfo = new FontInfoPanel(&font);
        fontInfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        charProps = new CharPropPanel(&font, fontView, view, unicode);
        charProps->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        textPreview = nullptr;

        //textPreview->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);

        addDockWidget(Qt::RightDockWidgetArea, charProps);
        charProps->setHidden(true);
        addDockWidget(Qt::LeftDockWidgetArea, fontInfo);
        fontInfo->setHidden(true);
        //connect(fontView, &QGraphicsScene::selectionChanged, charProps, &CharPropPanel::updateOnSelectionChanged);

        //Create menu bar
        setMenuBar(new QMenuBar());

        //Create menus
        QMenu *fileMenu = new QMenu("&File", this);
        QMenu *editMenu = new QMenu("&Edit", this);
        QMenu *windowMenu = new QMenu("&Window", this);
        QMenu *toolsMenu = new QMenu("&Tools", this);
        QMenu *helpMenu = new QMenu("&Help", this);

        //Populate menus with actions
        //File
        fileMenu->addAction(createAction(&MainWindow::fileOpenEvent, nullptr, "&Open", "Open file", "Ctrl+O"));
        fileSaveAction = createAction(&MainWindow::fileSaveEvent, nullptr, "&Save", "Save file", "Ctrl+S");
        fileSaveAction->setEnabled(false);
        fileMenu->addAction(fileSaveAction);
        fileSaveAsAction = createAction(&MainWindow::fileSaveAsEvent, nullptr, "Save &As", "Save font file with a new filename", "Ctrl+Shift+S");
        fileSaveAsAction->setEnabled(false);
        fileMenu->addAction(fileSaveAsAction);
        fileMenu->addSeparator();
        QMenu *generateMenu = new QMenu("&Import", this);
        generateMenu->addAction(createAction(&MainWindow::fileGenerateFromPcFontEvent, nullptr, "From PC font", "Generate font from one of the fonts installed on the PC", ""));
        generateMenu->addAction(createAction(&MainWindow::fileGenerateFromSheetEvent, nullptr, "From image sheet", "Generate font from and image sheet and character order layout (as in SDK tools)", ""));
        fileMenu->addMenu(generateMenu);
        fileMenu->addSeparator();
        fileCloseAction = createAction(&MainWindow::fileCloseEvent, nullptr, "&Close", "Close file", "Ctrl+W");
        fileCloseAction->setEnabled(false);
        fileMenu->addAction(fileCloseAction);
        fileMenu->addSeparator();
        fileMenu->addAction(createAction(&MainWindow::fileExitEvent, nullptr, "E&xit", "Exit the program", "Ctrl+Q"));
        //Edit
        editMenu->addAction(createAction(nullptr, nullptr, "Duplicate", "Duplicate selected glyph", "Ctrl+D"));
        editMenu->addAction(createAction(nullptr, nullptr, "Delete", "Delete selected glyph", "Shift+Delete"));
        editMenu->addSeparator();
        editMenu->addAction(createAction(nullptr, nullptr, "Resize", "Resize font", ""));
        editMenu->addAction(createAction(nullptr, nullptr, "Reencode", "Convert font to another text encoding", ""));
        editMenu->addAction(createAction(nullptr, nullptr, "Convert", "Convert font to another format", ""));
        //Window
        windowToggleGlyphPropAction = createAction(&MainWindow::windowToggleGlyphPropEvent, nullptr, "Glyph properties", "Toggle glyph properties panel", "", true, false);
        windowMenu->addAction(windowToggleGlyphPropAction);
        windowToggleFontInfoAction = createAction(&MainWindow::windowToggleFontInfoEvent, nullptr, "Font properties", "Toggle font properties panel", "", true, false);
        windowMenu->addAction(windowToggleFontInfoAction);
        windowTogglePreviewAction = createAction(&MainWindow::windowTogglePreviewEvent, nullptr, "Preview", "Toggle font preview panel", "", true, false);
        windowMenu->addAction(windowTogglePreviewAction);
        //Tools
        QMenu *bfttfMenu = new QMenu("BFTTF", this);
        bfttfMenu->addAction(createAction(&MainWindow::toolsBfttfDecryptEvent, nullptr, "&Decrypt", "Decrypt BFTTF/BFOTF to TTF/OTF", ""));
        bfttfMenu->addAction(createAction(&MainWindow::toolsBfttfEncryptNxEvent, nullptr, "Encrypt (&NX)", "Decrypt TTF/OTF to BFTTF/BFOTF for Switch/NX", ""));
        bfttfMenu->addAction(createAction(&MainWindow::toolsBfttfEncryptCafeEvent, nullptr, "Encrypt (&Cafe)", "Decrypt TTF/OTF to BFTTF/BFOTF for WiiU/Cafe", ""));
        bfttfMenu->addAction(createAction(&MainWindow::toolsBfttfEncryptWin32Event, nullptr, "Encrypt (&Win32)", "Decrypt TTF/OTF to BFTTF/BFOTF for Windows/Win32", ""));
        toolsMenu->addMenu(bfttfMenu);
        //Help
        helpMenu->addAction(createAction(&MainWindow::helpAboutEvent, nullptr, "&About NintyFont", "", ""));
        //Add menus to the menu bar
        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(editMenu);
        menuBar()->addMenu(windowMenu);
        menuBar()->addMenu(toolsMenu);
        menuBar()->addMenu(helpMenu);

        //Create statusbar
        setStatusBar(new QStatusBar());

        //Zoom stuff
        QToolButton* zoomInBtn = new QToolButton();
        zoomInBtn->setText("+");
        zoomInBtn->setAutoRaise(true);
        connect(zoomInBtn, &QToolButton::clicked, this, &MainWindow::zoomPlusEvent);
        zoomInBtn->setShortcut(QKeySequence("Ctrl++"));
        QToolButton* zoomOutBtn = new QToolButton();
        zoomOutBtn->setText("-");
        zoomOutBtn->setAutoRaise(true);
        connect(zoomOutBtn, &QToolButton::clicked, this, &MainWindow::zoomMinusEvent);
        zoomOutBtn->setShortcut(QKeySequence("Ctrl+-"));
        zoomResetBtn = new QToolButton();
        zoomResetBtn->setText("100%");
        zoomResetBtn->setAutoRaise(true);
        connect(zoomResetBtn, &QToolButton::clicked, this, &MainWindow::zoomResetEvent);
        zoomResetBtn->setShortcut(QKeySequence("Ctrl+R"));
        zoomIndex = 6; //Index of 100%
        QHBoxLayout* lyt = new QHBoxLayout();
        lyt->setContentsMargins(0, 0, 0, 0);
        lyt->addWidget(zoomOutBtn);
        lyt->addWidget(zoomResetBtn);
        lyt->addWidget(zoomInBtn);
        QWidget* w = new QWidget();
        w->setLayout(lyt);
        statusBar()->addPermanentWidget(w);

        //Enumerate font format descriptors to construct the file dialog mask
        fileSelectMask = "All supported files (";
        std::string tmp = "";
        for (unsigned i = 0; i < NUM_FORMATS; i++)
        {
            fileSelectMask.append(fontDescriptors[i].getFileExtensionString());
            tmp.append(fontDescriptors[i].getFileTypeString());
            if (i != NUM_FORMATS - 1)
            {
                fileSelectMask.append(" ");
                tmp.append(";;");
            }
        }
        fileSelectMask.append(");;Any files (*);;");
        fileSelectMask.append(tmp);

        ignoreSaveWarning = false;
        openedFontFormatIndex = -1;
    }

    void MainWindow::zoomPlusEvent()
    {
        setZoom(zoomIndex + 1);
    }

    void MainWindow::zoomMinusEvent()
    {
        setZoom(zoomIndex - 1);
    }

    void MainWindow::zoomResetEvent()
    {
        setZoom(6); //Index of 100
    }

    const std::vector<uint16_t> zoomLevels = {20, 35, 50, 65, 80, 90, 100, 125, 150, 200, 300, 400, 500, 600, 700, 800};

    void MainWindow::setZoom(int32_t newIndex)
    {
        uint16_t oldZoom = zoomLevels[zoomIndex];
        zoomIndex = newIndex;
        if (zoomIndex < 0) zoomIndex = 0;
        if (zoomIndex > zoomLevels.size() - 1U) zoomIndex = zoomLevels.size() - 1U;

        uint16_t newZoom = zoomLevels[zoomIndex];

        std::string tmp = fmt::format("{}%", newZoom);
        zoomResetBtn->setText(QString::fromStdString(tmp)); //The stupidity of using QString continues...

        view->scale((float)newZoom/(float)oldZoom, (float)newZoom/(float)oldZoom);
        view->zoom = newZoom;
        view->updateLayout();
    }

    QAction *MainWindow::createAction(void (MainWindow::*function)(void), QIcon* icon, QString text, QString statustext, QString shortcut, bool toggle, bool checked)
    {
        QAction* act;
        if (icon != nullptr)
            act = new QAction(*icon, text, this);
        else
            act = new QAction(text, this);
        if (shortcut != "") act->setShortcut(shortcut);
        if (statustext != "") act->setStatusTip(statustext);
        act->setCheckable(toggle);
        if (toggle) act->setChecked(checked);
        if (function != nullptr) connect(act, &QAction::triggered, this, function);
        return act;
    }

    void MainWindow::fileOpenEvent()
    {
        if (font != nullptr && font->edited)
        {
            QMessageBox msg = QMessageBox(this);
            msg.setText("Save changes to file before closing?");
            msg.setInformativeText("If you don't save, changes made to the file will be permanently lost.");
            msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Save);
            switch (msg.exec())
            {
                case QMessageBox::Save:
                    fileSaveEvent(); //Fall-through
                case QMessageBox::Discard:
                    break;
                case QMessageBox::Cancel:
                    return;
                default:
                    throw std::runtime_error("Somehow this messed up, so I'm just gonna crash :)");
            }
        }

        //Ask for file to be opened
        //QFileDialog dlg = QFileDialog(this, "Open font file", "", "All supported files (*.brfnt *.rfnt *.bcfnt *.cfnt *.bffnt *.ffnt *.bfn *.nftr *.rtfn *.bin *.dat *.pm4f);;Any files (*);;NW4R font files (*.brfnt *.rfnt);;NW4C font files (*.bcfnt *.cfnt);;NW4F/NW4C font files (*.bffnt *.ffnt);;NITRO SDK font (*.nftr *.rtfn);;Pocket Monsters gen 4 font (*.bin *.dat *.pm4f)");
        QFileDialog dlg = QFileDialog(this, "Open font file", "", QString::fromStdString(fileSelectMask));
        dlg.setFileMode(QFileDialog::ExistingFile);
        QString filePath = "";
        if (!dlg.exec()) 
        {
            statusBar()->showMessage("User aborted loading file!");
            return; //Abort if user hasn't selected a file
        }
        filePath = dlg.selectedFiles()[0];

        QString fontFormat = ""; //We'll put the format name here, so it can be displayed in the GUI

        //Open file
        BinaryTools::BinaryReader file = BinaryTools::BinaryReader(filePath.toStdString(), true);

        FontBase *newFont = nullptr;

        uint8_t *bytes = file.readBytes(8); //Hardcoded to 8 for now, since that's the maximum number of magic bytes r/n

        for (unsigned i = 0; i < NUM_FORMATS; i++) //Iterate through all the formats
        {
            if (fontDescriptors[i].identifyFile(bytes)) //If the identification function returns true, then this is the right class to handle the format
            {
                file.close();
                newFont = fontDescriptors[i].ctor(filePath.toStdString());
                openedFontFormatIndex = i;
                break;
            }
        }

        //Failsafes in case something went wrong during loading
        if (newFont == nullptr) 
        {
            statusBar()->showMessage("Unsupported file or memory allocation error!");
            openedFontFormatIndex = -1;
            return; //Failsafe
        }
        if (newFont->glyphs.size() == 0) 
        {
            delete newFont;
            statusBar()->showMessage("Error while loading font file!");
            openedFontFormatIndex = -1;
            return; //Another failsafe
        }

        //Clear the fontView selection
        fontView->clearSelection();
        //Get rid of the old font data if there's any
        if (font != nullptr) delete font;
        //Clear the fontView
        fontView->clear();
        //Set the new font
        font = newFont;
        font->edited = false;
        //Copy glyphs to GUI
        for (auto glyph = font->glyphs.begin(); glyph != font->glyphs.end(); glyph++)
        {
            fontView->addItem(*glyph);
        }
        view->updateDisplay();
        view->columns = 1;
        view->updateLayout();
        charProps->updateOnFontLoad();
        fontInfo->updateOnFontLoad(view);

        fileSaveAction->setEnabled(true);
        fileSaveAsAction->setEnabled(true);
        fileCloseAction->setEnabled(true);

        //if (charProps->isHidden()) windowToggleGlyphPropAction->trigger();
        //if (fontInfo->isHidden()) windowToggleFontInfoAction->trigger();
        toggleGlyphProp(true);
        toggleFontProp(true);

        setWindowTitle(QString::fromStdString(fmt::format("NintyFont Dev - {}", filePath.toStdString())));
    }

    void MainWindow::fileSaveEvent()
    {
        if (font == nullptr) return; //Failsafe

        if (!ignoreSaveWarning)
        {
            QMessageBox msg = QMessageBox(this);
            msg.setText("Use Save As instead?");
            msg.setInformativeText("The program is still in development and saving might corrupt the original file.");
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Ignore);
            msg.setDefaultButton(QMessageBox::Yes);
            switch (msg.exec())
            {
                case QMessageBox::Yes:
                    fileSaveAsEvent();
                    return;
                case QMessageBox::No:
                    break;
                case QMessageBox::Ignore:
                    ignoreSaveWarning = true;
                    break;
                default:
                    throw std::runtime_error("Somehow this messed up, so I'm just gonna crash :)");
            }
        }

        font->saveBinaryFont(font->fontPath);
        font->edited = false;
        statusBar()->showMessage("Successfully overwrote file!");
    }

    void MainWindow::fileSaveAsEvent()
    {
        if (font == nullptr) return; //Failsafe
        QString str;
        if (openedFontFormatIndex > -1 && openedFontFormatIndex < NUM_FORMATS) str = QString::fromStdString(fontDescriptors[openedFontFormatIndex].getFileTypeString());
        else str = "Any files (*)";
        QFileDialog dlg = QFileDialog(this, "Save font file", "", str);
        dlg.setFileMode(QFileDialog::FileMode::AnyFile);
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        dlg.selectFile(QString::fromStdString(font->fontPath));
        QString filePath = "";
        if (!dlg.exec()) 
        {
            statusBar()->showMessage("User aborted saving file!");
            return; //Abort if user hasn't selected a file
        }
        filePath = dlg.selectedFiles()[0];
        font->fontPath = filePath.toStdString();
        font->saveBinaryFont(font->fontPath);
        ignoreSaveWarning = true;
        setWindowTitle(QString::fromStdString(fmt::format("NintyFont Dev - {}", font->fontPath)));
        font->edited = false;

        statusBar()->showMessage("Successfully saved file!");
    }

    void MainWindow::fileGenerateFromPcFontEvent()
    {

    }

    void MainWindow::fileGenerateFromSheetEvent()
    {
        return;
        FontBase *newFont = nullptr;
        ImportSheetWindow *dlg = new ImportSheetWindow(newFont);
        dlg->exec();
    }

    void MainWindow::fileCloseEvent()
    {
        if (font == nullptr) return; //Failsafe
        if (font->edited)
        {
            QMessageBox msg = QMessageBox(this);
            msg.setText("Save changes to file before closing?");
            msg.setInformativeText("If you don't save, changes made to the file will be permanently lost.");
            msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msg.setDefaultButton(QMessageBox::Save);
            switch (msg.exec())
            {
                case QMessageBox::Save:
                    fileSaveEvent(); //Fall-through
                case QMessageBox::Discard:
                    break;
                case QMessageBox::Cancel:
                    return;
                default:
                    throw std::runtime_error("Somehow this messed up, so I'm just gonna crash :)");
            }
        }

        //Clear the fontView selection
        fontView->clearSelection();
        //Get rid of the old font data
        delete font; //This takes care of disposing all the internal stuff
        //Clear the fontView
        fontView->clear();
        font = nullptr;
        fileSaveAction->setEnabled(false);
        fileSaveAsAction->setEnabled(false);
        fileCloseAction->setEnabled(false);

        view->updateDisplay();
        view->updateLayout();
        charProps->updateOnFontLoad();
        fontInfo->updateOnFontLoad();

        //if (!charProps->isHidden()) windowToggleGlyphPropAction->trigger();
        //if (!fontInfo->isHidden()) windowToggleFontInfoAction->trigger();
        toggleGlyphProp(false);
        toggleFontProp(false);

        setWindowTitle("NintyFont Dev");
    }

    void doBfttfOp(MainWindow *w, bool enc, BFTTF::Platform platform)
    {
        //First ask for which file we want to use as input
        QFileDialog *dlg;
        if (enc) dlg = new QFileDialog(w, "Open TTF/OTF to encrypt", "", "TrueType Font / OpenType Font (*.ttf *otf);;Any files (*)");
        else dlg = new QFileDialog(w, "Open BFTTF/BFOTF to decrypt", "", "Binary caFe TTF/OTF (*.bfttf *bfotf);;Any files (*)");
        dlg->setFileMode(QFileDialog::FileMode::ExistingFile);
        dlg->setAcceptMode(QFileDialog::AcceptOpen);
        QString inPath = "";
        if (!dlg->exec()) 
        {
            //w->statusBar()->showMessage("User aborted BFTTF/BFOTF operation!");
            return; //Abort if user hasn't selected a file
        }
        inPath = dlg->selectedFiles()[0];

        //Then we repurpose the same dialog for asking for where to save the new file
        if (enc) 
        {
            dlg->setWindowTitle("Save encrypted BFTTF/BFOTF");
            dlg->setNameFilter("Binary caFe TTF/OTF (*.bfttf *bfotf);;Any files (*)");
            
        }
        else 
        {
            dlg->setWindowTitle("Save decrypted TTF/OTF");
            dlg->setNameFilter("TrueType Font / OpenType Font (*.ttf *otf);;Any files (*)");
        }
        dlg->setFileMode(QFileDialog::FileMode::AnyFile);
        dlg->setAcceptMode(QFileDialog::AcceptSave);
        QString outPath = "";
        if (!dlg->exec()) 
        {
            //w->statusBar()->showMessage("User aborted BFTTF/BFOTF operation!");
            return; //Abort if user hasn't selected a file
        }
        outPath = dlg->selectedFiles()[0];

        BFTTF::Result result;

        if (enc) result = BFTTF::encrypt(inPath.toStdString(), outPath.toStdString(), platform);
        else result = BFTTF::decrypt(inPath.toStdString(), outPath.toStdString());

        switch (result)
        {
            case BFTTF::Result::OK:
                if (enc) w->statusBar()->showMessage("Successfully encrypted TTF/OTF!");
                else w->statusBar()->showMessage("Successfully decrypted BFTTF/BFOTF!");
                break;
            case BFTTF::Result::WRONG_FILE:
                w->statusBar()->showMessage("BFTTF/BFOTF decryption error - file is not a BFTTF/BFOTF!");
                break;
            case BFTTF::Result::INVALID_FILE:
                w->statusBar()->showMessage("BFTTF/BFOTF decryption error - broken file!");
                break;
            case BFTTF::Result::ERR:
                if (enc) w->statusBar()->showMessage("TTF/OTF encryption error!");
                else w->statusBar()->showMessage("BFTTF/BFOTF decryption error!");
                break;
            default:
                throw std::runtime_error("This somehow got messed up, so I guess it's a good reason to crash");
        }
    }

    void MainWindow::toolsBfttfDecryptEvent()
    {
        doBfttfOp(this, false, BFTTF::Platform::None);
        //BFTTF::decrypt("/mnt/DATADRIVE/ARCDRIVE/Project NX/hactool/upd301/FontStandard/FontStandard.bfttf", "/mnt/DATADRIVE/ARCDRIVE/Project NX/hactool/upd301/FontStandard/FontStandard.ttf");
        //BFTTF::encrypt("/mnt/DATADRIVE/ARCDRIVE/Project NX/hactool/upd301/FontStandard/FontStandard.ttf", "/mnt/DATADRIVE/ARCDRIVE/Project NX/hactool/upd301/FontStandard/FontStandard_edit.bfttf", BFTTF::Platform::NX);
    }

    void MainWindow::toolsBfttfEncryptNxEvent()
    {
        doBfttfOp(this, true, BFTTF::Platform::NX);
    }

    void MainWindow::toolsBfttfEncryptCafeEvent()
    {
        doBfttfOp(this, true, BFTTF::Platform::CAFE);
    }

    void MainWindow::toolsBfttfEncryptWin32Event()
    {
        doBfttfOp(this, true, BFTTF::Platform::CAFE);
    }

    void MainWindow::fileExitEvent()
    {
        fileCloseEvent(); //Attempt to close file before exiting just in case
        exit(0);
    }

    void MainWindow::toggleGlyphProp(bool on)
    {
        if (on)
        {
            windowToggleGlyphPropAction->blockSignals(true);
            windowToggleGlyphPropAction->setChecked(true);
            windowToggleGlyphPropAction->blockSignals(false);
            charProps->setHidden(false);
        }
        else
        {
            windowToggleGlyphPropAction->blockSignals(true);
            windowToggleGlyphPropAction->setChecked(false);
            windowToggleGlyphPropAction->blockSignals(false);
            charProps->setHidden(true);
        }
    }

    void MainWindow::toggleFontProp(bool on)
    {
        if (on)
        {
            windowToggleFontInfoAction->blockSignals(true);
            windowToggleFontInfoAction->setChecked(true);
            windowToggleFontInfoAction->blockSignals(false);
            fontInfo->setHidden(false);
        }
        else
        {
            windowToggleFontInfoAction->blockSignals(true);
            windowToggleFontInfoAction->setChecked(false);
            windowToggleFontInfoAction->blockSignals(false);
            fontInfo->setHidden(true);
        }
    }

    void MainWindow::togglePreview(bool on)
    {

    }

    void MainWindow::windowToggleGlyphPropEvent()
    {
        if (charProps->isHidden())
        {
            toggleGlyphProp(true);
        }
        else
        {
            toggleGlyphProp(false);
        }
    }

    void MainWindow::windowToggleFontInfoEvent()
    {
        if (fontInfo->isHidden())
        {
            toggleFontProp(true);
        }
        else
        {
            toggleFontProp(false);
        }
    }

    void MainWindow::windowTogglePreviewEvent()
    {
        if (fontInfo->isHidden())
        {
            togglePreview(true);
        }
        else
        {
            togglePreview(false);
        }
    }

    void MainWindow::viewLeadingEvent()
    {

    }

    void MainWindow::viewAscentEvent()
    {

    }

    void MainWindow::viewBaselineEvent()
    {

    }

    void MainWindow::viewWidthsEvent()
    {

    }

    void MainWindow::helpAboutEvent()
    {
        AboutWindow *about = new AboutWindow(this);
        about->show();
    }

    QSize MainWindow::sizeHint() const
    {
        return QSize(1280, 720);
    }

    MainWindow::~MainWindow()
    {
        delete font;
        delete view;
        delete fontView;
        delete fontInfo;
        delete charProps;
        delete textPreview;
        delete fileSaveAction;
        delete fileSaveAsAction;
        delete fileCloseAction;
        delete windowToggleGlyphPropAction;
        delete windowToggleFontInfoAction;
        delete windowTogglePreviewAction;
        delete zoomResetBtn;
        delete unicode;
    }
}

