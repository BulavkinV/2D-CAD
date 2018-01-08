#include "point2d.h"

Point2D::Point2D(double _x, double _y, object_id_t _id):
    GeometryObject(GeometryObjectType::Point, _id),
    QGraphicsRectItem(0., 0., point_default_width, point_default_height, Q_NULLPTR), x(_x), y(_y) {

    setPos(_x, _y);
}

Point2D::Point2D(QPointF _pos, object_id_t _id):
    GeometryObject(GeometryObjectType::Point, _id),
    QGraphicsRectItem(0., 0., point_default_width, point_default_height, Q_NULLPTR), x(_pos.x()), y(_pos.y()) {

    setPos(_pos);
}

double Point2D::distance_between(const Point2D &p1, const Point2D &p2)
{
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
