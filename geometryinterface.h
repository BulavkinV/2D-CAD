#ifndef GEOMETRYINTERFACE_H
#define GEOMETRYINTERFACE_H

#include <geometryobject.h>
#include <constraint.h>
#include <point2d.h>
#include <segment2p.h>
#include "solver.h"

#include <exception>

#include <vector>
#include <QPointF>
#include <QMap>

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

class Solver;

class GeometryInterface
{
private:
    Solver* solver;
    QMap<object_id_t, GeometryObject*> objects;
    QMap<constraint_id_t, Constraint*> constraints;

    object_id_t new_id;
    constraint_id_t const_id;
    bool containsConstraints;
public:
    GeometryInterface();

    object_id_t addSegment(object_id_t, object_id_t);
    object_id_t add_point(QPointF);
    constraint_id_t addConstraint(ConstraintType _type, const QList<object_id_t>& _list, const QList<double> _value = {});
    constraint_id_t makeConstraintByPtrs(ConstraintType _type, GeometryObject *_first, GeometryObject *_second, const QList<double>& _value = {});

    GeometryObject* getObjectById(object_id_t);
    object_id_t getIdByObject(GeometryObject*);
    QList<Constraint*> getConstraintsList();

    Segment2P* findLineByPoint(Point2D* _point);
    void movePoint(Point2D *point, QPointF position);
    void replacePoint(Point2D *point, QPointF position);
};

#endif // GEOMETRYINTERFACE_H
