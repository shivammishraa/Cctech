#ifndef LINE3D_H
#define LINE3D_H

#include "shape.h"
#include <vector>
#include <string>

class Line3D : public Shape { 
private:
    std::vector<std::vector<double>> points;

public:
    Line3D();
    void setPoints(double x1, double y1, double z1, double x2, double y2, double z2);
    
    // These override the virtual methods in Shape
    void plot(const std::string &filename) const override;
    void saveToFile(const std::string &filename) const override;
    void translate(double dx, double dy, double dz) override;

    void scale(double sx, double sy, double sz);
    void rotate(double angle, char axis);
};

#endif
