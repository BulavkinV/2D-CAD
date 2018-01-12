#ifndef SOLVER_H
#define SOLVER_H

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

#include <QMap>
#include <geometryobject.h>
#include <constraint.h>
#include "geometryinterface.h"

#include <vector>
#include <cmath>

#include <stdexcept>
#include <notimplementedexception.h>
#include <drawingredifinedexception.h>

class GeometryInterface;

const double tolerance = 1e-1;
const double epsilon = 1e-16;
const double lambda_begin_value = 1.;

class Solver
{
    GeometryInterface* interface;
    QMap<object_id_t, QList<unsigned>> mapper;
    QMap<Constraint*, unsigned> const_mapper;

    std::vector<double> parameters_init_vals;
    std::vector<double> parameters_current_vals;

    std::vector<std::vector<double>> matrix;
    std::vector<double> vect;

    bool on_rebuild;
public:
    enum class MatrixInstances {
        None,
        Object,
        Constraint
    };

    Solver(GeometryInterface*);

    void resolve(const QList<Constraint*>&);
    void rebuild(const QList<Constraint*>&);
    void backMap();
    void reset();

    void matrixInitialise();
    bool containsObject(object_id_t);
    bool containsConstraint(Constraint*);
    QList<unsigned> addObject(object_id_t);
    QList<unsigned> addPoint(Point2D*);
    unsigned addNewConstraint(Constraint*);

    void checkMatrixBeforeExec(const std::vector<std::vector<double>>&);
    std::vector<double> gauss(std::vector<std::vector<double>>&);

    QList<unsigned> getIndicies(object_id_t);
    unsigned getConstraintIndex(Constraint*);

    void appendConstraint(Constraint*, QList<unsigned>);
    void addSamePointConstraint(Constraint*, QList<unsigned>);
    void addVerticalConstraint(Constraint*, QList<unsigned>);
    void addHorisontalConstraint(Constraint*, QList<unsigned>);
    void addFixedConstraint(Constraint*, QList<unsigned>);

    void setRebuild(bool);
    bool isRebuild();

    MatrixInstances instantTypeOnIndex(unsigned);


#ifdef ENABLE_DEBUG
    void debugStateOutput();
    void debugPrintMatrix(const std::vector<std::vector<double>>&);
    void debugPrintVector(const std::vector<double>&);
#endif
};

double norm(const std::vector<double>&);

std::vector<double> operator+(const std::vector<double>&, const std::vector<double>&);

#endif // SOLVER_H
