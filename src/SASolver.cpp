#include "SASolver.h"

bool SASolver::solve(const TSP& tsp) {
    auto start = std::chrono::system_clock::now();
    TSPSolution bestSol = initialNeighborSolution(tsp);
    double bestVal = evaluate(tsp, bestSol);
    double temp = T;  

    for (int multistart = 0; multistart < 5; ++multistart) {
        //Random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        
        //Create a initial solution
        TSPSolution currentSol(tsp.getSize());
        double currentVal = 0;

        if (multistart == 0) {
            currentSol = bestSol;
            currentVal = bestVal;
        } else if (multistart == 1) {
            currentSol = cheapestInsertionSolution(tsp);
            currentVal = evaluate(tsp, currentSol);
        } else {
            currentSol.randomize();
            currentVal = evaluate(tsp, currentSol);
        }
        //Set as the best solution for this multistart
        TSPSolution localBestSol(currentSol);
        double localBestVal = currentVal;

        int not_improving = 0;

        T = temp;

        for (int i = 0; i < max_iter; i++) {
            //Get start and end of the segment to swap/2opt
            std::uniform_int_distribution<int> dist_start(1, tsp.getSize() - 3);
            int start = dist_start(gen);
            std::uniform_int_distribution<int> dist_end(start + 1, tsp.getSize() - 2);
            int end = dist_end(gen);

            TSPSolution newSol(currentSol);
            double newVal = 0;

            std::uniform_real_distribution<double> prob_dist(0, 1);
            if (prob_dist(gen) < p_2_opt) { 
                newSol = twoOptMove(currentSol, start, end);
                newVal = evaluate(tsp, currentVal, currentSol, start, end);
            }else if(prob_dist(gen) < 0.6) {
                threeOptMove(newSol);
                newVal = evaluate(tsp, newSol);
            }else {
                swapMove(newSol, start, end);
                newVal = evaluate(tsp, newSol);
            }
            
            //LOG
            std::cout << "Multistart: " << multistart << " | Iteration: " << i << " | New value: " << newVal << " | Path: " << newSol.toString() << std::endl;

            if (newVal < localBestVal) {
                localBestSol = newSol;
                localBestVal = newVal;
                not_improving = 0;
                currentSol = newSol;
                currentVal = newVal;
            } else {
                double loss = std::max(0.0, newVal - currentVal);
                double acceptance_prob = exp(-loss/T);
                std::uniform_real_distribution<double> acc_dist(0, 1);
                if (acc_dist(gen) < acceptance_prob) {
                    currentSol = newSol;
                    currentVal = newVal;
                }
                not_improving++;
            }
            
            T = T * cooling_rate;

            //Stop condition
            if (not_improving > not_improving_limit) break;
        }

        if (localBestVal < bestVal) {
            bestSol = localBestSol;
            bestVal = localBestVal;
        }
        std::cout<< "Multistart: " << multistart << " | Best value: " << bestVal << std::endl;
    }

    path_solution = bestSol.getPathSolution();
    auto end = std::chrono::system_clock::now();
    solving_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Best solution found with value: " << bestVal << std::endl << "Total solving time: " << solving_time << " seconds." << std::endl;
    return true;
}

TSPSolution SASolver::twoOptMove(TSPSolution& sol, int start, int end) {
    TSPSolution new_sol(sol);
    std::vector<int> path = new_sol.getPathSolution();
    if (start >= 1 && end < path.size() - 1 && start < end) 
        std::reverse(path.begin() + start, path.begin() + end + 1);
    new_sol.setPathSolution(path);
    return new_sol;
}

