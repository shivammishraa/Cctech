#include "cuboid.h"
#include "transform_utils.h"
#include "plot_utils.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

Cuboid::Cuboid(double length, double width, double height) {
    vertices = {
        {0, 0, 0}, {length, 0, 0}, {length, width, 0}, {0, width, 0}, // Bottom face
        {0, 0, height}, {length, 0, height}, {length, width, height}, {0, width, height} // Top face
    };
}

void Cuboid::translate(double dx, double dy, double dz) {
    translateVertices(vertices, dx, dy, dz);  
}

void Cuboid::scale(double sx, double sy, double sz) {
    scaleVertices(vertices, sx, sy, sz);  
}

void Cuboid::rotate(double angle, char axis) {
    rotateVertices(vertices, angle, axis);  
}

void Cuboid::saveToFile(const string& filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    for (int i = 0; i < 12; ++i) {
        const auto& v1 = vertices[edges[i][0]];
        const auto& v2 = vertices[edges[i][1]];
        file << v1[0] << " " << v1[1] << " " << v1[2] << "\n";
        file << v2[0] << " " << v2[1] << " " << v2[2] << "\n\n";
    }

    file.close();
}

void Cuboid::plot(const string& filename) const {
    saveToFile(filename);
    plotWithGnuplot(filename, "blue", 2);  
}
