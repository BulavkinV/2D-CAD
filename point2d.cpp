#include "point2d.h"

Point2D::Point2D(double _x, double _y, object_id_t _id): Point2D(QPointF(_x,_y), _id) {}

Point2D::Point2D(QPointF _pos, object_id_t _id):
    GeometryObject(GeometryObjectType::Point, _id),
    QGraphicsRectItem(upper_left_point.x(), upper_left_point.y(), bottom_right_point.x(), bottom_right_point.y(), Q_NULLPTR), x(_pos.x()), y(_pos.y()) {

    setPos(_pos);
}

QPointF Point2D::getPos()
{
    return QPointF(x, y);
}

double Point2D::distance_between(const Point2D &p1, const Point2D &p2)
{
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

void Point2D::move(QPointF position) {
   x = position.x();
   y = position.y();

   setPos(position);
}
