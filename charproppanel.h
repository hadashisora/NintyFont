// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the header file for GUI class
// describing a flexible/dynamic glyph
// property panel
#pragma once

#include <QtWidgets>
#include <QObject>
#include <cstdint>

#include "globalstuffs.h"
#include "propertylist.h"
#include "controls/control.h"

namespace NintyFont::GUI
{
    class CharPropPanel : public QDockWidget
    {
    private:
        //FontBase **font;
        //QGraphicsScene *fontView; //This is here so that we can access the selected items directly
        //ViewWidget *view; //This is here so that we can force the view to update upon manipulating the glyphs
        //UnicodeNames *unicode; //This is here so we can get Unicode names for characters
        GlobalStuffs *globals;
        //Private methods
        uint32_t getIndexOfGlyphInStdVec(std::vector<Glyph *> vec, Glyph *item);
        //GUI stuff
        QWidget *w;
        QVBoxLayout *layout;
        QPushButton *addBtn;
        QPushButton *deleteBtn;
        QPushButton *importBtn;
        QPushButton *exportBtn;
        QLabel *glyphPixmapView;
        QPixmap *emptyGlyph;
        //Event handling
        void addEvent(void);
        void deleteEvent(void);
        void importEvent(void);
        void exportEvent(void);
        void lockControls(void);
        void unlockControls(void);
        //Property list stuff
        std::vector<PropertyList::PropertyListEntryDescriptor *> *props;
        std::vector<PropertyList::PropertyBase *> *propList;
        std::vector<Controls::Control *> controls;
    public:
        //Ctor
        //CharPropPanel(FontBase **font, QGraphicsScene *fontView, ViewWidget *view, UnicodeNames *unicode);
        CharPropPanel(GlobalStuffs *globals, QWidget *parent = nullptr);
        //Dtor
        ~CharPropPanel();
        //Public methods
        void updateOnFontLoad(void);
        void updateOnSelectionChanged(void);
        static QFrame *createHorizontalLine(void);
    };
}
