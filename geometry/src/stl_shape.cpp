#include "stl_shape.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <array>
#include <chrono>
#include <unordered_set>

using namespace std;

struct VectorHash {
    std::size_t operator()(const std::array<double, 3>& v) const {
        std::hash<double> hasher;
        // Combine the hashes of the three elements in the array
        return ((hasher(v[0]) ^ (hasher(v[1]) << 1)) >> 1) ^ (hasher(v[2]) << 1);
    }
};
struct VectorEqual {
    bool operator()(const std::array<double, 3>& a, const std::array<double, 3>& b) const noexcept {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    }
};

// Constructor to parse the STL file
STLShape::STLShape(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open STL file.");
    }

    std::string line;
    std::vector<std::vector<double>> triangle(3, std::vector<double>(3));
    int vertexCount = 0;

    while (std::getline(file, line)) {
        if (line.find("vertex") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            double x, y, z;
            iss >> temp >> x >> y >> z;
            triangle[vertexCount++] = { x, y, z };

            if (vertexCount == 3) {
                triangles.push_back(triangle);
                vertexCount = 0;
            }
        }
    }

    if (triangles.empty()) {
        throw std::runtime_error("No triangles found in STL file.");
    }
}

// Save triangles to .dat file using buffered write (stringstream)
void STLShape::saveToFile(const string& filename) const {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    ofstream file(filename, ios::out);
    if (!file.is_open()) {
        cerr << "Error: Could not open .dat file: " << filename << endl;
        return;
    }

    stringstream buffer; // Accumulate output here

    for (const auto& triangle : triangles) {
        for (const auto& vertex : triangle) {
            buffer << vertex[0] << " " << vertex[1] << " " << vertex[2] << '\n';
        }
        // Close triangle loop (repeat first point)
        buffer << triangle[0][0] << " " << triangle[0][1] << " " << triangle[0][2] << '\n';
        buffer << '\n'; // Separate triangle
    }

    file << buffer.str(); // Dump all at once
    file.close();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << " Saved .dat file in: " << duration << " ms\n";
}

// Plot with GNUPLOT
void STLShape::plot(const string& filename) const {
    saveToFile(filename);

    string command =
        "gnuplot -p -e \""
        "set terminal wxt; "
        "set xlabel 'X'; "
        "set ylabel 'Y'; "
        "set zlabel 'Z'; "
        "set view equal xyz; "
        "splot '" + filename + "' with lines 7 linecolor 'blue'\"";
    system(command.c_str());
}

std::vector<std::vector<std::vector<double>>> STLShape::getTriangles() const {
    return triangles; // Already optimized, no redundant data
}
