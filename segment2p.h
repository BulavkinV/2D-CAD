#ifndef SEGMENT2P_H
#define SEGMENT2P_H

#include "geometryobject.h"
#include "point2d.h"
#include <QGraphicsLineItem>
#include <QPointF>
#include <QPen>
#include <QPainterPath>
#include <QList>
#include <QRectF>

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif


class Segment2P : public GeometryObject, public QGraphicsLineItem
{
private:
    Point2D* p1;
    Point2D* p2;

    double length;

//    const qreal selectionOffset;
//    QPolygonF selectionPolygon;
//    void createSelectionPolygon();
public:
    enum class PointPosition {
        None,
        First,
        Second
    };

    Segment2P(Point2D*, Point2D*, object_id_t);

    QList<Point2D*> getPointsList();
    void movePoint(PointPosition, QPointF);

    virtual void setBlock(bool) override;


    void notused() override {}
};

#endif // SEGMENT2P_H
