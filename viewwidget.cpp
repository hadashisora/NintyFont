// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This is the source file for ViewWidget class
// Based on BRFNTify-Next code, ported to C++
#include "viewwidget.h"
#include <QDebug>

namespace NintyFont::GUI
{
    ViewWidget::ViewWidget(FontBase** t_font)
        : QGraphicsView()
    {
        font = t_font;
        zoom = 100.0;
        columns = 0;
        drawLeading = false;
        drawAscent = false;
        drawBaseline = false;
        drawWidths = false;

        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setBackgroundBrush(QBrush(QColor::fromRgb(119, 136, 153)));
        setMouseTracking(true);
        setVerticalScrollBar(new QScrollBar(Qt::Vertical, parentWidget()));
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    void ViewWidget::updateDisplay()
    {
        //printf("updateDisplay() called.\n");
        update();
    }

    void ViewWidget::updateLayout()
    {
        if (*font == nullptr)
        {
            scene()->setSceneRect(0, 0, 0, 0);
            return;
        }

        auto cellSize = (*font)->getGlyphImageSize();
        int32_t cols = (int32_t)floor((1.0F / (zoom / 100.0F)) * (float)viewport()->width() / (float)cellSize.first);
        if (cols < 1) cols = 1;

        columns = cols;

        for (uint32_t i = 0; i < (*font)->glyphs.size(); i++)
        {
            //uint32_t x = (*font)->cellSize.first * (i % cols);
            //uint32_t y = (*font)->cellSize.second * (uint32_t)(i / cols);
            (*font)->glyphs[i]->setPos(cellSize.first * (i % cols), cellSize.second * (i / cols));
        }

        scene()->setSceneRect(0, 0, cellSize.first * cols, cellSize.second * (1 + (int32_t)((*font)->glyphs.size() / cols)));
        update();
    }

    void ViewWidget::resizeEvent(QResizeEvent *event)
    {
        QGraphicsView::resizeEvent(event);
        updateLayout();
    }

    void ViewWidget::selectionChangedEvent()
    {
        update();
        //repaint();
    }

    void ViewWidget::drawForeground(QPainter *painter, const QRectF &rect)
    {
        if (*font == nullptr) return;

        float rows = (*font)->glyphs.size() / columns;
        if ((int32_t)rows == rows) rows = (uint32_t)rows;
        else rows = (uint32_t)rows + 1;

        //Draw drawable properties
        (*font)->drawDrawableProperties(painter, rows, columns);
    }

    ViewWidget::~ViewWidget()
    {

    }
}
