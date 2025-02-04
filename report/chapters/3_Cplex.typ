#import "../tools.typ": *
= Part I: Cplex Solver
<cplex>
This section details the implementation of the TSP model using the CPLEX API for C++, in the class #code("CplexSolver") .
== MILP Model

*Sets:*\
- $N =$ graph nodes, representing the holes;

- $A = "arcs" (i, j), forall i, j in N$, representing the trajectory covered by the drill to move from hole $i$ to hole $j$

*Parameters:*\

- $c_(i j)$ = time taken by the drill to move from $i$ to $j$, $forall (i, j) in A$

- $0 =$ arbitrarily selected starting node, $0 in N$.

*Decision Variables:*\

- $ x_(i j)=$ amount of the flow shipped from $i "to" j, forall (i, j) in A$;

- $y_(i j) = 1 "if"  "arc"(i, j)$ ships some flow, $0$ otherwise, $forall (i, j) in A$

*Integer Linear Programming model:*\
$ 
min & sum_(i, j : (i, j) in A) c_(i j)y_(i j) &&#h(5cm) &&#h(3cm) && (1) \
s.t. & sum_(i : (i, k) in A) x_(i k) - sum_(j : (k, j), j != 0) x_(k j) && = 1 && forall k in N \\{0} #h(3cm) &&(2)\
 & sum_(j : (i, j) in A) y_(i j) && = 1 && forall i in N &&(3)\
 &sum_(i: (i, j) in A) y_(i j) && = 1 && forall j in N &&(4)\
 &x_(i j) &&<= (|N| - 1) y_(i j) &&forall (i, j) in A, j != 0 &&(5)\
 &x_(i j) in RR_+ && && forall (i, j) in A, j != 0 &&(6)\
 &y_(i j) in {0, 1} && && forall (i, j) in A &&(7)\
$
== Variables creation
Variables $x$ and $y$ are created using the #code("CPXnewcols") function with two calls, one for $x$ variables and one for $y$ variables. 

To keep a connection between the CPLEX variables representation, indexes are stored in a map, defined as follows: 
#align(center,
  code(
  "std::map<std::tuple<char, int, int>, int> vars_map;")
)
The #code("CplexSolver") class implements the following private method to store the indexes in the map, where #code("type") is the variable type, #code("i") and #code("j") are the indexes of the variable and #code("cur_index") is the current index of the variable in the CPLEX model:
#align(center,
  code("void add_index_to_map(char type, int i, int j, int cur_index)")
)

To retrieve the CPLEX index of a variable, the following private method is implemented:
#align(center,
  code("int get_index_from_map(char type, int i, int j)")
)


To efficiently insert variables in the CPLEX model:

+ vectors are created to define: variable types, lower and upper bounds, objective coefficients and variable names. For each variable, the corresponding index is stored in the map using the #code("add_index_to_map()") method.

+ The #code("CPXnewcols") function is called once to add all $y$  variables and then all $x$ variables to the CPLEX problem. This is more efficient than adding variables one by one.

+ Memory cleanup is performed: the allocated space for variables names (pointers to string) is released after the variables are added to the CPLEX model.




== Constraints creation
Similarly to the variables creation, the row constraints (2), (3), (4), (5) are added in a single call for each one of them: the #code("CPXaddrows") function is called once for each constraint type, adding all the constraints of that type to the CPLEX model. 

To efficiently insert constraints in the CPLEX model.
+ vectors are initialized to define: right-hand side values, the sense of the constraints, the indixes of the variables involved, and their coefficients.
+ The #code("CPXaddrows") function is called once to add all the constraints at once. This is more efficient than adding constraints one by one.

+ Memory cleanup is performed: the allocated space for the constraints names (pointers to string) is released after the constraints are added to the CPLEX model.

Let's take a look at the values of the vectors used to add the constraints (2), (3), (4), (5) to the CPLEX model.\

*Constraint (2)*
$ sum_(i : (i, k) in A) x_(i k) - sum_(j : (k, j), j != 0) x_(k j) = 1 #h(4em) forall k in N \\{0} $

