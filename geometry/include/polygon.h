#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include "shape.h"
#include "shape3d.h" 

class Polygon : public Shape, public Shape3D {
private:
    std::vector<std::vector<double>> vertices; // Stores (x, y, z) coordinates
    void calculateCentroid(double &cx, double &cy, double &cz); // Helper function

public:
    // Constructor that takes a list of vertices instead of sides
    Polygon(const std::vector<std::vector<double>>& vertices); // Constructor
    Polygon();

    void addVertex(double x, double y, double z);
    void plot(const std::string& filename) const override; // Override plot method
    void saveToFile(const std::string& filename) const override;

    void translate(double dx, double dy, double dz);
    void scale(double sx, double sy, double sz);
    void rotate(double angle, char axis, double cx = 0, double cy = 0, double cz = 0);

    std::vector<std::pair<std::vector<double>, std::vector<double>>> getEdges() const ;

    // void rotateVertices(std::vector<std::vector<double>> &vertices, double angle, char axis, double cx = 0, double cy = 0, double cz = 0);
};

#endif // POLYGON_H
