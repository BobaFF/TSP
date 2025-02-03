#include "TSPSolver.h"

double TSPSolver::getSolvingTime() const
{
    return solving_time;
}

TSPSolution TSPSolver::getSolution() const {
    return TSPSolution(path_solution);
}