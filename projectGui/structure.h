#pragma once

#include <vector>

struct Vertex {
    float x, y, z;
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Edge {
    Vertex* v1;
    Vertex* v2;
    Edge(Vertex* v1, Vertex* v2) : v1(v1), v2(v2) {}
};

struct Face {
    std::vector<Edge*> edges;
    Face(const std::vector<Edge*>& edges) : edges(edges) {}
};

struct Solid {
    std::vector<Face*> faces;
};
