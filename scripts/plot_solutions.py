import matplotlib.pyplot as plt # type: ignore

def load_data(dataset, solution):
    """Loads data and path solutions from .dat files."""
    path_sol = []
    points_x=[]
    points_y=[]
    with open(dataset, 'r') as file:
        file.readline()
        for line in file:
            x, y = line.split()
            points_x.append(float(x))
            points_y.append(float(y))
    with open(solution, 'r') as file:
        sol = file.readline()
        for el in sol.split():
            path_sol.append(int(el))
    return points_x, points_y, path_sol

def order_points(path_sol, points_x, points_y):
    """Orders points according to the path solution."""
    new_points_x = []
    new_points_y = []
    for i in range(len(path_sol)):
        el = path_sol[i]
        new_points_x.append(points_x[el])
        new_points_y.append(points_y[el])
    return new_points_x, new_points_y

def save_path_plot_as_file(filename, points_x, points_y):
    """Saves plot with path solution in .png file."""
    plt.title("TSP solution")
    plt.scatter(points_x, points_y, color='black', s=20)
    plt.plot(points_x, points_y, linestyle='-', color='gray')
    plt.gca().set_aspect('equal', adjustable='box')
    plt.title(f'Path solution for {filename}')
    plt.savefig("../plots/sol/Path_"+ filename + ".png")
    plt.close()
    
max_nodes = [10, 25, 50, 75, 100]
for max_nodes in max_nodes:
    for i in range(1, 10):
        name = f'TSP_{max_nodes}_{i}'
        dataset = "../data/" + name + ".dat"
        solution = "../data/sol/Path_" + name + ".dat"
        points_x, points_y, path_sol = load_data(dataset, solution)
        ordered_x, oredered_y = order_points(path_sol, points_x, points_y)
        save_path_plot_as_file(name, ordered_x, oredered_y)
        print(name + " saved")