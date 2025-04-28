#include "obj_shape.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

OBJShape::OBJShape(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open OBJ file.");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({ x, y, z });
        }
        else if (prefix == "f") {
            std::vector<int> face;
            std::string vertex;
            while (iss >> vertex) {
                size_t pos = vertex.find('/');
                int index = std::stoi(vertex.substr(0, pos)) - 1; // Convert 1-based to 0-based index
                face.push_back(index);
            }
            faces.push_back(face);
        }
    }

    if (vertices.empty() || faces.empty()) {
        throw std::runtime_error("Invalid OBJ file.");
    }
}

std::vector<std::vector<std::vector<double>>> OBJShape::getTriangles() const
{
    std::vector<std::vector<std::vector<double>>> triangles;
    for (const auto& face : faces) {
        for (size_t i = 1; i + 1 < face.size(); ++i) {
            triangles.push_back({
                vertices[face[0]],
                vertices[face[i]],
                vertices[face[i + 1]]
                });
        }
    }
    return triangles;
}
