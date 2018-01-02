#ifndef POINT2D_H
#define POINT2D_H

#include "geometryobject.h"

#include <cmath>
#include <vector>

class Point2D : public GeometryObject
{
private:
    double x;
    double y;
public:
    Point2D(double _x=0., double _y=0., object_id_t _id= 1):
        GeometryObject(GeometryObjectType::Point, _id), x(_x), y(_y) {}

    static double distance_between(const Point2D&, const Point2D&);
};

#endif // POINT2D_H
