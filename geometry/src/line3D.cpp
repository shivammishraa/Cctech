#include "line3D.h"
#include "plot_utils.h"           
#include "transform_utils.h"      

#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

Line3D::Line3D() {
    points.resize(2, vector<double>(3, 0.0));
}

void Line3D::setPoints(double x1, double y1, double z1, double x2, double y2, double z2) {
    points[0] = {x1, y1, z1};
    points[1] = {x2, y2, z2};
}

void Line3D::plot(const string &filename) const {
    saveToFile(filename);
    plotWithGnuplot(filename);  
}

void Line3D::saveToFile(const string &filename) const {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    file << points[0][0] << " " << points[0][1] << " " << points[0][2] << "\n";
    file << points[1][0] << " " << points[1][1] << " " << points[1][2] << "\n\n";

    file.close();
}

void Line3D::translate(double dx, double dy, double dz) {
    translateVertices(points, dx, dy, dz);  
}

void Line3D::scale(double sx, double sy, double sz) {
    scaleVertices(points, sx, sy, sz);      
}

void Line3D::rotate(double angle, char axis) {
    rotateVertices(points, angle, axis);    
}
