#include "polygon.h"
#include "plot_utils.h"         
#include "transform_utils.h"     
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

Polygon::Polygon(int sides) {
    vertices.reserve(sides);
}

void Polygon::addVertex(double x, double y, double z) {
    vertices.push_back({x, y, z});
}

void Polygon::calculateCentroid(double &cx, double &cy, double &cz) {
    cx = cy = cz = 0;
    for (const auto &v : vertices) {
        cx += v[0];
        cy += v[1];
        cz += v[2];
    }
    if (!vertices.empty()) {
        cx /= vertices.size();
        cy /= vertices.size();
        cz /= vertices.size();
    }
}

void Polygon::plot(const string &filename) const {
    saveToFile(filename);
    plotWithGnuplot(filename);  
}

void Polygon::saveToFile(const string &filename) const {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t next = (i + 1) % vertices.size();  // Close polygon
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[next][0] << " " << vertices[next][1] << " " << vertices[next][2] << "\n\n";
    }

    file.close();
}

void Polygon::translate(double dx, double dy, double dz) {
    translateVertices(vertices, dx, dy, dz);  
}

void Polygon::scale(double sx, double sy, double sz) {
    scaleVertices(vertices, sx, sy, sz);      
}

void Polygon::rotate(double angle, char axis, double cx, double cy, double cz) {
    // If user did not override center, use centroid
    if (cx == 0 && cy == 0 && cz == 0) {
        calculateCentroid(cx, cy, cz);
    }
    rotateVertices(vertices, angle, axis, cx, cy, cz);
}


