#include "solver.h"

Solver::Solver(GeometryInterface* _interface)
{
    interface = _interface;
}

void Solver::resolve(const QList<Constraint *>& _constraints)
{
    for (const auto& constraint: _constraints) {
        //get obj list
        QList<object_id_t> list = constraint->getObjects();
        //set indicies for objects
        QList<unsigned> indicies;
        for (const auto& obj_id: list) {
            if (containsObject(obj_id)) {
                indicies.append(getIndicies(obj_id));
            }
            else {
                //add obj to mapper and increase indicies
                indicies.append(addObject(obj_id));
            }
        }
        appendConstraint(constraint, indicies);
    }

    setRebuild(true);
    bool exit_cond;
    do {
        std::vector<std::vector<double>> A = matrix;
        std::vector<double> b = vect;

        for (unsigned i=0; i!=A.size(); ++i) {
            A[i].push_back(-b[i]);
        }
        //checkMatrixBeforeExec(A);
        //debugPrintMatrix(A);
        b = gauss(A);
        //debugPrintMatrix(A);
        if (std::isnan(b[0])) {
            qWarning() << "NaN";
            debugStateOutput();
        }
        parameters_current_vals = parameters_current_vals + b;
        //qWarning() << "Result:" << norm(b);
        //debugPrintVector(b);
        exit_cond = norm(b) >= tolerance;
        rebuild(_constraints);
    } while(exit_cond);

    backMap();
    reset();
}

void Solver::rebuild(const QList<Constraint *> & _constraints) {
    for (auto& str: matrix) {
        str.assign(matrix.size(), 0.);
    }
    vect.assign(vect.size(), 0.);

    matrixInitialise();

    for (const auto& constraint: _constraints) {
        QList<object_id_t> list = constraint->getObjects();
        QList<unsigned> indicies;
        for (const auto& obj_id: list) {
            if (containsObject(obj_id)) {
                indicies.append(getIndicies(obj_id));
            }
            else {
                qWarning() << "New object on rebuilding!!";
                indicies.append(addObject(obj_id));
            }
        }
        appendConstraint(constraint, indicies);
    }

    //debugPrintMatrix(matrix);
    //debugPrintVector(vect);
    //system("sleep 10");
}

