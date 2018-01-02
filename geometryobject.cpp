#include "geometryobject.h"

GeometryObject::GeometryObject(GeometryObjectType _t, object_id_t _id)
{
    type = _t;
    objectID = _id;
}
