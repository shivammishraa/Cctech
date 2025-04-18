// Bezier.h
#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include <string>
#include "shape.h"
#include "shape3d.h"

class Bezier : public Shape, public Shape3D {
private:
    std::vector<std::vector<double>> controlPoints;
    int interpolationCount = 100;

public:
    Bezier();

    // Override the base class method
    void plot(const std::string& filename) const override;

    // Additional method for custom plotting with interpolation count
    void plot(const std::string& filename, int numInterpolationPoints) const;

    void saveToFile(const std::string& filename) const override;

    void addControlPoint(double x, double y, double z);
    void translate(double dx, double dy, double dz) override;
    void rotate(double angle, char axis);
    void scale(double sx, double sy, double sz);
    void setInterpolationCount(int count);
    void generateCurve();
    std::vector<std::vector<double>> calculateBezierCurve(int numSegments = 100) const;
    std::vector<std::vector<double>> calculateBezierCurve() const;

    std::vector<double> deCasteljau(const std::vector<std::vector<double>>& points, double t)const;

    std::vector<std::pair<std::vector<double>, std::vector<double>>> getEdges() const;

};

#endif // BEZIER_H
