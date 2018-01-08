#ifndef SOLVER_H
#define SOLVER_H

#ifdef ENABLE_DEBUG
#include <QDebug>
#endif

class Solver
{
public:
    Solver();

    void resolve() { qWarning() << "Resolved!"; }
};

#endif // SOLVER_H
