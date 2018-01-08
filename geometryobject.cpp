#include "geometryobject.h"

GeometryObject::GeometryObject(GeometryObjectType _t, object_id_t _id)
{
    type = _t;
    objectID = _id;
    blocked = false;
}

GeometryObjectType GeometryObject::getType()
{
    return type;
}

bool GeometryObject::isBlocked()
{
    return blocked;
}

void GeometryObject::setBlock(bool _block) {
    blocked = _block;
}