void SASolver::threeOptMove(TSPSolution& sol) {
    std::vector<int> path = sol.getPathSolution();
    int n = path.size();

    std::random_device rd;
    std::mt19937 gen(rd());

    // Select three points: a < b < c
    std::uniform_int_distribution<int> dist(1, n - 3);
    int a = dist(gen);
    int b = std::uniform_int_distribution<int>(a + 1, n - 2)(gen);
    int c = std::uniform_int_distribution<int>(b + 1, n - 1)(gen);

    // 7 possible moves 
    int choice = std::uniform_int_distribution<int>(0, 6)(gen);
    std::vector<int> new_path;

    switch (choice) {
        case 0: //[0 ... a-1] [b-1 ... a] [b ... c-1] [c ... n-1]
            std::reverse(path.begin() + a, path.begin() + b);
            break;
        case 1:  // [0 ... a-1] [a ... b-1] [c-1 ... b] [c ... n-1]
            std::reverse(path.begin() + b, path.begin() + c);
            break;
        case 2: //  [0 ... a-1] [c-1 ... a] [c ... n-1]
            std::reverse(path.begin() + a, path.begin() + c);
            break;
        case 3: // [0 ... a-1] [b ... c-1] [a ... b-1] [c ... n-1]
            new_path.insert(new_path.end(), path.begin(), path.begin() + a);
            new_path.insert(new_path.end(), path.begin() + b, path.begin() + c);
            new_path.insert(new_path.end(), path.begin() + a, path.begin() + b);
            new_path.insert(new_path.end(), path.begin() + c, path.end());
            path = new_path;
            break;
        case 4: // [0 ... a-1] [b-1 ... a] [c-1 ... b] [c ... n-1]
            std::reverse(path.begin() + a, path.begin() + b);
            std::reverse(path.begin() + b, path.begin() + c);
            break;
        case 5: // [0 ... a-1] [c-1 ... b] [a ... b-1] [c ... n-1]
            std::reverse(path.begin() + a, path.begin() + c);
            std::reverse(path.begin() + b, path.begin() + c);
            break;
        case 6: // [0 ... a-1] [b ... c-1] [b-1 ... a] [c ... n-1]
            std::reverse(path.begin() + a, path.begin() + b);
            std::reverse(path.begin() + b, path.begin() + c);
            std::reverse(path.begin() + a, path.begin() + c);
            break;
    }

    sol.setPathSolution(path);
}

void SASolver::swapMove(TSPSolution& sol, int i, int j) {
    std::vector<int> path = sol.getPathSolution();
    std::swap(path[i], path[j]);
    sol.setPathSolution(path);
}

double SASolver::evaluate(const TSP& tsp, const TSPSolution& sol) const {
    double cost = 0;
    std::vector<int> path = sol.getPathSolution();
    std::vector<std::vector<double> > costs = tsp.getCosts();
    for (int i = 0; i < path.size() - 1; i++) {
        cost += costs[path[i]][path[i+1]];
    }
    return cost;
}
double SASolver::evaluate(const TSP& tsp, double old_val, const TSPSolution& old_sol, int start, int end) const {
    std::vector<int> path = old_sol.getPathSolution();
    std::vector<std::vector<double> > costs = tsp.getCosts();
    double cost = old_val 
                - costs[path[start-1]][path[start]] 
                - costs[path[end]][path[end+1]] 
                + costs[path[start-1]][path[end]] 
                + costs[path[start]][path[end+1]];
    return cost;
}

TSPSolution SASolver::initialNeighborSolution(const TSP& tsp) {
    int n = tsp.getSize();
    std::vector<std::vector<double> > costs = tsp.getCosts();
    std::vector<bool> visited(n, false);
    std::vector<int> path(n + 1);
    path[0] = 0;
    visited[0] = true;
    for (int i = 1; i < n; i++) {
        int last = path[i-1];
        int next = -1;
        double min_cost = std::numeric_limits<double>::max();
        for (int j = 0; j < n; j++) {
            if (!visited[j] && costs[last][j] < min_cost) {
                min_cost = costs[last][j];
                next = j;
            }
        }
        path[i] = next;
        visited[next] = true;
    }
    path[n] = 0;
    return TSPSolution(path);
}

TSPSolution SASolver::cheapestInsertionSolution(const TSP& tsp) {
    int n = tsp.getSize();
    std::vector<std::vector<double>> costs = tsp.getCosts();
    std::vector<int> path = {0, 1, 0}; 
    std::vector<bool> visited(n, false);
    visited[0] = true;
    visited[1] = true;
    
    for (int i = 2; i < n; i++) {
        int bestNode = -1;
        int bestPos = -1;
        double minInsertionCost = std::numeric_limits<double>::max();
        
        for (int j = 0; j < n; j++) {
            if (visited[j]) continue;
            
            for (int pos = 1; pos < path.size(); pos++) {
                int prev = path[pos - 1];
                int next = path[pos];
                double insertionCost = costs[prev][j] + costs[j][next] - costs[prev][next];
                
                if (insertionCost < minInsertionCost) {
                    bestNode = j;
                    bestPos = pos;
                    minInsertionCost = insertionCost;
                }
            }
        }
        
        path.insert(path.begin() + bestPos, bestNode);
        visited[bestNode] = true;
    }
    return TSPSolution(path);
}