// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2019-2020

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for Glyph class
#include "glyph.h"

namespace NintyFont
{
    Glyph::Glyph()
        : QObject(), QGraphicsItem()
    {
        this->pixmap = new QPixmap(16, 16);

        this->setFlag(ItemIsMovable, false);
        this->setFlag(ItemIsSelectable, true);
        this->setFlag(ItemIsFocusable, true);
    }

    Glyph::Glyph(std::vector<PropertyList::PropertyBase *> *t_props, QPixmap *t_pixmap)
    {
        props = t_props;
        pixmap = t_pixmap;

        this->setFlag(ItemIsMovable, false);
        this->setFlag(ItemIsSelectable, true);
        this->setFlag(ItemIsFocusable, true);
    }

    QRectF Glyph::selectionRect() const
    {
        return QRectF(-1, -1, pixmap->width() + 1, pixmap->height() + 1);
    }

    QRectF Glyph::boundingRect() const
    {
        return QRectF(0,0, pixmap->width(), pixmap->height());
    }

    void Glyph::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
    {
        QMenu *menu = new QMenu();
        QAction *actImport = new QAction("Import", this);
        connect(actImport, &QAction::triggered, this, &Glyph::importEvent);
        QAction *actExport = new QAction("Export", this);
        connect(actExport, &QAction::triggered, this, &Glyph::exportEvent);
        menu->addAction(actImport);
        menu->addAction(actExport);
        menu->exec(event->screenPos());
        delete menu;
        delete actImport;
        delete actExport;
    }

    void Glyph::exportEvent()
    {
        //Ask for file to be opened
        QFileDialog dlg = QFileDialog(nullptr, "Save PNG file", "", "Portable Network Graphics (*.png)");
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        //dlg.setFileMode(QFileDialog::ExistingFile);
        QString filePath = "";
        if (!dlg.exec()) return; //Abort if user hasn't selected a file
        filePath = dlg.selectedFiles()[0];

        //Save file as PNG
        pixmap->save(filePath, "PNG");
    }

    void Glyph::importEvent()
    {
        //Ask for file to be opened
        QFileDialog dlg = QFileDialog(nullptr, "Open PNG file", "", "Portable Network Graphics (*.png)");
        dlg.setFileMode(QFileDialog::ExistingFile);
        QString filePath = "";
        if (!dlg.exec()) return; //Abort if user hasn't selected a file
        filePath = dlg.selectedFiles()[0];

        //Save file as PNG
        QPixmap *pix = new QPixmap(filePath);
        if (pix->width() != pixmap->width() || pix->height() != pixmap->height())
        {
            QMessageBox dialog = QMessageBox(QMessageBox::Icon::Critical, "Error", "The size of the provided image file does not match the size of the glyph! Aborting loading!", QMessageBox::StandardButton::Discard);
            dialog.exec();
            delete pix;
            return;
        }
        delete pixmap;
        pixmap = pix;
        update();
    }

    void Glyph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->drawPixmap(0, 0, *pixmap);

        if (isSelected())
        {
            painter->setPen(QPen(Qt::blue, 1.0, Qt::PenStyle::SolidLine, Qt::PenCapStyle::FlatCap, Qt::PenJoinStyle::MiterJoin));
            painter->drawRect(selectionRect());
            // painter->fillRect(selectionRect(), QColor::fromRgb(255, 255, 255, 64));
        }
    }

    Glyph::~Glyph()
    {
        delete pixmap;
        for (auto i = props->begin(); i != props->end(); i++)
        {
            delete *i;
        }
        delete props;
    }
}