// gauss method (must work)
std::vector<double> Solver::gauss(std::vector<std::vector<double>>& A) {
    int n = A.size();

    for (int i=0; i<n; i++) {
        // Search for maximum in this column
        double maxEl = abs(A[i][i]);
        int maxRow = i;
        for (int k=i+1; k<n; k++) {
            if (abs(A[k][i]) > maxEl) {
                maxEl = abs(A[k][i]);
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        for (int k=i; k<n+1;k++) {
            double tmp = A[maxRow][k];
            A[maxRow][k] = A[i][k];
            A[i][k] = tmp;
        }

        // Make all rows below this one 0 in current column
        for (int k=i+1; k<n; k++) {
            double c = -A[k][i]/A[i][i];
            for (int j=i; j<n+1; j++) {
                if (i==j) {
                    A[k][j] = 0;
                } else {
                    A[k][j] += c * A[i][j];
                }
            }
        }
    }

    // Solve equation Ax=b for an upper triangular matrix A
    std::vector<double> x(n);
    for (int i=n-1; i>=0; i--) {
        x[i] = A[i][n]/A[i][i];
        for (int k=i-1;k>=0; k--) {
            A[k][n] -= A[k][i] * x[i];
        }
    }
    return x;
}

QList<unsigned> Solver::getIndicies(object_id_t _id)
{
    return mapper[_id];
}

//check if ptr is in const_mapper
bool Solver::containsConstraint(Constraint * _ptr)
{
    return std::find(const_mapper.keyBegin(), const_mapper.keyEnd(), _ptr) != const_mapper.keyEnd();
}

//return index from const_mapper
unsigned Solver::getConstraintIndex(Constraint *_ptr)
{
    return const_mapper[_ptr];
}


void Solver::reset()
{
    setRebuild(false);
    mapper.clear();
    const_mapper.clear();

    parameters_current_vals.clear();
    parameters_init_vals.clear();

    matrix.clear();
    vect.clear();

    qWarning() << "Calculatuion finished!";
}

void Solver::matrixInitialise()
{
    for (const auto& key: mapper.keys()) {
        for (const auto& index: mapper[key]) {
            matrix[index][index] = 1.;
        }
    }
}

void Solver::backMap()
{
    for (const auto& key: mapper.keys()) {
        GeometryObject* obj = interface->getObjectById(key);
        if (obj->getType() == GeometryObjectType::Point) {
            Point2D* point = dynamic_cast<Point2D*>(obj);
            interface->replacePoint(point, {parameters_current_vals[mapper[key][0]], parameters_current_vals[mapper[key][1]]});
        }
    }
}

//check if an obj_id in mapper
bool Solver::containsObject(object_id_t _id)
{
    return std::find(mapper.keyBegin(), mapper.keyEnd(), _id) != mapper.keyEnd();
}

//adds point to matrix and vector and gets indicies
QList<unsigned> Solver::addPoint(Point2D *point) {
    QList<unsigned> result;
    QList<double> parameters{point->getPos().x(), point->getPos().y()};

    qWarning() << "Adding point " << point->getPos();
    for (const auto& parameter: parameters) {
        result.push_back(matrix.size());
        vect.resize(matrix.size()+1, 0.);
        if (isRebuild()) {
            vect[result.last()] = parameters_current_vals[result.last()] - parameters_init_vals[result.last()];
        }
        else {
            parameters_init_vals.push_back(parameter);
            parameters_current_vals.push_back(parameter);
        }
        matrix.resize(matrix.size()+1);
        for (auto &str: matrix) {
            str.resize(matrix.size(), 0.);
        }
        matrix[result.last()][result.last()] = 1.;
    }

    return result;
}

unsigned Solver::addNewConstraint(Constraint* _constraint)
{
    unsigned result = matrix.size();

   qWarning() << "Adding constraint";
    matrix.resize(matrix.size()+1);
    for (auto &str: matrix) {
        str.resize(matrix.size(), 0.);
    }

    vect.resize(matrix.size(), 0.);
    if (isRebuild()) {
        vect[result] = parameters_current_vals[result] - parameters_init_vals[result];
    }
    else
    {
        parameters_init_vals.resize(matrix.size(), lambda_begin_value);
        parameters_current_vals.resize(matrix.size(), lambda_begin_value);
    }
    const_mapper[_constraint] = result;

    return result;
}

void Solver::checkMatrixBeforeExec(const std::vector<std::vector<double> > &_M)
{
    for(unsigned i=0; i!=_M.size(); ++i) {
        if (_M[i] == std::vector<double>(_M[i].size(), 0.)) {
            switch(instantTypeOnIndex(i)){
                case(MatrixInstances::Constraint):
                    throw DrawingRedifinedException();
                    break;
                case(MatrixInstances::Object):
                case(MatrixInstances::None):
                default:
                    throw NotImplementedException();
            }
        }
    }
}


//adds object to matrix and returns indicies
QList<unsigned> Solver::addObject(object_id_t _id) {
    GeometryObject* object = interface->getObjectById(_id);
    QList<unsigned> indicies;

    if (object->getType() == GeometryObjectType::Point) {
        indicies.append(addPoint(dynamic_cast<Point2D*>(object)));
    }
    else if (object->getType() == GeometryObjectType::Segment) {
        Segment2P* segment = dynamic_cast<Segment2P*>(object);

        QList<Point2D*> point_list = segment->getPointsList();
        object_id_t point_id = interface->getIdByObject(static_cast<GeometryObject*>(point_list[0]));
        if (containsObject(point_id)) {
            indicies.append(getIndicies(point_id));
        }
        else {
            indicies.append(addObject(point_id));
        }
        point_id = interface->getIdByObject(static_cast<GeometryObject*>(point_list[1]));
        if (containsObject(point_id)) {
            indicies.append(getIndicies(point_id));
        }
        else {
            indicies.append(addObject(point_id));
        }
    }

    mapper[_id] = indicies;

    return indicies;
}

//appends constraint to matrix
void Solver::appendConstraint(Constraint *_constraint, QList<unsigned> _indicies)
{
    switch(_constraint->getType()) {
        case (ConstraintType::SamePoint):
            addSamePointConstraint(_constraint, _indicies);
            break;
        case (ConstraintType::Vertical):
            addVerticalConstraint(_constraint, _indicies);
            break;
        case (ConstraintType::Horisontal):
            addHorisontalConstraint(_constraint, _indicies);
            break;
        case (ConstraintType::FixedPoint):
            addFixedConstraint(_constraint, _indicies);
            break;
        case (ConstraintType::FixedLength):
            addLengthConstraint(_constraint, _indicies);
            break;
        case(ConstraintType::Parallel):
            addParallelConstraint(_constraint, _indicies);
            break;
        case(ConstraintType::Perpendicular):
            addPerpendicularConstraint(_constraint, _indicies);
            break;
        default:
            throw std::runtime_error("Not implemented constraint appending");
    }
}

unsigned Solver::getLambdaIndex(Constraint* _arg) {
    unsigned lambda_ind;

    if (containsConstraint(_arg)) {
        lambda_ind = getConstraintIndex(_arg);
    }
    else {
        lambda_ind = addNewConstraint(_arg);
    }

    return lambda_ind;
}

void Solver::addSamePointConstraint(Constraint* _constraint, QList<unsigned> _indicies) {
    unsigned lambda_ind = getLambdaIndex(_constraint);

    auto ix1 = _indicies[0];
    auto iy1 = _indicies[1];
    auto ix2 = _indicies[2];
    auto iy2 = _indicies[3];

    auto x1 = parameters_current_vals[ix1];
    auto y1 = parameters_current_vals[iy1];
    auto x2 = parameters_current_vals[ix2];
    auto y2 = parameters_current_vals[iy2];
    auto lambda = parameters_current_vals[lambda_ind];


    vect[ix1] += -2.*lambda*(x2-x1);
    vect[iy1] += -2.*lambda*(y2-y1);
    vect[ix2] += 2.*lambda*(x2-x1);
    vect[iy2] += 2.*lambda*(y2-y1);
    vect[lambda_ind] += (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);

    if ((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) < 1e-3) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][ix1] += -2.*(x2-x1);
    matrix[lambda_ind][iy1] += -2.*(y2-y1);
    matrix[lambda_ind][ix2] += 2.*(x2-x1);
    matrix[lambda_ind][iy2] += 2.*(y2-y1);
    matrix[ix1][ix1] += 2.*lambda;
    matrix[ix1][ix2] += -2.*lambda;
    matrix[ix1][lambda_ind] += -2.*(x2-x1);
    matrix[iy1][iy1] += 2.*lambda;
    matrix[iy1][iy2] += -2.*lambda;
    matrix[iy1][lambda_ind] += -2.*(y2-y1);
    matrix[ix2][ix2] += 2.*lambda;
    matrix[ix2][ix1] += -2.*lambda;
    matrix[ix2][lambda_ind] += 2.*(x2-x1);
    matrix[iy2][iy2] += 2.*lambda;
    matrix[iy2][iy1] += -2.*lambda;
    matrix[iy2][lambda_ind] += 2.*(y2-y1);
}

void Solver::addVerticalConstraint(Constraint* _constraint, QList<unsigned> _indicies) {
    unsigned lambda_ind = getLambdaIndex(_constraint);

    auto ix1 = _indicies[0];
    //auto iy1 = _indicies[1];
    auto ix2 = _indicies[2];
    //auto iy2 = _indicies[3];

    auto x1 = parameters_current_vals[ix1];
    //auto y1 = parameters_current_vals[iy1];
    auto x2 = parameters_current_vals[ix2];
    //auto y2 = parameters_current_vals[iy2];
    auto lambda = parameters_current_vals[lambda_ind];

    vect[lambda_ind] += (x2-x1)*(x2-x1);
    vect[ix1] += 2*lambda*(x1-x2);
    vect[ix2] += 2*lambda*(x2-x1);

    if ((x2-x1)*(x2-x1) < 1e-3) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][ix1] += 2.*(x1-x2);
    matrix[lambda_ind][ix2] += 2.*(x2-x1);
    matrix[ix1][ix1] += 2.*lambda;
    matrix[ix1][ix2] += -2.*lambda;
    matrix[ix1][lambda_ind] += 2*(x1-x2);
    matrix[ix2][ix2] += 2.*lambda;
    matrix[ix2][ix1] += -2.*lambda;
    matrix[ix2][lambda_ind] += 2*(x2-x1);

}

