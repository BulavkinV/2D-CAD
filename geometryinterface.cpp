#include "geometryinterface.h"

GeometryInterface::GeometryInterface()
{
    containsConstraints = false;
    new_id = 1;
}

object_id_t GeometryInterface::add_point(QPointF position)
{
    Point2D *ptr = new Point2D(position, new_id);
    objects.insert(new_id, ptr);
    ++new_id;

    return new_id-1;
}

GeometryObject* GeometryInterface::getObjectById(object_id_t id) {
    return objects[id];
}


