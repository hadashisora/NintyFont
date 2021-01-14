// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for FontInfoPanel GUI class
#include "fontinfopanel.h"
#include "charproppanel.h"

namespace NintyFont::GUI
{
    FontInfoPanel::FontInfoPanel(FontBase **t_font)
        : QDockWidget()
    {
        font = t_font;
        propList = nullptr;
        drawablePropList = nullptr;
        w = nullptr;
        layout = nullptr;
        editCallback = new MemberCallback{&editCallbackEvent, this, nullptr};

        setWindowTitle("Font Info");
    }

    void FontInfoPanel::update()
    {
        //Don't lock/unlock controls here cause they do it on their own
        for (auto control = controls.begin(); control != controls.end(); control++)
        {
            (*control)->update();
        }
    }

    void FontInfoPanel::editCallbackEvent(void *a_self, void *) //Gonna name the "this" argument "self" a-la Python to avoid keyword collision
    {
        FontInfoPanel *self = (FontInfoPanel *)a_self; //Aka f*ck C++, we're gonna do it the C way
        (*(self->font))->edited = true;
        //qDebug() << "Font callback" << endl;
    }

    void FontInfoPanel::updateOnFontLoad(ViewWidget *scene)
    {
        //Delete the existing controls and layout
        for (auto control = controls.begin(); control != controls.end(); control++)
        {
            delete *control;
        }
        controls.clear();
        if (layout != nullptr) delete layout;
        if (w != nullptr) delete w;
        w = nullptr;
        layout = nullptr;

        //Fetch the new list of properties
        if ((*font) == nullptr) return; //Failsafe
        propList = (*font)->getPropertiesList();
        if (propList == nullptr) return; //Failsafe
        drawablePropList = (*font)->getDrawablePropertiesList();
        if (drawablePropList == nullptr) return; //Failsafe

        //Set up the GUI
        w = new QWidget();
        setWidget(w);
        layout = new QVBoxLayout(w);
        QFormLayout *lyt = new QFormLayout();
        for (auto prop = propList->begin(); prop != propList->end(); prop++)
        {
            switch ((*prop)->descriptor->ctrlType)
            {
                case PropertyList::ControlType::None:
                    break;
                case PropertyList::ControlType::EndiannessPicker:
                {
                    Controls::EndiannessPicker *control = new Controls::EndiannessPicker(&propList, (*prop)->descriptor, editCallback, w);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                case PropertyList::ControlType::BoolPicker:
                {
                    Controls::BoolPicker *control = new Controls::BoolPicker(&propList, (*prop)->descriptor, editCallback, w);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                case PropertyList::ControlType::SpinBox:
                {
                    Controls::SpinBox *control = new Controls::SpinBox(&propList, (*prop)->descriptor, editCallback, w);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                case PropertyList::ControlType::Label:
                {
                    bool isHex = false;
                    if ((*prop)->descriptor->valueRange.first == 1) isHex = true; //If the min of the range is set to one, then we treat the label as hex (in case it's a numeric property)
                    Controls::Label *control = new Controls::Label(&propList, (*prop)->descriptor, isHex, w, *font);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                default:
                    throw std::runtime_error("This hasn't been implemented yet...");
                    break;
            }
        }
        if (drawablePropList->size() != 0)
        {
            //lyt->addWidget(CharPropPanel::createHorizontalLine());
            lyt->addRow(CharPropPanel::createHorizontalLine());
            for (auto prop = drawablePropList->begin(); prop != drawablePropList->end(); prop++)
            {
                Controls::BoolPicker *control = new Controls::BoolPicker(&drawablePropList, (*prop)->descriptor);
                lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                controls.push_back(control);
            }
        }
        layout->addLayout(lyt);
        layout->addStretch();

        update();
    }

    FontInfoPanel::~FontInfoPanel()
    {
        //Delete the existing controls and layout
        // for (auto control = controls.begin(); control != controls.end(); control++)
        // {
        //     delete *control;
        // }
        //controls.clear();
        delete editCallback;
        if (layout != nullptr) delete layout;
        if (w != nullptr) delete w;
    }
}
