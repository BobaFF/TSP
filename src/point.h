#ifndef POINT_H
#define POINT_H

#include "utility"
#include <cmath>
using namespace std;

class Point {
    private:
        double x;
        double y;
    public:
        Point(): x(0), y(0) {};
        Point(double X, double Y): x(X), y(Y) {};
        Point(pair<double, double> c): x(c.first), y(c.second) {};
        /* Returns a pair containing the coordinates*/
        pair<double, double> getXY() const;
        /* Computes the euclidean distance from another point */
        double distance(const Point&) const;
};

#endif