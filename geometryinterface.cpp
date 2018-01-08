#include "geometryinterface.h"

GeometryInterface::GeometryInterface() : solver()
{
    containsConstraints = false;
    new_id = 1;
    const_id = 1;
}

object_id_t GeometryInterface::add_point(QPointF position)
{
    Point2D *ptr = new Point2D(position, new_id);
    objects.insert(new_id, ptr);

#ifdef ENABLE_DEBUG
qWarning() << "Added new point " << new_id << " at " << ptr->getPos();
#endif

    ++new_id;

return new_id-1;
}

GeometryObject* GeometryInterface::getObjectById(object_id_t id) {
    return objects[id];
}

object_id_t GeometryInterface::getIdByObject(GeometryObject *ptr) {
    for (auto& key: objects.keys()){
        if(objects[key] == ptr)
            return key;
    }

    return 0;
}

object_id_t GeometryInterface::addSegment(object_id_t first_p, object_id_t second_p) {
    if (getObjectById(first_p)->getType() != GeometryObjectType::Point || getObjectById(second_p)->getType() != GeometryObjectType::Point) {
        throw std::exception();
    }
    Point2D* first = dynamic_cast<Point2D *>(getObjectById(first_p));
    Point2D* second = dynamic_cast<Point2D *>(getObjectById(second_p));

    Segment2P *ptr = new Segment2P(first, second, new_id);
    objects.insert(new_id, ptr);
#ifdef ENABLE_DEBUG
qWarning() << "Added new segment " << new_id << " between " << first->getPos() << " and " << second->getPos();
#endif

    new_id++;

    return new_id - 1;
}

constraint_id_t GeometryInterface::addConstraint(ConstraintType _type, const QList<object_id_t>& _list) {
    Constraint* ptr = new Constraint(_type, _list, const_id);
    constraints.insert(const_id, ptr);

    for(const auto& item: _list) {
        getObjectById(item)->setBlock(true);
    }

#ifdef ENABLE_DEBUG
qWarning() << "Added new constraint " << const_id << " between ";
for (const auto& item: _list)
    qWarning() << item << " ";
#endif

    ++const_id;

    solver.resolve();

    return const_id-1;
}

constraint_id_t GeometryInterface::makeConstraintByPtrs(ConstraintType _type, GeometryObject *_first, GeometryObject *_second)
{
    QList<object_id_t> list{this->getIdByObject(_first), this->getIdByObject(_second)};
    return addConstraint(_type, list);
}

Segment2P* GeometryInterface::findLineByPoint(Point2D* _point) {
    for (const auto& key: objects.keys()) {
        if (objects[key]->getType() == GeometryObjectType::Segment) {
            Segment2P* segment = dynamic_cast<Segment2P*>(objects[key]);
            for (const Point2D* point: segment->getPointsList()) {
                if (point == _point) {
                    return segment;
                }
            }
        }
    }
    return Q_NULLPTR;
}

void GeometryInterface::movePoint(Point2D *point, QPointF position) {
    bool resolving;

    point->move(position);
    resolving = point->isBlocked();

    Segment2P *segment = findLineByPoint(point);
    QList<Point2D*> list = segment->getPointsList();
    if (point == list[0])
        segment->movePoint(Segment2P::PointPosition::First, position);
    if (point == list[1])
        segment->movePoint(Segment2P::PointPosition::Second, position);
    resolving = resolving || segment->isBlocked();

    if (resolving)
        solver.resolve();
}
