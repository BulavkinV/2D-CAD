#ifndef GEOMETRYINTERFACE_H
#define GEOMETRYINTERFACE_H

#include <geometryobject.h>
#include <constraint.h>

#include <vector>

class GeometryInterface
{
private:
    std::vector<GeometryObject*> objects;
    std::vector<Constraint*> constraints;

    bool containsConstraints;
public:
    GeometryInterface();
    bool add_segment();
    bool add_constraint();
};

#endif // GEOMETRYINTERFACE_H
