#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include "TSP.h"
#include "TSPSolution.h"

/* Solver for the TSP problem*/
class TSPSolver {
    protected:
        int iter;
        int n;
        double solving_time;
        std::vector<int> path_solution;
    public:
        TSPSolver() {
            path_solution.clear();
            solving_time = 0;
            n = 0;
        };
        virtual bool solve(const TSP&) = 0;
        double getSolvingTime() const;
        TSPSolution getSolution() const;
};

#endif