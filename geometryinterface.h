#ifndef GEOMETRYINTERFACE_H
#define GEOMETRYINTERFACE_H

#include <geometryobject.h>

class GeometryInterface
{
private:
    vector<GeometryObject*> objects;
    vector<Constraint*> constraints;
public:
    GeometryInterface();
    bool add_segment();
    bool add_constraint();
};

#endif // GEOMETRYINTERFACE_H
