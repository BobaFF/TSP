#include "CplexSolver.h"
#include "SASolver.h"
#include "TSPSolution.h"
#include <iostream>

int main() {

    /* CPLEX SOLVER */
    CplexSolver CplexSolver(0);

    /* SIZE = 10 */
    TSP tsp10("../data/TSP_10_1.dat");
    CplexSolver.solve(tsp10);
    double Sol1 = CplexSolver.getObjVal();
    double time1 = CplexSolver.getSolvingTime();

    /* SIZE = 50 */
    TSP tsp50("../data/TSP_50_2.dat");
    CplexSolver.solve(tsp50);
    double Sol2 = CplexSolver.getObjVal();
    double time2 = CplexSolver.getSolvingTime();

    /* SIMULATED ANNEALING */

    /* SIZE = 10 */
    SASolver SASolver1(500, 0.99, 12, 200);
    SASolver1.solve(tsp10);
    TSPSolution Sol3 = SASolver1.getSolution();
    double time3 = SASolver1.getSolvingTime();

    /* SIZE = 50 */
    SASolver SASolver2(3000, 0.94, 100, 1000);
    SASolver2.solve(tsp50);
    TSPSolution Sol4 = SASolver2.getSolution();
    double time4 = SASolver2.getSolvingTime();

    /* PRINT RESULTS */
    std::cout << "\n\n****Size 10****" << std::endl;
    std::cout << "CPLEX SOLVER - ";
    std::cout << "Solution: " << Sol1 << " Time: " << time1 << std::endl;
    std::cout << "SIMULATED ANNEALING - ";
    std::cout << "Solution: " << SASolver1.evaluate(tsp10, Sol3) << " Time: " << time3 << std::endl;

    std::cout << "\n\n****Size 50****" << std::endl;

    std::cout << "CPLEX SOLVER - ";
    std::cout << "Solution: " << Sol2 << " Time: " << time2 << std::endl;

    std::cout << "SIMULATED ANNEALING - ";
    std::cout << "Solution: " << SASolver2.evaluate(tsp50, Sol4) << " Time: " << time4 << std::endl;

    return 0;
}