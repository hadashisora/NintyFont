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

#include "formats/fontbase.h"
#include "viewwidget.h"
#include "qhexspinbox.h"
#include "unicodenames.h"
#include "propertylist.h"
#include "controls/controls.h"
#include "membercallback.h"

namespace NintyFont::GUI
{
    class FontInfoPanel : public QDockWidget
    {
    private:
        FontBase **font;
        MemberCallback *editCallback;
        //GUI stuff
        QWidget *w;
        QVBoxLayout *layout;
        //Property list stuff
        std::vector<PropertyList::PropertyBase *> *propList;
        std::vector<PropertyList::PropertyBase *> *drawablePropList;
        std::vector<Controls::Control *> controls;
        //Even handling
        static CallbackFunction editCallbackEvent;
    public:
        //Ctor
        FontInfoPanel(FontBase **font);
        //Dtor
        ~FontInfoPanel();
        //Public methods
        void update(void);
        void updateOnFontLoad(ViewWidget *scene = nullptr);
    };
}
