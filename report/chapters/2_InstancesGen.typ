#import "../tools.typ": *
= Instances generator
<instances-gen>
The instances were generated with the script #code("data_generator.py"). The script considers problems with 10, 25, 50, 75, and 100 points. For each problem size, it generates 9 examples. The number of examples is chosen considering the second part of the exercise: a significant portion of the training samples (66%) will be used to find the best parameters for the searches, while the remaining part will be used for testing.

To accurately recreate the problem description, the datasets are generated with specific criteria. Points are placed in shapes such as rectangles, squares, and circles. Shapes are picked randomly at each iteration. Additionally, there are points considered to be holes for bolts: there are at least 4 in each dataset, placed near the corners of the board.

Points are generated randomly according to a shape, but the script ensures that the distance between points is at least 0.3. This is done to avoid points being too close to each other, which could lead to numerical instability in the solution, particularly when subtracting numbers very close to each other.

The script also ensures that shapes do not overlap. Before adding a new shape, it checks for overlaps with existing shapes using specific functions:
- #code("check_overlap()", lan:"python") for circles

- #code("check_rectangle_overlap()", lan:"python") for rectangles and squares
- #code("check_circle_rectangle_overlap()", lan: "python") for checking overlaps between circles and rectangles


The data is stored in a #code(".dat") file, named #code("TSP_XX_X.dat"). The first line contains the number of nodes generated, and each subsequent line stores the coordinates of a point. The datasets are stored in the #code("/data", lan: none) folder.

The script generates also plots to visualize data using the function #code("save_plot_as_file()"), which are stored in the #code("/plot", lan: none) folder.
Some examples can be seen in #fig("example1", "Figure 2") and #fig("example2", "Figure 3").
#align(center, [#grid(columns: 2, 
  [#figure(image("../img/TSP_75_3.png", width: 100%), caption: [Instance with 75 points]) <example1>],
  [#figure(image("../img/TSP_100_5.png", width: 100%), caption: [Instance with 100 points]) <example2>]
)])



To visualize solutions, the script #code("plot_solutions.py") plots the dataset with the path solution. In particular, it reads data from a #code(".dat") file containing the dataset with the points coordinates placed in the #code("data/", lan: none) folder and from another #code(".dat") file the path solution, placed in the #code("data/sol/", lan: none) folder. Some examples can be visualized in #fig("example3", "Figure 4") and  #fig("example4", "Figure 5")
#pagebreak()

#align(center, [#grid(columns: 2, 
  [#figure(image("../img/Path_TSP_75_3.png", width: 100%), caption: [Optimal path with 75 points]) <example3>],
  [#figure(image("../img/Path_TSP_100_5.png", width: 100%), caption: [Optimal path with 100 points]) <example4>]
)])
#pagebreak()