- #code("rowcnt = n - 1") is the number of constaints of this type. In fact we are considering all the nodes except the starting one.

- #code("nzcnt = rowcnt * (2*n - 3)") is the number of non-zero coefficients in the constraint matrix. In fact, for each node $k$ we have $n-1$ variables $x_(i k)$ and $n-2$ variables $x_(k j)$, because $j != 0$.

- #code("vector <double> rhs(rowcnt, 1)") is the right-hand side values of the constraints. In this case, all the values are set to 1.

- #code("vector <char> sense(rowcnt, 'E')") is the sense of the constraints. In this case, all the constraints are equalities, identified by char 'E'.

- #code("vector <int> rmatbeg(rowcnt)") is the starting index of the non-zero coefficients in the constraint matrix. In this case, the starting index of each row is given by $k * (2*n - 3)$: in fact, at iteration $k$, we have already added $k * (n - 1)$ variables $x_(i k)$ and $k * (n - 2)$ variables $x_(k j)$.

- #code("vector <int> rmatind(nzcnt)") is the indexes of the variables involved in the constraints. The indexes are stored in the vector at each iteration using the #code("get_index_from_map()") method. An internal counter is used to keep track of the current index in the vector.

- #code("vector <double> rmatval(nzcnt, 0)") is the coefficients of the variables involved in the constraints. The coefficients are set to 1 for the variables $x_(i k)$ and -1 for the variables $x_(k j)$. As it happens for the indexes, the coefficients are stored in the vector using an internal counter to keep track of the current index.

- #code("vector <char*> rowname(rowcnt)") is the names of the constraints. The names are set using the #code("cpxString()") method which returns a pointer to a string after allocating the necessary space.

*Constraint (3)*
$ sum_(j : (i, j) in A) y_(i j)  = 1 #h(4em) forall i in N $

- #code("rowcnt = n ") , in fact we are creating a constraint for each node $i in N$.

- #code("nzcnt = rowcnt * n ") , in fact, for each  row $i$ we are summing $n$ variables $y$.

- #code("vector <double> rhs(rowcnt, 1)") : in this case, all the right hand sides are set to 1.

- #code("vector <char> sense(rowcnt, 'E')") all the constraints are equalities, identified by char 'E'.

- #code("vector <int> rmatbeg(rowcnt)"): in this case, the starting index of each row $i$ is given by $i * n$: in fact, at iteration $i$, we have already added $i * n$ variables $y_(i j)$.

- #code("vector <int> rmatind(nzcnt)") the indexes are stored in the vector at each iteration using #code("get_index_from_map()") , in particular the variable $y_(i j)$ is in the position $i * n + j$ of this vector.

- #code("vector <double> rmatval(nzcnt, 1)") the coefficients are set to 1 for all the variables $y_(i j)$ .

- #code("vector <char*> rowname(rowcnt)") : at each iteration $i$ a new pointer to a string is added to this vector.
*Constraint (4)*
$ sum_(i: (i, j) in A) y_(i j)  = 1 #h(4em) forall j in N $
- #code("rowcnt = n ") , in fact we are creating a constraint for each node $j in N$.

- #code("nzcnt = rowcnt * n ") , in fact, for each  row $j$ we are summing $n$ variables $y$.

- #code("vector <double> rhs(rowcnt, 1)") : in this case, all the right hand sides are set to 1.

- #code("vector <char> sense(rowcnt, 'E')") all the constraints are equalities, identified by char 'E'.

- #code("vector <int> rmatbeg(rowcnt)"): the starting index of each row $j$ is given by $j * n$: in fact, at iteration $j$, we have already added $j * n$ variables $y_(i j)$.

- #code("vector <int> rmatind(nzcnt)") the indexes are stored in the vector at each iteration using #code("get_index_from_map()") , in particular the variable $y_(i j)$ is in the position $j * n + i$ of this vector.

