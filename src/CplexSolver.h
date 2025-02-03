#ifndef CPLEXSOLVER_H
#define CPLEXSOLVER_H

#include "TSPSolver.h"
#include "cpxmacro.h"
#include "map"
#include "tuple"
#include <cstdio>

class CplexSolver: public TSPSolver {
    private:
        double time_limit;
        double setup_time;
        std::vector<double> varvals;
        double objval;
        std::string solution_type;
        /* Maps variables by their type and indexes to their Cplex index */
        std::map<std::tuple<char, int, int>, int> vars_map;

        /* Setups LP: env, vars and constraints */
        void setupLP(CEnv env, Prob lp, const TSP&);
        /* Returns a char* pointer to a string */
        char* cpxString(const std::string&);
        /* Deallocates a vector of char* */
        void deallocateVectorPointer(std::vector<char *>);
        /* Adds a new index to the map */
        void add_index_to_map(char, int , int , int );
        /* Returns the cplex index of a var given its type and indexes */
        int get_index_from_map(char, int, int) const;
        void createPathSolution();
        
        // error status and messagge buffer (from cpxmacro.h)
        int status;
        char errmsg[BUF_SIZE];


        
    public:
        CplexSolver(double time): TSPSolver(), time_limit(time) {
            varvals.clear();
            objval = 0;
            solution_type = "Unknown";
            setup_time = 0;
            solving_time = 0;
        };
        /* Solves a TSP problem*/
        bool solve(const TSP&) override final;
        /* Returns the objective value of a TSP problem*/
        double getObjVal() const;
        /* Returns the status of the found solution*/
        std::string getStatus() const;
        /* Return the time to setup the Cplex Solver */
        double getSetupTime() const;
};


#endif
