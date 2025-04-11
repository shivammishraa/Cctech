#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include <string>
#include "shape.h"

class Triangle : public Shape {
private:
    std::vector<std::vector<double>> vertices;

public:
    Triangle(double x1, double y1, double z1,
             double x2, double y2, double z2,
             double x3, double y3, double z3);

    void plot(const std::string& filename) const override;
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz) override;
    void scale(double sx, double sy, double sz) ;
    void rotate(double angle, char axis, double Px = 0, double Py = 0, double Pz = 0) ;
};

#endif // TRIANGLE_H
