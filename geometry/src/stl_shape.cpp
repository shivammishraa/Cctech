#include "stl_shape.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <unordered_set>

using namespace std;

struct VectorHash {
    size_t operator()(const array<double, 3>& v) const {
        hash<double> hasher;
        return ((hasher(v[0]) ^ (hasher(v[1]) << 1)) >> 1) ^ (hasher(v[2]) << 1);
    }
};

struct VectorEqual {
    bool operator()(const array<double, 3>& a, const array<double, 3>& b) const {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    }
};

// Constructor to parse the STL file
STLShape::STLShape(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error: Could not open STL file: " << filepath << endl;
        return;
    }

    unordered_set<array<double, 3>, VectorHash, VectorEqual> uniqueVertices;

    string line;
    array<array<double, 3>, 3> triangle;
    int vertexCount = 0, totalVerticesParsed = 0;

    while (getline(file, line)) {
        if (line.find("vertex") != string::npos) {
            istringstream iss(line);
            string temp;
            double x, y, z;
            iss >> temp >> x >> y >> z;
            totalVerticesParsed++;

            array<double, 3> vertex = {x, y, z};
            triangle[vertexCount++] = vertex;

            uniqueVertices.insert(vertex);

            if (vertexCount == 3) {
                triangles.push_back(triangle);
                vertexCount = 0;
            }
        }
    }

    file.close();

    cout << "STL File Loaded: " << filepath << endl;
    cout << " Total Vertices Parsed       : " << totalVerticesParsed << endl;
    cout << " Unique Vertices Stored      : " << uniqueVertices.size() << endl;
    cout << " Total Triangles Extracted   : " << triangles.size() << endl << endl;
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
