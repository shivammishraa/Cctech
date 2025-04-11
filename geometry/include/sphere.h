#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <string>
#include "shape.h"

class Sphere : public Shape {
private:
    double radius;
    int segments;
    std::vector<std::vector<double>> vertices;

public:
    Sphere(double r, int seg);

    void plot(const std::string& filename) const override;
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz);
    void scale(double factor);
    void rotate(double angle, char axis);  // Optional: if you want rotation
};

#endif // SPHERE_H
