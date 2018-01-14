#include "segment2p.h"

Segment2P::Segment2P(Point2D* _p1, Point2D* _p2, object_id_t _id): GeometryObject(GeometryObjectType::Segment, _id),
    QGraphicsLineItem(_p1->getPos().x(), _p1->getPos().y(), _p2->getPos().x(), _p2->getPos().y()), p1(_p1), p2(_p2)
{
    //const QPen default_style();

    length = Point2D::distance_between(*p1, *p2);
    //createSelectionPolygon();

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

    update();
}

void Segment2P::setBlock(bool)
{
    blocked = true;

    p1->setBlock(true);
    p2->setBlock(true);
}


//QRectF Segment2P::boundingRect() const {
//    QPolygonF nPolygon;
//    qreal radAngle = line().angle()* M_PI / 180;
//    qreal dx = 20. * sin(radAngle);
//    qreal dy = 20. * cos(radAngle);
//    QPointF offset1 = QPointF(dx, dy);
//    QPointF offset2 = QPointF(-dx, -dy);
//    nPolygon << line().p1() + offset1
//             << line().p1() + offset2
//             << line().p2() + offset2
//             << line().p2() + offset1;
//    return nPolygon.boundingRect();
//}

//void Segment2P::createSelectionPolygon(){
//    QPolygonF nPolygon;
//    qreal radAngle = line().angle()* M_PI / 180;
//    qreal dx = selectionOffset * sin(radAngle);
//    qreal dy = selectionOffset * cos(radAngle);
//    QPointF offset1 = QPointF(dx, dy);
//    QPointF offset2 = QPointF(-dx, -dy);
//    nPolygon << line().p1() + offset1
//             << line().p1() + offset2
//             << line().p2() + offset2
//             << line().p2() + offset1;
//    selectionPolygon = nPolygon;
//    update();
//}

//QRectF Segment2P::boundingRect() const {
//    return selectionPolygon.boundingRect();
//}

//QPainterPath Segment2P::shape() const{
//    const int adjustment = 5;

//    QPainterPath path;
//    path.addRect(boundingRect().adjusted(-adjustment, -adjustment, adjustment, adjustment));
//    return path;
//}

//void Segment2P::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
//                           QWidget *widget){
//    Q_UNUSED(option);
//    Q_UNUSED(widget);
//    painter->setPen(pen());
//    painter->drawLine(line());
//    if (isSelected()) {
//        painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
//        painter->drawPolygon(selectionPolygon);
//    }
//}
