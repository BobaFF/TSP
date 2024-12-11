#include "TSP.h"

void TSP::read(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::ios_base::failure("Error while opening " + filename + "!");
    file >> n;
    if (n <= 0)
        throw std::invalid_argument("Number of nodes must be positive!");
    nodes.resize(n);
    for (int i = 0; i < n; i++) {
        double x;
        double y;
        if (!(file >> x >> y))
            throw std::ios_base::failure("Error reading coordinates for node " + std::to_string(i + 1) + " in " + filename + "!");
        nodes[i] = Point(x, y);
    }
}
void TSP::calcCosts() {
    costs.resize(n, std::vector<double>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double cost = nodes[i].distance(nodes[j]);
            costs[i][j] = cost;
            costs[j][i] = cost;
        }
    }
}
std::vector<std::vector<double> > TSP::getCosts() const {
    return costs;
};

int TSP::getSize() const {
    return n;
}
std::string TSP::getFileName() const {
    return filename;
}