void Solver::addHorisontalConstraint(Constraint* _constraint, QList<unsigned> _indicies) {
    unsigned lambda_ind = getLambdaIndex(_constraint);

    //auto ix1 = _indicies[0];
    auto iy1 = _indicies[1];
    //auto ix2 = _indicies[2];
    auto iy2 = _indicies[3];

    //auto x1 = parameters_current_vals[ix1];
    auto y1 = parameters_current_vals[iy1];
    //auto x2 = parameters_current_vals[ix2];
    auto y2 = parameters_current_vals[iy2];
    auto lambda = parameters_current_vals[lambda_ind];

    vect[lambda_ind] += (y2-y1)*(y2-y1);
    vect[iy1] += 2*lambda*(y1-y2);
    vect[iy2] += 2*lambda*(y2-y1);

    if ((y2-y1)*(y2-y1) < 1e-3) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][iy1] += 2.*(y1-y2);
    matrix[lambda_ind][iy2] += 2.*(y2-y1);
    matrix[iy1][iy1] += 2.*lambda;
    matrix[iy1][iy2] += -2.*lambda;
    matrix[iy1][lambda_ind] += 2*(y1-y2);
    matrix[iy2][iy2] += 2.*lambda;
    matrix[iy2][iy1] += -2.*lambda;
    matrix[iy2][lambda_ind] += 2*(y2-y1);
}

