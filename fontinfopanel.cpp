// NintyFont - Nintendo binary font editor
// Copyleft TheDzeraora 2020-2021

// This software is provided under
// the GNU General Public License v3
// See license.txt in the root of the
// source tree for full license conditions

// Source file for FontInfoPanel GUI class
#include "fontinfopanel.h"
#include "controls/controls.h"

namespace NintyFont::GUI
{
    FontInfoPanel::FontInfoPanel(GlobalStuffs *t_globals, QWidget *t_parent)
        : QDockWidget(t_parent)
    {
        globals = t_globals;
        propList = nullptr;
        drawablePropList = nullptr;
        w = nullptr;
        layout = nullptr;

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

    namespace CharPropPanel
    {
        extern QFrame *createHorizontalLine();
    }

    void FontInfoPanel::updateOnFontLoad()
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
        if (globals->font == nullptr) return; //Failsafe
        propList = globals->font->getPropertiesList();
        if (propList == nullptr) return; //Failsafe
        drawablePropList = globals->font->getDrawablePropertiesList();
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
                    Controls::EndiannessPicker *control = new Controls::EndiannessPicker(&propList, (*prop)->descriptor, globals, w);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                case PropertyList::ControlType::BoolPicker:
                {
                    Controls::BoolPicker *control = new Controls::BoolPicker(&propList, (*prop)->descriptor, globals, w);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                case PropertyList::ControlType::SpinBox:
                {
                    Controls::SpinBox *control = new Controls::SpinBox(&propList, (*prop)->descriptor, globals, w);
                    lyt->addRow(QString::fromStdString((*prop)->descriptor->name), control);
                    controls.push_back(control);
                    break;
                }
                case PropertyList::ControlType::Label:
                {
                    bool isHex = false;
                    if ((*prop)->descriptor->valueRange.first == 1) isHex = true; //If the min of the range is set to one, then we treat the label as hex (in case it's a numeric property)
                    Controls::Label *control = new Controls::Label(&propList, (*prop)->descriptor, globals, isHex, w);
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
                Controls::BoolPicker *control = new Controls::BoolPicker(&drawablePropList, (*prop)->descriptor, globals, w);
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
        if (layout != nullptr) delete layout;
        if (w != nullptr) delete w;
    }
}
