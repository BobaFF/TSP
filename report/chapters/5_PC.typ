#import "../tools.typ": *
= Performances comparison
#fig("Performances-comp", "Table 9") compares the performances of the CPlex solver and the SA algorithm. The table shows that the CPlex implementation is always able to find the optimal solution, while the SA algorithm is not. However, the SA algorithm is able to find a 
resonable solution in small amount of time, even for the largest instances. The table also shows that the SA algorithm is able to find a solution that is close to the optimal one, with a relative error of less than  $approx 10%$ for all instances. 

#figure(performances_comp(
  "data/C-SA.csv"
), caption: "Performances comparison of SA and CPlex Solver") <Performances-comp>

#pagebreak()