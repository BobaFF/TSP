#include "point.h"

pair<double, double> Point::getXY() const{
    return make_pair(x, y);
};
double Point::distance(const Point& p) const {
    pair<double, double> p2 = p.getXY();
    return sqrt(pow(x - p2.first, 2) + pow(y - p2.second, 2));
}