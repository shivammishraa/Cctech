#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include <string>
#include "shape.h"

class Cuboid : public Shape {
private:
    std::vector<std::vector<double>> vertices; // Stores cuboid vertices
   

public:
    Cuboid(double length, double width, double height);

    void plot(const std::string& filename) const override; // Override plot method
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz);
    void scale(double sx, double sy, double sz);
    void rotate(double angle, char axis);
};

#endif // CUBOID_H