#ifndef GUI_SCENE_H
#define GUI_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "gui_mainwindow.h"
#include "geometryinterface.h"

//exceptions
#include "notimplementedexception.h"
#include "mainwindowwrongstate.h"

#include <point2d.h>

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

class GUI_Scene : public QGraphicsScene
{
    GUI_MainWindow* mw;
    GeometryInterface* interface;

    void beginLine(QPointF);
public:
    GUI_Scene(GUI_MainWindow*, GeometryInterface*);

    //void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mousePressEventDispather(QPointF);
};

#endif // GUI_SCENE_H
