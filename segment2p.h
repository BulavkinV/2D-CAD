#ifndef SEGMENT2P_H
#define SEGMENT2P_H

#include "point2d.h"

class Segment2P
{
private:
    Point2D p1;
    Point2D p2;

    double length;
public:
    Segment2P(): p1({}), p2({}), length(Point2D::distance_between(p1,p2)) {}
    Segment2P(const Point2D& _p1, const Point2D& _p2): p1(_p1), p2(_p2), length(Point2D::distance_between(p1,p2)) {}
};

#endif // SEGMENT2P_H
