#include "gui_buttonarea.h"

GUI_ButtonArea::GUI_ButtonArea(GUI_MainWindow *parent) : QToolBar(Q_NULLPTR)
{
    setOrientation(Qt::Vertical);

    for (const auto& action_name: ba_actions) {
       addAction(QIcon(QString(":/ButtonArea/Icons/Resources/") +
                        action_name + QString("_icon.png")), action_name, parent, [=] { parent->actionDispather(action_name); });
        if (std::find(separators_after.begin(), separators_after.end(), action_name) != separators_after.end()) {
            addSeparator();
        }
    }
}
