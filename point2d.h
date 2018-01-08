#ifndef POINT2D_H
#define POINT2D_H

#include "geometryobject.h"
#include <QGraphicsItem>

#include <cmath>
#include <vector>

const double point_default_width = 3.;
const double point_default_height = 3.;

class Point2D : public GeometryObject, public QGraphicsRectItem
{
private:
    double x;
    double y;
public:
    Point2D(double _x=0., double _y=0., object_id_t _id= 1);
    Point2D(QPointF _pos, object_id_t _id=1);

    static double distance_between(const Point2D&, const Point2D&);

    void notused() override {}
};

#endif // POINT2D_H
