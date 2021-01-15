// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the header file for view widget,
// which is responsible for drawing stuff
// to the QGraphicsScene
#pragma once

#include <cstdint>
#include <QtWidgets>
#include <QObject>
#include <QWidget>

#include "globalstuffs.h"

namespace NintyFont
{
    struct GlobalStuffs; //Forward-declaration
}

namespace NintyFont::GUI
{
    class ViewWidget : public QGraphicsView
    {
    private:
        GlobalStuffs *globals;
    public:
        //Ctor
        ViewWidget(GlobalStuffs *globals);
        //Dtor
        ~ViewWidget();
        //Methods
        void updateDisplay(void);
        void updateLayout(void);
        void resizeEvent(QResizeEvent *event) override;
        void selectionChangedEvent(void);
        void drawForeground(QPainter *painter, const QRectF &rect) override;
        //Stuffs
        //float zoom;
        uint32_t columns;
        bool drawLeading;
        bool drawAscent;
        bool drawBaseline;
        bool drawWidths;
    };
}
