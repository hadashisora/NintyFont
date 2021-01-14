// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes the main window
// class
#pragma once

#include <cstdint>
#include <QDebug>
#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QObject>

#include "viewwidget.h"
#include "fontinfopanel.h"
#include "binarytools/binaryreader.h"
#include "formats/fontbase.h"
#include "charproppanel.h"
#include "unicodenames.h"

namespace NintyFont::GUI
{
    class MainWindow : public QMainWindow
    {
    private:
        //GUI
        ViewWidget *view;
        QGraphicsScene *fontView;
        FontInfoPanel *fontInfo;
        CharPropPanel *charProps;
        QDockWidget *textPreview;
        QAction *fileSaveAction;
        QAction *fileSaveAsAction;
        QAction *fileCloseAction;
        QAction *windowToggleGlyphPropAction;
        QAction *windowToggleFontInfoAction;
        QAction *windowTogglePreviewAction;
        std::string fileSelectMask;
        bool ignoreSaveWarning;
        int openedFontFormatIndex; //-1 means none
        //Helper functions
        QAction *createAction(void (MainWindow::*function)(void), QIcon *icon, QString text, QString statustext, QString shortcut, bool toggle = false, bool checked = false);
        //Event handling
        void fileOpenEvent(void);
        void fileSaveEvent(void);
        void fileSaveAsEvent(void);
        void fileGenerateFromPcFontEvent(void); //Yes, I know these two are named Generate, yet the thing is called Import and the later code refers
        void fileGenerateFromSheetEvent(void); //to it as Import, just deal with it, I was too lazy to rename the early names.
        void fileCloseEvent(void);
        void fileExitEvent(void);
        void windowToggleGlyphPropEvent(void);
        void windowToggleFontInfoEvent(void);
        void windowTogglePreviewEvent(void);
        void toggleGlyphProp(bool on);
        void toggleFontProp(bool on);
        void togglePreview(bool on);
        void viewLeadingEvent(void);
        void viewAscentEvent(void);
        void viewBaselineEvent(void);
        void viewWidthsEvent(void);
        void toolsBfttfDecryptEvent(void);
        void toolsBfttfEncryptNxEvent(void);
        void toolsBfttfEncryptCafeEvent(void);
        void toolsBfttfEncryptWin32Event(void);
        void helpAboutEvent(void);
        void zoomPlusEvent(void);
        void zoomMinusEvent(void);
        void zoomResetEvent(void);
        void setZoom(int32_t newIndex);
        QSize sizeHint(void) const;
        //Font stuff
        FontBase *font = nullptr;
        //Zoom stuff
        QToolButton *zoomResetBtn;
        uint32_t zoomIndex;
        //Unicode thing
        UnicodeNames *unicode;
    public:
        //Ctor
        MainWindow(QWidget *parent = nullptr);
        //Dtor
        ~MainWindow();
    };
}
