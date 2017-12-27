#include "point2d.h"

double Point2D::distance_between(const Point2D &p1, const Point2D &p2)
{
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
