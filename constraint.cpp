#include "constraint.h"

Constraint::Constraint(ConstraintType _type, const QList<object_id_t> &_list, constraint_id_t _id):
    id(_id), type(_type)
{
    objects = _list;
}

ConstraintType Constraint::getType()
{
    return type;
}

QList<object_id_t> Constraint::getObjects()
{
    return objects;
}
