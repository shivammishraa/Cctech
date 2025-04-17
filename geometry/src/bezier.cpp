// Bezier.cpp
#include "../include/bezier.h"
#include "../include/transform_utils.h"
#include "../include/plot_utils.h"

#include <fstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

// Compute binomial coefficient C(n, k)
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
    controlPoints.push_back({ x, y, z });
}

std::vector<std::vector<double>> Bezier::calculateBezierCurve(int numPoints) const {
    std::vector<std::vector<double>> curve;
    int n = controlPoints.size() - 1;

    if (n < 1 || numPoints < 2) {
        throw std::invalid_argument("Not enough control points or invalid number of interpolation points.");
    }

    for (int i = 0; i < numPoints; ++i) {
        double t = static_cast<double>(i) / (numPoints - 1); // from 0 to 1 inclusive
        std::vector<double> point(3, 0);

        for (int j = 0; j <= n; ++j) {
            double bernstein = binomialCoeff(n, j) * pow(t, j) * pow(1 - t, n - j);
            point[0] += bernstein * controlPoints[j][0];
            point[1] += bernstein * controlPoints[j][1];
            point[2] += bernstein * controlPoints[j][2];
        }

        curve.push_back(point);
    }

    return curve;
}

// Override the base class method
void Bezier::plot(const std::string& filename) const {
    plot(filename, interpolationCount);  // Default value for numInterpolationPoints
}

// Existing function with the extra parameter
void Bezier::plot(const std::string& filename, int numInterpolationPoints) const {
    std::vector<std::vector<double>> curve = calculateBezierCurve(numInterpolationPoints);
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

std::vector<std::pair<std::vector<double>, std::vector<double>>> Bezier::getEdges() const {
    std::vector<std::pair<std::vector<double>, std::vector<double>>> edges;

    std::vector<std::vector<double>> curve = calculateBezierCurve(100);

    for (size_t i = 0; i < curve.size() - 1; ++i) {
        edges.push_back({ curve[i], curve[i + 1] });
    }

    return edges;
}

void Bezier::setInterpolationCount(int count) {
    interpolationCount = count;
}

