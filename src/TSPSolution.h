#ifndef TSPSOLUTION_H
#define TSPSOLUTION_H
#include "TSP.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

/* Path solution for the TSP problem*/
class TSPSolution {
    private:
        std::vector<int> path_solution;
    public:
        TSPSolution(int n);
        TSPSolution(std::vector<int> path_solution):  path_solution(path_solution) {};
        void randomize();
        std::vector<int> getPathSolution() const;
        void setPathSolution(std::vector<int>);
        void print() const;
        std::string toString() const;
};

#endif