- #code("vector <double> rmatval(nzcnt, 1)") the coefficients are set to 1 for all the variables $y_(i j)$ .

- #code("vector <char*> rowname(rowcnt)") : at each iteration $j$ a new pointer to a string is added to this vector.

*Constraint (5)*
$ x_(i j) <= (|N| - 1) y_(i j) #h(3em) forall (i, j) in A, j != 0 $
Note that the constraint is added into the CPLEX model moving all the variables on the left-hand side:
$ x_(i j)  <= (|N| - 1) y_(i j) #h(1.5em) => #h(1.5em)
  x_(i j) - (|N| - 1) y_(i j)  <= 0 $

- #code("rowcnt = n * (n - 1) ") , in fact we are creating a constraint for each pair of arc, except for the ones to the starting node.

- #code("nzcnt = 2 * rowcnt") , in fact, for each  row $i j$ we are summing two variables.

- #code("vector <double> rhs(rowcnt, 0)") : all the right hand sides are set to 0.

- #code("vector <char> sense(rowcnt, 'L')") all the constraints are less or equal, identified by char 'L'.

- #code("vector <int> rmatbeg(rowcnt)"): the starting index of each row constraint $i j$ is given by $2 * r o w$, where $r o w$ is an internal counter to keep track of the number of row constraints added.

- #code("vector <int> rmatind(nzcnt)") the indexes are stored in the vector at each iteration using #code("get_index_from_map()") , in particular, at a given $r o w$ the variable $x_(i j)$ is in position $r o w * 2$ and the variable $y_(i j)$ is in position $r o w * 2 + 1$.

- #code("vector <double> rmatval(nzcnt)") the coefficients are set to 1 for all the variables $x_(i j)$, so the ones in position $r o w * 2$, and to $-(|N| - 1)$ for all the variables $y_(i j)$, so the ones in position $r o w * 2 + 1$.

- #code("vector <char*> rowname(rowcnt)") : at each iteration a new pointer to a string is added to this vector.


== Optimal path
The optimal path is retrieved from the CPLEX model after the optimization process is completed. Using the CPLEX function #code("CPXgetx()") the values of the variables are stored in a private member of the #code("CplexSolver") class:  #code("vector<double> varvals") .\ Then, the optimal path is reconstructed by following the values of the variables $y_(i j)$ using the private method #code("createPathSolution()") . It starts by setting the initial node as node 0 and adding it to the path. Then, it iteratively explores the subsequent nodes by checking, for each possible destination  $j$ , whether the variable  $y_(i j)$  indicates an active connection (when it is set to 1). When it finds a connected node  $j$ , it adds  $j$  to the path, updates the current node, and continues until all nodes have been visited. Once the tour is complete, it adds the starting node to the end of the path to represent the return to the starting point. In the end, the optimal path is stored in the #code("vector<int> path_solution") variable as an ordered sequence of nodes.

The optimal path is then returned by the public method #code("getPathSolution()") . This method is used to initialize a #code("TSPSolution") object, which is then used to print the path solution, like that:
#align(center,
  code(
  ">> Path solution: < 0 6 7 2 4 8 5 3 1 9 0 >", lan: none)
)

== Tests
All the tests have been made in the LabTA calculators from remote, using Ubuntu operative system. The time limits were set to 1, 10, 30, 60, 120, 300 seconds. A cycle was made to collect the results of the tests and store them in a csv file. The results are shown in the following #fig("testsCplex", "Table 1") and #fig("perfCplex", "Table 2") figures.

Note that it was not possible to find an optimal path solution for the dataset #code(lan: none, "TSP_100_2.dat") within the time limit of 300 seconds. 

#figure(tests_cplex("data/data_tsp.csv"), caption: [Cplex API Benchmarks]) <testsCplex>

The average performances for the optimal path setup and solving time are shown in the following #fig("perfCplex", "Table 2").

#figure(cplex_score("data/performance_tsp.csv"), caption: [Cplex API average performances]) <perfCplex>
#pagebreak()