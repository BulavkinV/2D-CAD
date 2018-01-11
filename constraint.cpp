#include "constraint.h"

Constraint::Constraint(ConstraintType _type, const QList<object_id_t> &_list, constraint_id_t _id, const QList<double>& _value):
    id(_id), type(_type)
{
    objects = _list;
    value = _value;
}

ConstraintType Constraint::getType()
{
    return type;
}

QList<object_id_t> Constraint::getObjects()
{
    return objects;
}

QList<double> Constraint::getValue() {
    return value;
}


void Constraint::setValue(const QList<double> &_arg) {
    value = _arg;
}
