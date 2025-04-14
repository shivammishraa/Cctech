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
        {0, 0, 0}, {length, 0, 0}, {length, width, 0}, {0, width, 0},
        {0, 0, height}, {length, 0, height}, {length, width, height}, {0, width, height}
    };
}

Cuboid::Cuboid(const std::vector<double>& origin, double length, double width, double height) {
    double x = origin[0], y = origin[1], z = origin[2];
    vertices = {
        {x, y, z}, {x + length, y, z}, {x + length, y + width, z}, {x, y + width, z},
        {x, y, z + height}, {x + length, y, z + height}, {x + length, y + width, z + height}, {x, y + width, z + height}
    };
}

// std::vector<std::array<double, 3>> Cuboid::getVertices() const {
//     std::vector<std::array<double, 3>> verticesToReturn;
//     // Return the 8 vertices for the cuboid
//     verticesToReturn.insert(verticesToReturn.end(), vertices.begin(), vertices.end());
//     return verticesToReturn;
// }

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

std::vector<std::vector<std::vector<double>>> Cuboid::getCuboidTriangles() const {
    std::vector<std::vector<std::vector<double>>> triangles;

    const int faces[6][4] = {
        {0, 1, 2, 3}, // bottom
        {4, 5, 6, 7}, // top
        {0, 1, 5, 4}, // front
        {2, 3, 7, 6}, // back
        {1, 2, 6, 5}, // right
        {3, 0, 4, 7}  // left
    };

    for (const auto& face : faces) {
        triangles.push_back({ vertices[face[0]], vertices[face[1]], vertices[face[2]] });
        triangles.push_back({ vertices[face[2]], vertices[face[3]], vertices[face[0]] });
    }

    return triangles;
}