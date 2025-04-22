#include "polyline.h"
#include "transform_utils.h"
#include <iostream>
#include <qdebug.h>
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

vector<pair<vector<double>, vector<double>>> Polyline::getEdges() const {
    vector<pair<vector<double>, vector<double>>> edges;
    if (points.size() < 2) return edges;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        edges.push_back({ points[i], points[i + 1] });
    }

    qDebug() << "Polyline::getEdges() called - Edge count:" << edges.size();
    for (const auto& edge : edges) {
        qDebug() << "Edge from (" << edge.first[0] << edge.first[1] << edge.first[2] << ")"
            << "to (" << edge.second[0] << edge.second[1] << edge.second[2] << ")";
    }

    return edges;
}

