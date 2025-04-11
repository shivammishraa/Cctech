#include "sphere.h"
#include "transform_utils.h"
#include "plot_utils.h"

#include <fstream>
#include <cmath>
#include <iostream>
#define M_PI 3.14159265358979323846

using namespace std;

Sphere::Sphere(double r, int seg) : radius(r), segments(seg) {
    cout << "Generating sphere vertices..." << endl;

    for (int i = 0; i <= segments; i++) {
        double theta = M_PI * i / segments;
        for (int j = 0; j <= segments; j++) {
            double phi = 2 * M_PI * j / segments;
            double x = radius * sin(theta) * cos(phi);
            double y = radius * sin(theta) * sin(phi);
            double z = radius * cos(theta);

            if (isnan(x) || isnan(y) || isnan(z)) {
                cerr << "Error: Invalid vertex generated for sphere.\n";
                return;
            }

            vertices.push_back({x, y, z});
        }
    }

    cout << "Generated " << vertices.size() << " vertices for the sphere." << endl;
}

void Sphere::plot(const string& filename) const {
    ofstream file(filename);
    for (const auto& v : vertices) {
        file << v[0] << " " << v[1] << " " << v[2] << "\n";
    }
    file.close();

    plotWithGnuplot(filename, "blue", 2);
}

void Sphere::saveToFile(const string &filename) const {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Error: Cannot open file for writing.\n";
        return;
    }

    int numLatitudes = segments + 1;
    int numLongitudes = segments + 1;

    for (int i = 0; i < numLatitudes; i++) {
        for (int j = 0; j < numLongitudes; j++) {
            int index = i * numLongitudes + j;
            file << vertices[index][0] << " " << vertices[index][1] << " " << vertices[index][2] << "\n";
        }
        file << "\n";
    }

    file.close();
}

void Sphere::translate(double dx, double dy, double dz) {
    translateVertices(vertices, dx, dy, dz);
}

void Sphere::scale(double factor) {
    scaleVertices(vertices, factor);
}

void Sphere::rotate(double angle, char axis) {
    rotateVertices(vertices, angle, axis);
}
