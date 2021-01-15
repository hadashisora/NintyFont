// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes a GUI class
// for a dynamic/flexible property list for
// a font
#pragma once

#include <QtWidgets>
#include <QObject>
#include <cstdint>

#include "globalstuffs.h"
#include "propertylist.h"
#include "controls/control.h"

namespace NintyFont::GUI
{
    class ViewWidget;

    class FontInfoPanel : public QDockWidget
    {
    private:
        //FontBase **font;
        GlobalStuffs *globals;
        //GUI stuff
        QWidget *w;
        QVBoxLayout *layout;
        //Property list stuff
        std::vector<PropertyList::PropertyBase *> *propList;
        std::vector<PropertyList::PropertyBase *> *drawablePropList;
        std::vector<Controls::Control *> controls;
    public:
        //Ctor
        FontInfoPanel(GlobalStuffs *globals, QWidget *parent = nullptr);
        //Dtor
        ~FontInfoPanel();
        //Public methods
        void update(void);
        void updateOnFontLoad(void);
    };
}
