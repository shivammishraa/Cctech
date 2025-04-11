#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include <string>
#include "shape.h"

class Cylinder : public Shape {
private:
    double radius;
    double height;
    int resolution;
    std::vector<std::vector<double>> vertices;

    void generateVertices();

public:
    Cylinder(double r, double h, int res);

    void plot(const std::string& filename) const override; // Override plot method
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz);
    void rotate(double angle, char axis);
    void scale(double sx, double sy, double sz);
};

#endif // CYLINDER_H
