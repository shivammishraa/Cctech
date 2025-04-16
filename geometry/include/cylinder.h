#ifndef CYLINDER_H
#define CYLINDER_H

#include <vector>
#include <string>
#include "shape3d.h"
#include "shape.h" // or also include shape3d.h if needed

class Cylinder : public Shape, public Shape3D {
private:
    double radius;
    double height;
    int resolution;
    std::vector<std::vector<double>> vertices;

    void generateVertices();

public:
    Cylinder(double r, double h, int res);

    // ? REMOVE override unless getEdges() exists in base class
    std::vector<std::pair<std::vector<double>, std::vector<double>>> getEdges() const;

    void plot(const std::string& filename) const override;
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz);
    void rotate(double angle, char axis);
    void scale(double sx, double sy, double sz);
};

#endif // CYLINDER_H
