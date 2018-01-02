#ifndef SEGMENT2P_H
#define SEGMENT2P_H

#include "geometryobject.h"
#include "point2d.h"

class Segment2P : public GeometryObject
{
private:
    Point2D* const p1;
    Point2D* const p2;

    double length;
public:
    Segment2P(Point2D* const _p1, Point2D* const _p2, object_id_t _id):
        GeometryObject(GeometryObjectType::Segment, _id), p1(_p1), p2(_p2), length(Point2D::distance_between(*p1,*p2)) {}
};

#endif // SEGMENT2P_H
