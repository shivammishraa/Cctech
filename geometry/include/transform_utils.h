#ifndef TRANSFORM_UTILS_H
#define TRANSFORM_UTILS_H

#include <vector>

// Translation
void translateVertices(std::vector<std::vector<double>>& vertices, double dx, double dy, double dz);

// Scaling
void scaleVertices(std::vector<std::vector<double>>& vertices, double sx, double sy, double sz);  // non-uniform
void scaleVertices(std::vector<std::vector<double>>& vertices, double factor);                    // uniform  new

// Rotation
void rotateVertices(std::vector<std::vector<double>>& vertices, double angle, char axis);
void rotateVertices(std::vector<std::vector<double>>& vertices, double angle, char axis, double cx, double cy, double cz);

#endif
