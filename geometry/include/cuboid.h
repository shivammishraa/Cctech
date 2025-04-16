#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include <string>
#include "shape.h"
#include "shape3d.h"  // NEW include

class Cuboid : public Shape, public Shape3D {
private:
    std::vector<std::vector<double>> vertices;

public:
    Cuboid(double length, double width, double height);
    Cuboid(const std::vector<double>& origin, double length, double width, double height);

    std::vector<std::vector<std::vector<double>>> getCuboidTriangles() const;

    void plot(const std::string& filename) const override;
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz);
    void scale(double sx, double sy, double sz);
    void rotate(double angle, char axis);

    std::vector<std::pair<std::vector<double>, std::vector<double>>> getCuboidEdgesAsLines() const;

    // Implementation of Shape3D interface
    std::vector<std::pair<std::vector<double>, std::vector<double>>> getEdges() const override {
        return getCuboidEdgesAsLines();
    }
};

#endif // CUBOID_H