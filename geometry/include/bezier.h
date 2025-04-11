#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <string>
#include "shape.h"

class Bezier : public Shape {
private:
    std::vector<std::vector<double>> controlPoints;

public:
    Bezier();

    void plot(const std::string& filename) const override;
    void saveToFile(const std::string& filename) const override;

    void addControlPoint(double x, double y, double z);
    void translate(double dx, double dy, double dz) override;
    void rotate(double angle, char axis) ;
    void scale(double sx, double sy, double sz) ;

    std::vector<std::vector<double>> calculateBezierCurve(int numSegments = 100) const;
};

#endif // BEZIER_H
