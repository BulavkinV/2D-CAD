#ifndef GUI_BUTTONAREA_H
#define GUI_BUTTONAREA_H

#include <QToolBar>
#include <gui_mainwindow.h>

#include <vector>
#include <QString>

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

const std::vector<QString> ba_actions{"Cursor", "Input", "Vertical",
                                      "Horisontal", "Parallel",
                                       "Perpendicular", "Angle", "Distance",
                                      "Fixed", "Length", "Belongs"};

const std::vector<QString> separators_after{ba_actions[1]};

class GUI_ButtonArea : public QToolBar
{
    Q_OBJECT

public:
    explicit GUI_ButtonArea(GUI_MainWindow *parent = nullptr);

signals:

public slots:
};

#endif // GUI_BUTTONAREA_H
