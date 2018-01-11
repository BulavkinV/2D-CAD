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

        b = gauss(A);
        parameters_current_vals = parameters_current_vals + b;
        qWarning() << "Result:" << norm(b);
        debugPrintVector(b);
        exit_cond = norm(b) >= tolerance;
        rebuild(_constraints);
    } while(exit_cond);

    //back mapping
    backMap();
    reset();
}

void Solver::rebuild(const QList<Constraint *> & _constraints) {
    for (auto& str: matrix) {
        str.assign(matrix.size(), 0.);
    }
    vect.assign(vect.size(), 0.);

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

std::vector<double> Solver::gauss(std::vector<std::vector<double>> A) {
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
    mapper.clear();
    const_mapper.clear();

    parameters_current_vals.clear();
    parameters_init_vals.clear();

    matrix.clear();
    vect.clear();
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

    //!!
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
    }

    return result;
}

unsigned Solver::addNewConstraint(Constraint* _constraint)
{
    unsigned result = matrix.size();

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
        parameters_init_vals.resize(matrix.size(), 1.);
        parameters_current_vals.resize(matrix.size(), 1.);
    }
    const_mapper[_constraint] = result;

    return result;
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
        case (ConstraintType::SamePoint): {
                addSamePointConstraint(_constraint, _indicies);
            }
            break;
        default:
            throw NotImplementedException();
    }
}

void Solver::addSamePointConstraint(Constraint* _constraint, QList<unsigned> _indicies) {
    unsigned lambda_ind;
    if (containsConstraint(_constraint)) {
        lambda_ind = getConstraintIndex(_constraint);
    }
    else {
        if (isRebuild()) {
            qWarning() << "Adding new constraint on rebuild!!!";
        }
        lambda_ind = addNewConstraint(_constraint);
    }


    auto x1 = parameters_current_vals[_indicies[0]];
    auto y1 = parameters_current_vals[_indicies[1]];
    auto x2 = parameters_current_vals[_indicies[2]];
    auto y2 = parameters_current_vals[_indicies[3]];

    auto ix1 = _indicies[0];
    auto iy1 = _indicies[1];
    auto ix2 = _indicies[2];
    auto iy2 = _indicies[3];

    vect[_indicies[0]] += -2.*parameters_current_vals[lambda_ind]*(parameters_current_vals[_indicies[2]]-parameters_current_vals[_indicies[0]]);
    vect[_indicies[1]] += -2.*parameters_current_vals[lambda_ind]*(parameters_current_vals[_indicies[3]]-parameters_current_vals[_indicies[1]]);
    vect[_indicies[2]] += 2.*parameters_current_vals[lambda_ind]*(parameters_current_vals[_indicies[2]]-parameters_current_vals[_indicies[0]]);
    vect[_indicies[3]] += 2.*parameters_current_vals[lambda_ind]*(parameters_current_vals[_indicies[3]]-parameters_current_vals[_indicies[1]]);
    vect[lambda_ind] += (parameters_current_vals[_indicies[2]] - parameters_current_vals[_indicies[0]]) * (parameters_current_vals[_indicies[2]] - parameters_current_vals[_indicies[0]])+
        (parameters_current_vals[_indicies[3]] - parameters_current_vals[_indicies[1]]) * (parameters_current_vals[_indicies[3]] - parameters_current_vals[_indicies[1]]);

    matrix[lambda_ind][ix1] += -2.*(x2-x1);
    matrix[lambda_ind][iy1] += -2.*(y2-y1);
    matrix[lambda_ind][ix2] += 2.*(x2-x1);
    matrix[lambda_ind][iy2] += 2.*(y2-y1);
    matrix[ix1][ix1] += 1.+2.*parameters_current_vals[lambda_ind];
    matrix[ix1][ix2] += -2.*parameters_current_vals[lambda_ind];
    matrix[ix1][lambda_ind] += -2.*(x2-x1);
    matrix[iy1][iy1] += 1.+2.*parameters_current_vals[lambda_ind];
    matrix[iy1][iy2] += -2.*parameters_current_vals[lambda_ind];
    matrix[iy1][lambda_ind] += -2.*(y2-y1);
    matrix[ix2][ix2] += 1.+2.*parameters_current_vals[lambda_ind];
    matrix[ix2][ix1] += -2.*parameters_current_vals[lambda_ind];
    matrix[ix2][lambda_ind] += 2.*(x2-x1);
    matrix[iy2][iy2] += 1.+2.*parameters_current_vals[lambda_ind];
    matrix[iy2][iy1] += -2.*parameters_current_vals[lambda_ind];
    matrix[iy2][lambda_ind] += 2.*(y2-y1);
}

void Solver::setRebuild(bool _arg)
{
    on_rebuild = _arg;
}

bool Solver::isRebuild()
{
    return on_rebuild;
}


#ifdef ENABLE_DEBUG
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
