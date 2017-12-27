#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

class Point2D
{
private:
    double x;
    double y;
public:
    Point2D(double _x=0., double _y=0.): x(_x), y(_y) {}

    static double distance_between(const Point2D&, const Point2D&);
};

#endif // POINT2D_H
