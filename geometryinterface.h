#ifndef GEOMETRYINTERFACE_H
#define GEOMETRYINTERFACE_H

#include <geometryobject.h>
#include <constraint.h>
#include <point2d.h>

#include <vector>
#include <QPointF>
#include <QMap>

class GeometryInterface
{
private:
    QMap<object_id_t, GeometryObject*> objects;
    std::vector<Constraint> constraints;

    object_id_t new_id;
    bool containsConstraints;
public:
    GeometryInterface();

    bool add_segment();
    object_id_t add_point(QPointF);
    bool add_constraint();

    GeometryObject* getObjectById(object_id_t);


};

#endif // GEOMETRYINTERFACE_H
