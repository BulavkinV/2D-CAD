#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <geometryobject.h>

#include <vector>

enum class ConstraintType: unsigned {
    None,
    SamePoint,
    Parallel,
    Perpendicular,
    AngleBetween,
    DistanceBetween,
    FixedPoint,
    FixedLength
};

class Constraint
{
private:
    ConstraintType type;
    std::vector<object_id_t> objects;
public:
    Constraint();
};

#endif // CONSTRAINT_H
