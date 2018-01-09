#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <geometryobject.h>

#include <QList>

typedef unsigned constraint_id_t;

enum class ConstraintType: unsigned {
    None,
    SamePoint,
    Parallel,
    Perpendicular,
    AngleBetween,
    DistanceBetween,
    FixedPoint,
    FixedLength,
    BelongsTo
};

class Constraint
{
private:
    constraint_id_t id;
    ConstraintType type;
    QList<object_id_t> objects;
public:
    Constraint(ConstraintType, const QList<object_id_t>&, constraint_id_t);

    ConstraintType getType();
    QList<object_id_t> getObjects();
};

#endif // CONSTRAINT_H
