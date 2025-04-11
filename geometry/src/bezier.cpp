#include "../include/bezier.h"
#include "../include/transform_utils.h"
#include "../include/plot_utils.h"

#include <fstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

int binomialCoeff(int n, int k) {
    if (k < 0 || k > n) throw std::invalid_argument("Invalid values for n and k");
    int res = 1;
    if (k > n - k) k = n - k;
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

Bezier::Bezier() : controlPoints{} {}

void Bezier::addControlPoint(double x, double y, double z) {
    controlPoints.push_back({x, y, z});
}

std::vector<std::vector<double>> Bezier::calculateBezierCurve(int numSegments) const {
    std::vector<std::vector<double>> curve;
    int n = controlPoints.size() - 1;

    for (int i = 0; i <= numSegments; i++) {
        double t = static_cast<double>(i) / numSegments;
        std::vector<double> point(3, 0);

        for (int j = 0; j <= n; j++) {
            double bernstein = binomialCoeff(n, j) * pow(t, j) * pow(1 - t, n - j);
            point[0] += bernstein * controlPoints[j][0];
            point[1] += bernstein * controlPoints[j][1];
            point[2] += bernstein * controlPoints[j][2];
        }

        curve.push_back(point);
    }

    return curve;
}

void Bezier::plot(const std::string& filename) const {
    std::vector<std::vector<double>> curve = calculateBezierCurve();
    saveVerticesToFile(curve, filename);
    plotWithGnuplot(filename, "blue", 2);
}

void Bezier::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    for (size_t i = 0; i < controlPoints.size() - 1; ++i) {
        file << controlPoints[i][0] << " " << controlPoints[i][1] << " " << controlPoints[i][2] << "\n";
        file << controlPoints[i + 1][0] << " " << controlPoints[i + 1][1] << " " << controlPoints[i + 1][2] << "\n\n";
    }

    file.close();
}

void Bezier::translate(double dx, double dy, double dz) {
    translateVertices(controlPoints, dx, dy, dz);
}

void Bezier::rotate(double angle, char axis) {
    rotateVertices(controlPoints, angle, axis);
}

void Bezier::scale(double sx, double sy, double sz) {
    scaleVertices(controlPoints, sx, sy, sz);
}
