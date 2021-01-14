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

#include "formats/fontbase.h"

namespace NintyFont::GUI
{
    class ViewWidget : public QGraphicsView
    {
    private:
        FontBase **font; //This is a pointer to a pointer so we don't have to update this value manually
    public:
        //Ctor
        ViewWidget(FontBase **t_font);
        //Dtor
        ~ViewWidget();
        //Methods
        void updateDisplay(void);
        void updateLayout(void);
        void resizeEvent(QResizeEvent *event) override;
        void selectionChangedEvent(void);
        void drawForeground(QPainter *painter, const QRectF &rect) override;
        //Stuffs
        float zoom;
        int32_t columns;
        bool drawLeading;
        bool drawAscent;
        bool drawBaseline;
        bool drawWidths;
    };
}