void Solver::addFixedConstraint(Constraint* _constraint, QList<unsigned> _indicies) {
    auto ix = _indicies[0];
    auto iy = _indicies[1];

    auto x = parameters_current_vals[ix];
    auto y = parameters_current_vals[iy];
    auto x0 = parameters_init_vals[ix];
    auto y0 = parameters_init_vals[iy];

    if (((x-x0)*(x-x0)+(y-y0)*(y-y0)) < 1e-3 && !containsConstraint(_constraint)) {
        return;
    }

    unsigned lambda_ind = getLambdaIndex(_constraint);

    auto lambda = parameters_current_vals[lambda_ind];

    vect[lambda_ind] += (x-x0)*(x-x0)+(y-y0)*(y-y0);
    vect[ix] += 2.*lambda*(x-x0);
    vect[iy] += 2.*lambda*(y-y0);

    if ((x-x0)*(x-x0)+(y-y0)*(y-y0) < 1e-6) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][ix] += 2.*(x-x0);
    matrix[lambda_ind][iy] += 2.*(y-y0);
    matrix[ix][ix] += 2.*lambda;
    matrix[ix][lambda_ind] += 2*(x-x0);
    matrix[iy][iy] += 2.*lambda;
    matrix[iy][lambda_ind] += 2*(y-y0);
}

