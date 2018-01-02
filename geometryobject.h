#ifndef GEOMETRYOBJECT_H
#define GEOMETRYOBJECT_H

typedef unsigned object_id_t;

enum class GeometryObjectType: unsigned {
    None,
    Point,
    Segment
};


class GeometryObject
{
protected:
    object_id_t objectID;
    GeometryObjectType type;
public:
    GeometryObject(GeometryObjectType, object_id_t);
};

#endif // GEOMETRYOBJECT_H
