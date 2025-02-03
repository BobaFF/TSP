#include "CplexSolver.h"
#include <cstdio>

void CplexSolver::setupLP(CEnv env, Prob lp, const TSP &tsp)
{
    if (env == NULL || lp == NULL)
    {
        std::cerr << "Error: env or lp not initialized." << std::endl;
        return;
    }
    std::vector<std::vector<double> > costs = tsp.getCosts();
    n = tsp.getSize();

    int cur_index = 0;
    double t1, t2;
    CPXgettime(env, &t1);
    std::cout << "Creating variables..." << std::endl;
    // VAR Y
    {
        // 1. Prepare data
        int ccnt = n * n;
        std::vector<char> ytypes(n * n, 'B');
        std::vector<double> lbs(n * n, 0);
        std::vector<double> ubs(n * n, 1);
        std::vector<double> obj(n * n);
        std::vector<char *> colnames(n * n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                obj[n * i + j] = costs[i][j];
                std::string var_name = "y_" + std::to_string(i) + "_" + std::to_string(j);
                colnames[n * i + j] = cpxString(var_name);
                add_index_to_map('y', i, j, cur_index++);
            }
        }
        // 2. Insert data
        CHECKED_CPX_CALL(CPXnewcols, env, lp, ccnt, obj.data(), lbs.data(), ubs.data(), ytypes.data(), colnames.data());

        //3. Free memory on the heap
        deallocateVectorPointer(colnames);
    }

    // VAR X
    {
        // 1. Prepare data
        int ccnt = n * n;
        std::vector<char> xtypes(n * n, 'C');
        std::vector<double> lbs(n * n, 0);
        std::vector<double> ubs(n * n, CPX_INFBOUND);
        std::vector<double> obj(n * n, 0);
        std::vector<char *> colnames(n * n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::string var_name = "x_" + std::to_string(i) + "_" + std::to_string(j);
                colnames[n * i + j] = cpxString(var_name);
                add_index_to_map('x', i, j, cur_index++);
            }
        }
        // 2. Insert data
        CHECKED_CPX_CALL(CPXnewcols, env, lp, ccnt, obj.data(), lbs.data(), ubs.data(), xtypes.data(), colnames.data());

        //3. Free memory on the heap
        deallocateVectorPointer(colnames);
    }

    std::cout << "Variables Created!" << std::endl
              << "Creating row constraints.." << std::endl;
    
    // ROWS

    // sum_i (x_ik) - sum_j (x_kj) = 1 forall  k \ {0}
    {
        //1. Prepare data
        int rowcnt = n - 1;
        int nzcnt = rowcnt*(n - 1 + n - 2);
        std::vector<double> rhs(rowcnt, 1);
        std::vector<char> sense(rowcnt, 'E');
        std::vector<int> rmatbeg(rowcnt);
        std::vector<int> rmatind(nzcnt);
        std::vector<double> rmatval(nzcnt);
        std::vector<char *> rownames(rowcnt);
        
        int elements_in_row = (n - 1 + n - 2);
        for (int k = 1; k < n; k++) {
            int row_index = k - 1;
            rmatbeg[row_index] = row_index * elements_in_row;
            rownames[row_index] = cpxString("(2)_" + std::to_string(k));
            int index = 0;
            for(int i = 0; i < n; i++) {
                if (i == k)
                    continue;
                rmatind[row_index * elements_in_row + index] = get_index_from_map('x', i, k);
                rmatval[row_index * elements_in_row + index] = 1;
                index++;
            }
            for(int j = 1; j < n; j++) {
                if (j == k) 
                    continue;
                rmatind[row_index * elements_in_row + index] = get_index_from_map('x', k, j);
                rmatval[row_index * elements_in_row + index] = -1;
                index++;
            }
        }
        //2. Insert data
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, rowcnt, nzcnt, rhs.data(), sense.data(), rmatbeg.data(), rmatind.data(), rmatval.data(), nullptr, rownames.data());

        //3. Free memory on the heap
        deallocateVectorPointer(rownames);
    }

    // sum_j (y_ij) = 1 for all i   
    {
        //1. Prepare data
        int rowcnt = n;
        int nzcnt = n*n;
        std::vector<double> rhs(rowcnt, 1);
        std::vector<char> sense(rowcnt, 'E');
        std::vector<int> rmatbeg(rowcnt, 0);
        std::vector<int> rmatind(nzcnt);
        std::vector<double> rmatval(nzcnt, 1);
        std::vector<char *> rownames(rowcnt);
        for (int i = 0 ; i < n; i++) {
            rmatbeg[i] = i*n;
            rownames[i] = cpxString("(3)_" + std::to_string(i));
            for (int j = 0; j < n; j++) {
                rmatind[i*n + j] = get_index_from_map('y', i, j);
            }
        }
        //2. Insert data
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, rowcnt, nzcnt, rhs.data(), sense.data(), rmatbeg.data(), rmatind.data(), rmatval.data(), nullptr, rownames.data());

        //3. Free memory on the heap
        deallocateVectorPointer(rownames);
    }

    // sum_i (y_ij) = 1 for all j
    {   
        //1. Prepare data
        int rowcnt = n;
        int nzcnt = n*n;
        std::vector<double> rhs(rowcnt, 1);
        std::vector<char> sense(rowcnt, 'E');
        std::vector<int> rmatbeg(rowcnt, 0);
        std::vector<int> rmatind(nzcnt);
        std::vector<double> rmatval(nzcnt, 1);
        std::vector<char *> rownames(rowcnt);
        for (int j = 0 ; j < n; j++) {
            rmatbeg[j] = j*n;
            rownames[j] = cpxString("(4)_" + std::to_string(j));
            for (int i = 0; i < n; i++) {
                rmatind[j*n + i] = get_index_from_map('y', i, j);
            }
        }
        //2. Insert data
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, rowcnt, nzcnt, rhs.data(), sense.data(), rmatbeg.data(), rmatind.data(), rmatval.data(), nullptr, rownames.data());

        //3. Free memory on the heap
        deallocateVectorPointer(rownames);
    }

    // x_ij - y_ij * (N - 1) <= 0 for all i, j \ {0}
    {
        //1. Prepare data
        int rowcnt = n *(n - 1);
        int nzcnt = 2*rowcnt;
        std::vector<double> rhs(rowcnt, 0);
        std::vector<char> sense(rowcnt, 'L');
        std::vector<int> rmatbeg(rowcnt);
        std::vector<int> rmatind(nzcnt);
        std::vector<double> rmatval(nzcnt);
        std::vector<char *> rownames(rowcnt);

        int row = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 1; j < n; j++) {
                rmatbeg[row] = row * 2;
                rmatind[row*2] = get_index_from_map('x', i, j);
                rmatval[row*2] = 1;
                rmatind[row*2 + 1] = get_index_from_map('y', i, j);
                rmatval[row*2 + 1] = -(n - 1);
                rownames[row] = cpxString("(5)_" + std::to_string(i) + "_" + std::to_string(j));
                row++;
            }
        }
        //2. Insert data
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, rowcnt, nzcnt, rhs.data(), sense.data(), rmatbeg.data(), rmatind.data(), rmatval.data(), nullptr, rownames.data());

        //3. Free memory on the heap
        deallocateVectorPointer(rownames);
    }
    CPXgettime(env, &t2);
    setup_time = (double)(t2 - t1);
    std::cout << "Rows created! Total time to setup the problem: " << setup_time<< std::endl;
}

