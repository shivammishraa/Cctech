#ifndef POLYLINE_H
#define POLYLINE_H

#include <vector>
#include <string>
#include "shape.h"

using namespace std;

class Polyline : public Shape {
private:
    vector<vector<double>> points;

public:
    Polyline();

    void addPoint(double x, double y, double z);
    void plotPolyline(const string& filename) const;

    void translate(double dx, double dy, double dz);
    void scale(double sx, double sy, double sz);
    void rotate(double angle, char axis);

    void saveToFile(const string& filename) const override;
    void loadFromFile(const string& filename);

    // Implement abstract base class methods
    void plot(const string& filename) const override;
};

#endif
