#include "TSPSolution.h"

TSPSolution::TSPSolution(int n) {
    path_solution.resize(n + 1);
    path_solution[0] = 0;
    for (int i = 1; i < n; i++)
        path_solution[i] = i;
    path_solution[n] = 0;
}

void TSPSolution::randomize() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(path_solution.begin() + 1, path_solution.end() - 1, g);
}

std::vector<int> TSPSolution::getPathSolution() const {
    return path_solution;
}

void TSPSolution::setPathSolution(std::vector<int> new_path) {
    path_solution = new_path;
}
void TSPSolution::print() const {
    std::cout << "Path solution: < ";
    for (int i = 0; i < path_solution.size(); i++) {
        std::cout << path_solution[i] << " ";
    }
    std::cout << ">";
    std::cout << std::endl;
}

std::string TSPSolution::toString() const {
    std::string s_path_solution = "";
    for (int i = 0; i < path_solution.size(); i++) {
        s_path_solution = s_path_solution + std::to_string(path_solution[i]) + " ";
    }
    return s_path_solution;
}
