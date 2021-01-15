// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions
#pragma once

#include <QGraphicsScene>

#include "formats/fontbase.h"
#include "viewwidget.h"
#include "unicodenames.h"

namespace NintyFont
{
    namespace GUI
    {
        class ViewWidget; //Forward-declaration
    }

    struct GlobalStuffs
    {
        FontBase *font;
        NintyFont::GUI::ViewWidget *view;
        QGraphicsScene *fontView;
        UnicodeNames *unicode;
        float zoom;
    };
}
