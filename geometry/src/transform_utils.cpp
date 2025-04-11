#include "transform_utils.h"
#include <cmath>

#define M_PI 3.14159265358979323846

// Translate
void translateVertices(std::vector<std::vector<double>>& vertices, double dx, double dy, double dz) {
    for (auto& v : vertices) {
        v[0] += dx;
        v[1] += dy;
        v[2] += dz;
    }
}

// Scale (non-uniform)
void scaleVertices(std::vector<std::vector<double>>& vertices, double sx, double sy, double sz) {
    for (auto& v : vertices) {
        v[0] *= sx;
        v[1] *= sy;
        v[2] *= sz;
    }
}

// Scale (uniform)
void scaleVertices(std::vector<std::vector<double>>& vertices, double factor) {
    for (auto& v : vertices) {
        v[0] *= factor;
        v[1] *= factor;
        v[2] *= factor;
    }
}

// Rotate around axis
void rotateVertices(std::vector<std::vector<double>>& vertices, double angle, char axis) {
    double radians = angle * M_PI / 180.0;
    for (auto& v : vertices) {
        double x = v[0], y = v[1], z = v[2];
        switch (axis) {
            case 'x':
            case 'X':
                v[1] = y * cos(radians) - z * sin(radians);
                v[2] = y * sin(radians) + z * cos(radians);
                break;
            case 'y':
            case 'Y':
                v[0] = x * cos(radians) + z * sin(radians);
                v[2] = -x * sin(radians) + z * cos(radians);
                break;
            case 'z':
            case 'Z':
                v[0] = x * cos(radians) - y * sin(radians);
                v[1] = x * sin(radians) + y * cos(radians);
                break;
        }
    }
}

// Rotate around point
void rotateVertices(std::vector<std::vector<double>>& vertices, double angle, char axis, double cx, double cy, double cz) {
    for (auto& v : vertices) {
        v[0] -= cx;
        v[1] -= cy;
        v[2] -= cz;
    }

    rotateVertices(vertices, angle, axis);

    for (auto& v : vertices) {
        v[0] += cx;
        v[1] += cy;
        v[2] += cz;
    }
}
