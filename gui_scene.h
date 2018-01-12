#ifndef GUI_SCENE_H
#define GUI_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QTransform>

#include "gui_mainwindow.h"
#include "geometryinterface.h"

//exceptions
#include "notimplementedexception.h"
#include "mainwindowwrongstate.h"

#include <point2d.h>
#include <segment2p.h>

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

const QPen tmp_line_pen(Qt::green, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);


class GUI_Scene : public QGraphicsScene
{
    GUI_MainWindow* mw;
    GeometryInterface* interface;

    Point2D *tmp_point;
    QGraphicsLineItem  *tmp_segment;
    void beginLine(QPointF);
    void endLine(QPointF, const QList<QGraphicsItem*>&);

    void movePoint(const QList<QGraphicsItem*>&);
public:
    GUI_Scene(GUI_MainWindow*, GeometryInterface*);

    //void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mousePressEventDispather(QPointF, const QList<QGraphicsItem*> &);
    void mouseMoveEventDispather(QPointF);

    static QList<GeometryObject*> fromSceneToObjects(const QList<QGraphicsItem*>&);
};

#endif // GUI_SCENE_H
