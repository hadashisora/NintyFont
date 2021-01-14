// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// This header file describes a class,
// which contains a definition of a font
// glyph with dynamic list of properties
#pragma once

#include <cstdint>
#include <QtCore>
#include <QWidget>
#include <QtWidgets>
#include <QObject>
#include <QPainter>

#include "propertylist.h"

namespace NintyFont
{
    class Glyph : public QObject, public QGraphicsItem
    {
    private:
        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    public:
        //Constructors
        Glyph(void);
        Glyph(std::vector<PropertyList::PropertyBase *> *props, QPixmap* pixmap);
        //Destructor
        ~Glyph();
        //Methods
        static uint32_t getIndexOfFirstItemInStdVec(std::vector<Glyph *> vec, Glyph *item);
        void exportEvent(void); //These two were initially private, but I decided to add buttons in the glyph prop panel for convenience
        void importEvent(void);
        QRectF selectionRect(void) const;
        //Fields
        QPixmap *pixmap;
        //Property list stuff
        std::vector<PropertyList::PropertyBase *> *props;
    };
}
