import numpy as np
import matplotlib.pyplot as plt # type: ignore
import random
DECIMALS = 3
def add_point_if_valid(points, x, y, max_x, max_y):
    """Add a point to the list only if it is valid."""
    if 1 <= x <= max_x - 1 and 1 <= y <= max_y - 1:
        for px, py in points:
            if ((px - x)**2 + (py - y)**2)**0.5 < 0.5:
                return
        points.append((round(x, DECIMALS), round(y, DECIMALS)))
        

def save_as_dat_file(points, filename):
    """Saves points in .dat file."""
    with open(filename, 'w') as f:
        f.write(f'{len(points)}\n')
        for x, y in points:
            f.write(f'{round(x, DECIMALS)} {round(y, DECIMALS)}\n')

def generate_random_point(max_x, max_y):
    """Generates a random point."""
    return random.uniform(1, max_x - 1), random.uniform(1, max_y - 1)

def generate_random_bolts(max_x, max_y, bolt_distance=1, edge_distance=1, max_bolts=8):
    """Generates holes for bolts."""
    bolts = []
    
    # Corners
    bolts.append((edge_distance, edge_distance))  
    bolts.append((max_x - edge_distance, edge_distance))  
    bolts.append((edge_distance, max_y - edge_distance))  
    bolts.append((max_x - edge_distance, max_y - edge_distance))  

    # Others
    num_bolts_on_side = (max_x // bolt_distance) - 2
    for i in range(1, num_bolts_on_side):
        bolts.append((i * bolt_distance + edge_distance, edge_distance))
        bolts.append((i * bolt_distance + edge_distance, max_y - edge_distance))
        bolts.append((0 + edge_distance, i * bolt_distance + edge_distance))
        bolts.append((max_x - edge_distance, i * bolt_distance + edge_distance))

    # Limits the number of bolts
    return bolts[:max_bolts]


def generate_random_circle(max_x, max_y, max_radius, num_points):
    """Generates points in the perimeter of a circle."""
    center = (random.uniform(0, max_x), random.uniform(0, max_y))
    radius = random.uniform(1, max_radius)
    angles = np.linspace(0, 2 * np.pi, num_points, endpoint=False)
    points = []
    for a in angles:
      add_point_if_valid(points=points, x=center[0] + radius * np.cos(a), y=center[1] + radius * np.sin(a), max_x=max_x, max_y=max_y)
    return points, center, radius


def generate_random_rectangle(max_x, max_y, max_width, max_height, num_points):
    """Generates points in the perimeter of a rectangle."""
    corner = (random.uniform(0, max_x), random.uniform(0, max_y))
    width = random.uniform(1, max_width)
    height = random.uniform(1, max_height)
    points = []
    for t in np.linspace(0, 1, num_points // 4, endpoint=False):
        add_point_if_valid(points, corner[0] + t * width, corner[1], max_x, max_y)  # Bottom side
        add_point_if_valid(points, corner[0] + width, corner[1] + t * height, max_x, max_y)  # Right side
        add_point_if_valid(points, corner[0] + width - t * width, corner[1] + height, max_x, max_y)  # Top side
        add_point_if_valid(points, corner[0], corner[1] + height - t * height, max_x, max_y)  # Left side
    return points, corner, width, height


def generate_random_square(max_x, max_y, max_side, num_points):
    """Generates points in the perimeter of a square."""
    return generate_random_rectangle(max_x, max_y, max_side, max_side, num_points)


def check_overlap(center1, radius1, center2, radius2):
    """Check if two circles overlap."""
    distance = np.sqrt((center1[0] - center2[0]) ** 2 + (center1[1] - center2[1]) ** 2)
    return distance < (radius1 + radius2)


def check_rectangle_overlap(corner1, width1, height1, corner2, width2, height2):
    """Check if two rectangles overlap."""
    return not (corner1[0] + width1 < corner2[0] or corner2[0] + width2 < corner1[0] or
                corner1[1] + height1 < corner2[1] or corner2[1] + height2 < corner1[1])

def check_circle_rectangle_overlap(circle_center, radius, rect_corner, rect_width, rect_height):
    """Check if a circle and a rectangle overlap."""
    circle_x, circle_y = circle_center
    rect_x, rect_y = rect_corner

    # Find the closest point on the rectangle to the circle's center
    closest_x = max(rect_x, min(circle_x, rect_x + rect_width))
    closest_y = max(rect_y, min(circle_y, rect_y + rect_height))

    # Calculate the distance from the circle's center to this point
    distance = np.sqrt((circle_x - closest_x) ** 2 + (circle_y - closest_y) ** 2)

    # If the distance is less than or equal to the radius then shapes overlap 
    return distance <= radius


def generate_random_points(max_nodes, max_x, max_y):
    """Generates random shapes with a given number of nodes."""
    all_points = []
    shapes = []
    bolts = generate_random_bolts(max_x, max_y, bolt_distance=(max_x + max_y) // 10, edge_distance=(max_x + max_y) / 40, max_bolts=4)
    all_points.extend(bolts)
    remaining_nodes = max_nodes - len(bolts)

    while remaining_nodes > 0:
        figure_type = random.choices(
            ['circle', 'rectangle', 'square'],
            weights=[0.4, 0.2, 0.4],
            k=1
        )[0]
        num_points = random.randint(1, min(remaining_nodes, 8))  
        points = []
        if num_points < 4:
            points = [generate_random_point(max_x, max_y)]
        elif figure_type == 'circle':
            points, center, radius = generate_random_circle(max_x, max_y, random.randint(4, max(max_x // 10, 5)), num_points)
            if any(check_overlap(center, radius, s[1], s[2]) for s in shapes if s[0] == 'circle') or \
               any(check_circle_rectangle_overlap(center, radius, s[1], s[2], s[3]) for s in shapes if s[0] == 'rectangle'):
                continue
            shapes.append(('circle', center, radius))
        elif figure_type == 'rectangle':
            points, corner, width, height = generate_random_rectangle(max_x, max_y, random.randint(4, max(max_x // 10, 5)), random.randint(2, 8), num_points)
            if any(check_rectangle_overlap(corner, width, height, s[1], s[2], s[3]) for s in shapes if s[0] == 'rectangle') or \
               any(check_circle_rectangle_overlap(s[1], s[2], corner, width, height) for s in shapes if s[0] == 'circle'):
                continue
            shapes.append(('rectangle', corner, width, height))
        elif figure_type == 'square':
            points, corner, side, _ = generate_random_square(max_x, max_y, random.randint(4, max(max_x // 10, 5)), num_points)
            if any(check_rectangle_overlap(corner, side, side, s[1], s[2], s[3]) for s in shapes if s[0] == 'rectangle') or \
               any(check_circle_rectangle_overlap(s[1], s[2], corner, side, side) for s in shapes if s[0] == 'circle'):
                continue
            shapes.append(('rectangle', corner, side, side))

        all_points.extend(points)
        remaining_nodes -= len(points) 

    return all_points

def save_plot_as_file(points, filename, max_x, max_y):
    """Saves plot in .png file."""
    x_coords, y_coords = zip(*points)
    plt.scatter(x_coords, y_coords, color='blue', s=10)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.xlim(0, max_x)
    plt.ylim(0, max_y)
    plt.grid(True)
    plt.title(f'{len(points)} points generated with $max_x = max_y = $ {round(max_x, 2)}')
    plt.savefig(filename)
    plt.close()

# Data
max_nodes = [10, 25, 50, 75, 100]
max_x_y = [10, 25, 25, 75, 75]

for max_nodes, max_x_y in zip(max_nodes, max_x_y):
    for i in range(1, 10):
        points = []
        points = generate_random_points(max_nodes, max_x_y, max_x_y)
        print(i, len(points))
        save_as_dat_file(points, f'../data/TSP_{len(points)}_{i}.dat')
        save_plot_as_file(points, f'../plots/TSP_{len(points)}_{i}.png', max_x_y, max_x_y)