void Solver::addLengthConstraint(Constraint * _constraint, QList<unsigned> _indicies)
{
    auto lambda_ind = getLambdaIndex(_constraint);
    auto ix1 = _indicies[0];
    auto iy1 = _indicies[1];
    auto ix2 = _indicies[2];
    auto iy2 = _indicies[3];

    auto lambda = parameters_current_vals[lambda_ind];
    auto x1 = parameters_current_vals[ix1];
    auto y1 = parameters_current_vals[iy1];
    auto x2 = parameters_current_vals[ix2];
    auto y2 = parameters_current_vals[iy2];
    auto d = _constraint->getValue().front();

    auto root = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    auto root3 = root*root*root;

    vect[lambda_ind] += root - d;
    vect[ix1] += lambda*(x1-x2) / root;
    vect[iy1] += lambda*(y1-y2) / root;
    vect[ix2] += lambda*(x2-x1) / root;
    vect[iy2] += lambda*(y2-y1) / root;

    if (abs(root - d) < 1e-3) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][ix1] += (x1-x2) / root;
    matrix[lambda_ind][iy1] += (y1-y2) / root;
    matrix[lambda_ind][ix2] += (x2-x1) / root;
    matrix[lambda_ind][iy2] += (y2-y1) / root;

    matrix[ix1][ix1] += lambda*(y1-y2)*(y1-y2) / root3;
    matrix[ix1][iy1] += -lambda*(x1-x2)*(y1-y2) / root3;
    matrix[ix1][ix2] += -lambda*(y1-y2)*(y1-y2) / root3;
    matrix[ix1][iy2] += lambda*(x1-x2)*(y1-y2) / root3;
    matrix[ix1][lambda_ind] += (x1-x2) / root;

    matrix[iy1][ix1] += -lambda*(x1-x2)*(y1-y2) / root3;
    matrix[iy1][iy1] += lambda*(x1-x2)*(x1-x2) / root3;
    matrix[iy1][ix2] += lambda*(x1-x2)*(y1-y2) / root3;
    matrix[iy1][iy2] += -lambda*(x1-x2)*(x1-x2) / root3;
    matrix[iy1][lambda_ind] += (y1-y2) / root;

    matrix[ix2][ix1] += -lambda*(y1-y2)*(y1-y2) / root3;
    matrix[ix2][iy1] += lambda*(x1-x2)*(y1-y2) / root3;
    matrix[ix2][ix2] += lambda*(y1-y2)*(y1-y2) / root3;
    matrix[ix2][iy2] += -lambda*(x1-x2)*(y1-y2) / root3;
    matrix[ix2][lambda_ind] += -(x1-x2) / root;

    matrix[iy2][ix1] += lambda*(x1-x2)*(y1-y2) / root3;
    matrix[iy2][iy1] += -lambda*(x1-x2)*(x1-x2) / root3;
    matrix[iy2][ix2] += -lambda*(x1-x2)*(y1-y2) / root3;
    matrix[iy2][iy2] += lambda*(x1-x2)*(x1-x2) / root3;
    matrix[iy2][lambda_ind] += (y2-y1) / root;
}

