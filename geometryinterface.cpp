#include "geometryinterface.h"

GeometryInterface::GeometryInterface()
{
    solver = new Solver(this);
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

QList<Constraint *> GeometryInterface::getConstraintsList()
{
    return constraints.values();
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

constraint_id_t GeometryInterface::addConstraint(ConstraintType _type, const QList<object_id_t>& _list, const QList<double> _value) {
    if (_type == ConstraintType::Parallel) {

    }

    Constraint* ptr = new Constraint(_type, _list, const_id, _value);
    constraints.insert(const_id, ptr);

    for(const auto& item: _list) {
        getObjectById(item)->setBlock(true);
    }

#ifdef ENABLE_DEBUG
qWarning() << "Added new constraint " << const_id << " for ";
for (const auto& item: _list)
    qWarning() << item << " ";
#endif

    ++const_id;

    solver->resolve(getConstraintsList());

    return const_id-1;
}

constraint_id_t GeometryInterface::makeConstraintByPtrs(ConstraintType _type, GeometryObject *_first, GeometryObject *_second, const QList<double>& _value)
{
    QList<object_id_t> list{this->getIdByObject(_first), this->getIdByObject(_second)};
    return addConstraint(_type, list, _value);
}

constraint_id_t GeometryInterface::createConstraint(const ConstraintType _type, const QList<GeometryObject*>& _objects, const QList<GeometryObjectType>& _aceptable_types, unsigned _number,
                                                    const QList<double>& _value)
{
    QList<object_id_t> result_list;

    for(const auto& object: _objects) {
        if (std::find(_aceptable_types.begin(), _aceptable_types.end(), object->getType()) != _aceptable_types.end()) {
            if (_number > 0) {
                result_list.push_back(getIdByObject(object));
                _number--;
            }
            else {
                break;
            }
       }
    }

    if (!_number) {
        return addConstraint(_type, result_list, _value);
    }
    else {
        return 0;
    }
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
        solver->resolve(getConstraintsList());
}


void GeometryInterface::replacePoint(Point2D *point, QPointF position) {
     point->move(position);

     Segment2P *segment = findLineByPoint(point);
     QList<Point2D*> list = segment->getPointsList();
     if (point == list[0])
         segment->movePoint(Segment2P::PointPosition::First, position);
     if (point == list[1])
         segment->movePoint(Segment2P::PointPosition::Second, position);
}

void GeometryInterface::eraseObjects()
{
    new_id = 1;
    const_id = 1;

//    for (const auto& key: objects.keys()) {
//        switch(objects[key]->getType()) {
//            case(GeometryObjectType::Point):
//                delete dynamic_cast<Point2D*>(objects[key]);
//                break;
//            case(GeometryObjectType::Segment):
//                delete dynamic_cast<Segment2P*>(objects[key]);
//                break;
//            default:
//                throw std::runtime_error("Geometry object of unknown type");
//        }
//        delete objects[key];
//    }
    objects.clear();
//    for (const auto& key: constraints.keys()) {
//        delete constraints[key];
//    }
    constraints.clear();

    containsConstraints = false;
}
