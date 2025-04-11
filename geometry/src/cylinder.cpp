#include "cylinder.h"
#include "transform_utils.h"
#include "plot_utils.h"
#include <fstream>
#include <cmath>
#include <iostream>
#define M_PI 3.14159265358979323846
using namespace std;

Cylinder::Cylinder(double r, double h, int res) : radius(r), height(h), resolution(res) {
    generateVertices();
}

void Cylinder::generateVertices() {
    double angleStep = 2 * M_PI / resolution;

    // Bottom circle
    for (int i = 0; i < resolution; i++) {
        double x = radius * cos(i * angleStep);
        double y = radius * sin(i * angleStep);
        vertices.push_back({x, y, 0});
    }

    // Top circle
    for (int i = 0; i < resolution; i++) {
        double x = radius * cos(i * angleStep);
        double y = radius * sin(i * angleStep);
        vertices.push_back({x, y, height});
    }
}

void Cylinder::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    // Bottom circle edges
    for (int i = 0; i < resolution; i++) {
        int next = (i + 1) % resolution;
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[next][0] << " " << vertices[next][1] << " " << vertices[next][2] << "\n\n";
    }

    // Top circle edges
    for (int i = resolution; i < 2 * resolution; i++) {
        int next = (i + 1) % resolution + resolution;
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[next][0] << " " << vertices[next][1] << " " << vertices[next][2] << "\n\n";
    }

    // Vertical edges
    for (int i = 0; i < resolution; i++) {
        file << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << "\n";
        file << vertices[i + resolution][0] << " " << vertices[i + resolution][1] << " " << vertices[i + resolution][2] << "\n\n";
    }

    file.close();
}

void Cylinder::plot(const string &filename) const {
    saveToFile(filename);
    plotWithGnuplot(filename);  
};

void Cylinder::translate(double dx, double dy, double dz) {
    translateVertices(vertices, dx, dy, dz);  
}

void Cylinder::scale(double sx, double sy, double sz) {
    scaleVertices(vertices, sx, sy, sz);  
}

void Cylinder::rotate(double angle, char axis) {
    rotateVertices(vertices, angle, axis);  
}