bool CplexSolver::solve(const TSP &tsp)
{
    std::cout << "Starting CplexSolver for TSP file: " << tsp.getFileName() << std::endl;
    try
    {
        DECL_ENV(env);
        DECL_PROB(env, lp);
        
        if (time_limit > 1e-06)
            CPXsetdblparam(env, CPX_PARAM_TILIM, time_limit);
        setupLP(env, lp, tsp);

        CHECKED_CPX_CALL(CPXwriteprob, env, lp, "prob.lp", NULL);
        double t1,t2;
        CPXgettime(env, &t1);
        // Set problem to min
        CHECKED_CPX_CALL(CPXmipopt, env, lp);
        CPXgettime(env, &t2);
        solving_time = (double)(t2 - t1);
        
        //verificare status
        int status = CPXgetstat(env, lp);
        std::cout << "Status: " << status << std::endl;
        if (status == CPXMIP_OPTIMAL || status == CPXMIP_OPTIMAL_TOL)
        {
            solution_type = "Optimal";
            std::cout << "Optimal solution found!" << std::endl;
        }
        else if (status == CPXMIP_TIME_LIM_FEAS)
        {
            solution_type = "Feasible";
            std::cout << "Time limit reached!" << std::endl;
        }
        else if (status == CPXMIP_INFEASIBLE)
        {
            solution_type = "Infeasible";
            std::cout << "Problem infesible" << std::endl;
        }
        else
        {
            solution_type = "Unknown";
            std::cout << "Unknown status" << std::endl;
        }

        iter = CPXgetmipitcnt(env, lp);

        // Get objval
        CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);

        int n = CPXgetnumcols(env, lp);
        varvals.resize(n);
        int fromIdx = 0;
        int toIdx = vars_map.size() - 1;
        CHECKED_CPX_CALL(CPXgetx, env, lp, &varvals[0], fromIdx, toIdx);

        createPathSolution();
        // Free memory
        CPXfreeprob(env, &lp);
        CPXcloseCPLEX(&env);

        //Log results
        std::cout << "Objective value: " << objval << std::endl;
        std::cout << "Total solving time: " <<solving_time << std::endl;
        std::cout << "Total iterations: " << iter << std::endl;
        TSPSolution sol(path_solution);
        sol.print();

        return 1;
    }
    catch (std::exception &e)
    {
        std::cout << ">>> EXCEPTION: " << e.what() << std::endl;
        return 0;
    }
}

double CplexSolver::getObjVal() const
{
    return objval;
}

char *CplexSolver::cpxString(const std::string &s)
{
    char *stringPtr = new char[s.size() + 1];
    std::strcpy(stringPtr, s.c_str());
    return stringPtr;
}
void CplexSolver::deallocateVectorPointer(std::vector<char *> v) {
    for (char *ptr : v) {
        delete[] ptr;
    }
}

void CplexSolver::add_index_to_map(char var, int i, int j, int value)
{
    if (var != 'x' && var != 'y')
        return;
    vars_map[std::tuple<char, int, int>(var, i, j)] = value;
}

int CplexSolver::get_index_from_map(char var, int i, int j) const {
    if (var != 'x' && var != 'y')
        return -1;
    auto it = vars_map.find(std::tuple<char, int, int>(var, i, j));
    if (it != vars_map.end())
        return it->second;
    return -1;
}



void CplexSolver::createPathSolution()
{   
    int i = 0;
    int nodes = 1;
    path_solution.push_back(0);
    while (nodes < n)
    {
        for (int j = 0; j < n; j++)
        {
            if (varvals[get_index_from_map('y', i, j)] > 1e-06)
            {
                path_solution.push_back(j);
                i = j;
                nodes++;
                break;
            }
        }
    }
    path_solution.push_back(0);
}

std::string CplexSolver::getStatus() const
{
    return solution_type;
}

double CplexSolver::getSetupTime() const
{
    return setup_time;
}