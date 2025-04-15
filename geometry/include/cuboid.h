#ifndef CUBOID_H
#define CUBOID_H

#include <vector>
#include <string>
#include "shape.h"

class Cuboid : public Shape {
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


    vector<pair<vector<double>, vector<double>>> getCuboidEdgesAsLines() const;

};



#endif // CUBOID_H