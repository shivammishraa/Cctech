#include "polyline.h"
#include "transform_utils.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Polyline::Polyline() {}

void Polyline::addPoint(double x, double y, double z) {
    points.push_back({x, y, z});
}

void Polyline::translate(double dx, double dy, double dz) {
    translateVertices(points, dx, dy, dz);
}

void Polyline::scale(double sx, double sy, double sz) {
    scaleVertices(points, sx, sy, sz);
}

void Polyline::rotate(double angle, char axis) {
    rotateVertices(points, angle, axis);
}

void Polyline::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Cannot save file " << filename << endl;
        return;
    }
    for (const auto& point : points) {
        file << point[0] << " " << point[1] << " " << point[2] << "\n";
    }
    file.close();
}

void Polyline::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    points.clear();
    double x, y, z;
    while (file >> x >> y >> z) {
        points.push_back({x, y, z});
    }
    file.close();
}

void Polyline::plotPolyline(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cerr << "Error: Unable to create file: " << filename << endl;
        return;
    }

    for (const auto& point : points) {
        file << point[0] << " " << point[1] << " " << point[2] << endl;
    }
    file << "\n"; 
    file.close();

    string command = "gnuplot -p -e \"splot '" + filename + "' with linespoints title 'Polyline'; pause -1\"";
    int result = system(command.c_str());

    if (result != 0) {
        cerr << "Error: Could not execute GNUPlot command." << endl;
    }
}

void Polyline::plot(const string& filename) const {
    plotPolyline(filename);
}
