#ifndef TSP_H
#define TSP_H
#include "point.h"
#include <fstream>
#include <vector>
#include <string>

class TSP {
    private:
        /* Number of nodes */
        int n;
        /* Vector of the nodes (keeps their position)*/
        std::vector<Point> nodes;
        /* Matrix of the costs of the arc (i, j) */
        std::vector<std::vector<double> > costs;
        /* Computes the matrix costs*/
        void calcCosts();
        /* Reads coordinates from file*/
        void read(const std::string&);
        /* Source file name*/
        std::string filename;
    public:
        TSP(): n(0) {};
        TSP(const std::string& filename): filename(filename) {
            read(filename);
            calcCosts();
        }
        /* Returns the matrix costs associated to the TSP*/
        std::vector<std::vector<double> > getCosts() const;
        /* Returns the number of nodes*/
        int getSize() const;
        /* Returns the file name*/
        std::string getFileName() const;

};

#endif