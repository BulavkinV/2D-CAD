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
    BelongsTo,
    Vertical,
    Horisontal
};

class Constraint
{
private:
    constraint_id_t id;
    ConstraintType type;
    QList<object_id_t> objects;

    QList<double> value;
public:
    Constraint(ConstraintType, const QList<object_id_t>&, constraint_id_t, const QList<double>&);

    ConstraintType getType();
    QList<object_id_t> getObjects();

    QList<double> getValue();
    void setValue(const QList<double>&);
};

#endif // CONSTRAINT_H
