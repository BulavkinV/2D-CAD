#ifndef POINT2D_H
#define POINT2D_H

#include "geometryobject.h"
#include <QGraphicsItem>

#include <cmath>
#include <vector>

const QPointF upper_left_point{-8., -8.};
const QPointF bottom_right_point{8., 8.};


class Point2D : public GeometryObject, public QGraphicsRectItem
{
private:
    double x;
    double y;
public:
    Point2D(double _x=0., double _y=0., object_id_t _id= 1);
    Point2D(QPointF _pos, object_id_t _id=1);

    QPointF getPos();
    void move(QPointF);


    static double distance_between(const Point2D&, const Point2D&);

    void notused() override {}
};

#endif // POINT2D_H
