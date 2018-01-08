#include "segment2p.h"

Segment2P::Segment2P(Point2D* _p1, Point2D* _p2, object_id_t _id): GeometryObject(GeometryObjectType::Segment, _id),
    QGraphicsLineItem(_p1->getPos().x(), _p1->getPos().y(), _p2->getPos().x(), _p2->getPos().y()), p1(_p1), p2(_p2)
{
    //const QPen default_style();

    length = Point2D::distance_between(*p1, *p2);
}

QList<Point2D *> Segment2P::getPointsList()
{
    return {p1, p2};
}

void Segment2P::movePoint(Segment2P::PointPosition num, QPointF position)
{
    if (num == Segment2P::PointPosition::First) {
        this->setLine(position.x(), position.y(), line().p2().x(), line().p2().y());
    }
    if (num == Segment2P::PointPosition::Second) {
        this->setLine(line().p1().x(), line().p1().y(), position.x(), position.y());
    }
}
