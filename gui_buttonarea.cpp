#include "gui_buttonarea.h"

GUI_ButtonArea::GUI_ButtonArea(QWidget *parent) : QToolBar(parent)
{
    setOrientation(Qt::Vertical);

    for (const auto& action: ba_actions) {
        addAction(QIcon(std::string(std::string(":/ButtonArea/Icons/Resources/") +
                        action + std::string("_icon.png")).c_str()), action.c_str());
    }
}
