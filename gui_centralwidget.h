#ifndef GUI_CENTRALWIDGET_H
#define GUI_CENTRALWIDGET_H

#include <QGraphicsView>
#include "gui_mainwindow.h"
#include "gui_scene.h"

#include <QMouseEvent>

class GUI_CentralWidget : public QGraphicsView
{
    GUI_MainWindow *mw_ptr;
public:
    GUI_CentralWidget(GUI_MainWindow*, GUI_Scene*);
    //using QGraphicsView::QGraphicsView;
};

#endif // GUI_CENTRALWIDGET_H
