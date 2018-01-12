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

    bool blocked;
public:
    GeometryObject(GeometryObjectType, object_id_t);

    GeometryObjectType getType();
    bool isBlocked();
    virtual void setBlock(bool);

    virtual void notused() = 0;
};

#endif // GEOMETRYOBJECT_H
