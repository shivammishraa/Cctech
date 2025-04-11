#include "triangle.h"
#include "transform_utils.h"
#include "plot_utils.h"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Triangle::Triangle(double x1, double y1, double z1,
                   double x2, double y2, double z2,
                   double x3, double y3, double z3) {
    vertices = {{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}};
}

void Triangle::translate(double dx, double dy, double dz) {
    translateVertices(vertices, dx, dy, dz);
}

void Triangle::scale(double sx, double sy, double sz) {
    scaleVertices(vertices, sx, sy, sz);
}

void Triangle::rotate(double angle, char axis, double Px, double Py, double Pz) {
    rotateVertices(vertices, angle, axis, Px, Py, Pz);
}

void Triangle::saveToFile(const string &filename) const {
    std::ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t next = (i + 1) % vertices.size();
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[next][0] << " " << vertices[next][1] << " " << vertices[next][2] << "\n\n";
    }

    file.close();
}

void Triangle::plot(const std::string& filename) const {
    std::vector<std::vector<double>> closedVertices = vertices;
    closedVertices.push_back(vertices[0]); // Close the triangle loop

    saveVerticesToFile(closedVertices, filename);
    plotWithGnuplot(filename, "red");
}