void Solver::addParallelConstraint(Constraint* _constraint, QList<unsigned> _indicies) {

    auto lambda_ind = getLambdaIndex(_constraint);
    auto ix11 = _indicies[0];
    auto iy11 = _indicies[1];
    auto ix12 = _indicies[2];
    auto iy12 = _indicies[3];
    auto ix21 = _indicies[4];
    auto iy21 = _indicies[5];
    auto ix22 = _indicies[6];
    auto iy22 = _indicies[7];

    auto lambda = parameters_current_vals[lambda_ind];
    auto x11 = parameters_current_vals[ix11];
    auto y11 = parameters_current_vals[iy11];
    auto x12 = parameters_current_vals[ix12];
    auto y12 = parameters_current_vals[iy12];
    auto x21 = parameters_current_vals[ix21];
    auto y21 = parameters_current_vals[iy21];
    auto x22 = parameters_current_vals[ix22];
    auto y22 = parameters_current_vals[iy22];

    vect[lambda_ind] += (x11-x12)*(y21-y22) - (x21-x22)*(y11-y12);
    vect[ix11] += lambda*(y21-y22);
    vect[iy11] += lambda*(x22-x21);
    vect[ix12] += lambda*(y22-y21);
    vect[iy12] += lambda*(x21-x22);
    vect[ix21] += lambda*(y12-y11);
    vect[iy21] += lambda*(x11-x12);
    vect[ix22] += lambda*(y11-y12);
    vect[iy22] += lambda*(x12-x11);

    if (abs((x11-x12)*(y21-y22) - (x21-x22)*(y11-y12)) < 1e-3) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][ix11] += (y21-y22);
    matrix[lambda_ind][iy11] += (x22-x21);
    matrix[lambda_ind][ix12] += (y22-y21);
    matrix[lambda_ind][iy12] += (x21-x22);
    matrix[lambda_ind][ix21] += (y12-y11);
    matrix[lambda_ind][iy21] += (x11-x12);
    matrix[lambda_ind][ix22] += (y11-y12);
    matrix[lambda_ind][iy22] += (x12-x11);
    matrix[ix11][iy21] += lambda;
    matrix[ix11][iy22] += -lambda;
    matrix[ix11][lambda_ind] += (y21-y22);
    matrix[iy11][ix21] += -lambda;
    matrix[iy11][ix22] += lambda;
    matrix[iy11][lambda_ind] += x22-x21;
    matrix[ix12][iy21] += -lambda;
    matrix[ix12][iy22] += lambda;
    matrix[ix12][lambda_ind] += (y22-y21);
    matrix[iy12][ix21] += lambda;
    matrix[iy12][ix22] += -lambda;
    matrix[iy12][lambda_ind] += x21-x22;
    matrix[ix21][iy11] += -lambda;
    matrix[ix21][iy12] += lambda;
    matrix[ix21][lambda_ind] += y12-y11;
    matrix[iy21][ix11] += lambda;
    matrix[iy21][ix12] += -lambda;
    matrix[iy21][lambda_ind] += x11-x12;
    matrix[ix22][iy11] += lambda;
    matrix[ix22][iy12] += -lambda;
    matrix[ix22][lambda_ind] += y11-y12;
    matrix[iy22][ix11] += -lambda;
    matrix[iy22][ix12] += lambda;
    matrix[iy22][lambda_ind] += x12-x11;
}

