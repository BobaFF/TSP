#ifndef SASOLVER_H
#define SASOLVER_H
#include "TSPSolver.h"
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>

class SASolver: public TSPSolver {
    private:
        int max_iter;
        double cooling_rate;
        double T;
        int not_improving_limit;
        double p_2_opt;
        TSPSolution twoOptMove(TSPSolution&, int, int);
        void threeOptMove(TSPSolution&);
        void swapMove(TSPSolution&, int, int);
        /* Generates initial solution based on NN heuristic*/
        TSPSolution initialNeighborSolution(const TSP&);
        /* Generates initial solution based on Cheapest Insertion heuristic*/
        TSPSolution cheapestInsertionSolution(const TSP&);
    public:
        SASolver(int k, double c, double t, double l): max_iter(k), cooling_rate(c), T(t), not_improving_limit(l) {
            switch (n) {
            case 10:
                p_2_opt = 0.2;
                break;
            case 25 || 50:
                p_2_opt = 0.15;
                break;
            default:
                p_2_opt = 0.1;
                break;
            }
        };
        bool solve (const TSP&) override final;
        /*Evaluates a TSP solution using cost matrix*/
        double evaluate(const TSP&, const TSPSolution&) const;
        /*Evaluates a TSP solution after 2opt*/
        double evaluate(const TSP&, double, const TSPSolution&, int, int) const;

};


#endif