void Solver::addPerpendicularConstraint(Constraint* _constraint, QList<unsigned> _indicies) {
    auto lambda_ind = getLambdaIndex(_constraint);
    auto ix11 = _indicies[0];
    auto iy11 = _indicies[1];
    auto ix12 = _indicies[2];
    auto iy12 = _indicies[3];
    auto ix21 = _indicies[4];
    auto iy21 = _indicies[5];
    auto ix22 = _indicies[6];
    auto iy22 = _indicies[7];

    auto lambda = parameters_current_vals[lambda_ind];
    auto x11 = parameters_current_vals[ix11];
    auto y11 = parameters_current_vals[iy11];
    auto x12 = parameters_current_vals[ix12];
    auto y12 = parameters_current_vals[iy12];
    auto x21 = parameters_current_vals[ix21];
    auto y21 = parameters_current_vals[iy21];
    auto x22 = parameters_current_vals[ix22];
    auto y22 = parameters_current_vals[iy22];

    vect[lambda_ind] += (x11-x12)*(x21-x22)+(y11-y12)*(y21-y22);
    vect[ix11] += lambda*(x21-x22);
    vect[iy11] += lambda*(y21-y22);
    vect[ix12] += lambda*(-x21+x22);
    vect[iy12] += lambda*(-y21+y22);
    vect[ix21] += lambda*(x11-x12);
    vect[iy21] += lambda*(y11-y12);
    vect[ix22] += lambda*(-x11+x12);
    vect[iy22] += lambda*(-y11+y12);

    if (abs((x11-x12)*(x21-x22)+(y11-y12)*(y21-y22)) < 1e-3) {
        vect[lambda_ind] = 0.;
    }

    matrix[lambda_ind][ix11] += (x21-x22);
    matrix[lambda_ind][iy11] += (y21-y22);
    matrix[lambda_ind][ix12] += (-x21+x22);
    matrix[lambda_ind][iy12] += (-y21+y22);
    matrix[lambda_ind][ix21] += (x11-x12);
    matrix[lambda_ind][iy21] += (y11-y12);
    matrix[lambda_ind][ix22] += (-x11+x12);
    matrix[lambda_ind][iy22] += (-y11+y12);
    matrix[ix11][ix21] += lambda;
    matrix[ix11][ix22] += -lambda;
    matrix[ix11][lambda_ind] += (x21-x22);
    matrix[iy11][iy21] += lambda;
    matrix[iy11][iy22] += -lambda;
    matrix[iy11][lambda_ind] += (y21-y22);
    matrix[ix12][ix21] += -lambda;
    matrix[ix12][ix22] += lambda;
    matrix[ix12][lambda_ind] += (-x21+x22);
    matrix[iy12][iy21] += -lambda;
    matrix[iy12][iy22] += lambda;
    matrix[iy12][lambda_ind] += (-y21+y22);
    matrix[ix21][ix11] += lambda;
    matrix[ix21][ix12] += -lambda;
    matrix[ix21][lambda_ind] += (x11-x12);
    matrix[iy21][iy11] += lambda;
    matrix[iy21][iy12] += -lambda;
    matrix[iy21][lambda_ind] += (y11-y12);
    matrix[ix22][ix11] += -lambda;
    matrix[ix22][ix12] += lambda;
    matrix[ix22][lambda_ind] += (-x11+x12);
    matrix[iy22][iy11] += -lambda;
    matrix[iy22][iy12] += lambda;
    matrix[iy22][lambda_ind] += (-y11+y12);
}

void Solver::setRebuild(bool _arg)
{
    on_rebuild = _arg;
}

bool Solver::isRebuild()
{
    return on_rebuild;
}

Solver::MatrixInstances Solver::instantTypeOnIndex(unsigned _index)
{
    for (const auto& ind_list: mapper.values()) {
        if (std::find(ind_list.begin(), ind_list.end(), _index) != ind_list.end()) {
            return MatrixInstances::Object;
        }
    }

    for (const auto& ind: const_mapper.values()) {
        if (_index == ind) {
            return MatrixInstances::Constraint;
        }
    }

    return MatrixInstances::None;
}


#ifdef ENABLE_DEBUG
void Solver::debugStateOutput()
{
    qWarning() << "Rebuild state: " << isRebuild();
    qWarning() << "Mapper:";
    for (const auto& _key: mapper.keys()) {
        qWarning() << _key << " -> " << mapper[_key];
    }
    qWarning();
    qWarning() << "Const_mapper:";
    for (const auto& _key: const_mapper.keys()){
        qWarning() << _key << " -> " << const_mapper[_key];
    }

    exit(0);
}

void Solver::debugPrintMatrix(const std::vector<std::vector<double>>& M) {
    QDebug deb = qDebug();

    for (unsigned i=0; i!=M.size(); ++i) {
        for (unsigned j=0; j!=M[i].size(); ++j) {
            deb << M[i][j] << " ";
        }
        deb << "\n";
    }
}

void Solver::debugPrintVector(const std::vector<double>& V) {
    QDebug dbg = qDebug();

    for (unsigned i=0; i!=V.size(); ++i) {
        dbg << V[i];
    }
}
#endif


//additional
double norm(const std::vector<double> &vect)
{
    double result = 0.;

    for (auto& el: vect) {
        result += el*el;
    }

    return sqrt(result);
}

std::vector<double> operator+(const std::vector<double>& op1, const std::vector<double>& op2) {
    std::vector<double> result = op1;

    for (unsigned i=0; i!=op1.size(); ++i) {
        result[i] = op1[i] + op2[i];
    }

    return result;
}
