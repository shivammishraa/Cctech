#include "objToStl.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <array>
#include <chrono>

using namespace std;

// Hash function for array<double, 3>
struct ArrayHash
{
    size_t operator()(const array<double, 3>& v) const
    {
        hash<double> hasher;
        return ((hasher(v[0]) ^ (hasher(v[1]) << 1)) >> 1) ^ (hasher(v[2]) << 1);
    }
};

// Equality comparator for array<double, 3>
struct ArrayEqual
{
    bool operator()(const array<double, 3>& a, const array<double, 3>& b) const
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    }
};

// Convert and plot STL file
void ObjToStlConverter::convertAndPlot(const string& objFilename, const string& stlFilename)
{
    if (!ObjToStlConverter::convert(objFilename, stlFilename))
    {
        cerr << "Error: Conversion failed. Cannot plot.\n";
        return;
    }

    STLShape shape(stlFilename);
}

// Convert OBJ to STL
bool ObjToStlConverter::convert(const string& objFilename, const string& stlFilename)
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    vector<array<double, 3>> vertices;
    vector<vector<int>> faces;
    ObjToStlConverter::readObj(objFilename, vertices, faces);

    if (vertices.empty())
    {
        cerr << "Error: Failed to read OBJ file or file is empty.\n";
        return false;
    }

    ObjToStlConverter::writeStl(vertices, faces, stlFilename);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "Converted " << objFilename << " to " << stlFilename << " in " << duration << " ms\n";

    return true;
}

// Read OBJ file and extract vertices and face indices
void ObjToStlConverter::readObj(const string& filename,
    vector<array<double, 3>>& dedupedVertices,
    vector<vector<int>>& faces)
{
    ifstream file(filename);

    // Clear vectors to ensure clean slate
    dedupedVertices.clear();
    faces.clear();

    unordered_map<array<double, 3>, int, ArrayHash, ArrayEqual> vertexMap;

    if (!file.is_open())
    {
        cerr << "Error: Could not open OBJ file: " << filename << "\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string prefix;
        iss >> prefix;

        if (prefix == "v")
        {
            double x, y, z;
            if (!(iss >> x >> y >> z))
                continue;

            array<double, 3> vertex = { x, y, z };
            if (vertexMap.find(vertex) == vertexMap.end())
            {
                vertexMap[vertex] = static_cast<int>(dedupedVertices.size());
                dedupedVertices.push_back(vertex);
            }
        }
        else if (prefix == "f")
        {
            vector<int> face;
            string token;
            while (iss >> token)
            {
                try
                {
                    int index = stoi(token) - 1;
                    face.push_back(index);
                }
                catch (const exception&)
                {
                    continue;
                }
            }
            if (face.size() >= 3)
                faces.push_back(face);
        }
    }

    file.close();
}

// Write STL file with buffered output
void ObjToStlConverter::writeStl(const vector<array<double, 3>>& vertices,
    const vector<vector<int>>& faces,
    const string& filename)
{
    ofstream outFile(filename, ios::out | ios::binary);
    if (!outFile.is_open())
    {
        cerr << "Error: Could not write to STL file: " << filename << "\n";
        return;
    }

    const size_t BUFFER_SIZE = 1 << 20; // 1 MB buffer
    vector<char> buffer(BUFFER_SIZE);
    outFile.rdbuf()->pubsetbuf(buffer.data(), BUFFER_SIZE);

    outFile << "solid obj_to_stl\n";

    for (const auto& face : faces)
    {
        for (size_t i = 1; i + 1 < face.size(); ++i)
        {
            outFile << "  facet normal 0 0 0\n";
            outFile << "    outer loop\n";

            const auto& v0 = vertices[face[0]];
            const auto& v1 = vertices[face[i]];
            const auto& v2 = vertices[face[i + 1]];

            outFile << "      vertex " << v0[0] << " " << v0[1] << " " << v0[2] << "\n";
            outFile << "      vertex " << v1[0] << " " << v1[1] << " " << v1[2] << "\n";
            outFile << "      vertex " << v2[0] << " " << v2[1] << " " << v2[2] << "\n";

            outFile << "    endloop\n";
            outFile << "  endfacet\n";
        }
    }

    outFile << "endsolid obj_to_stl\n";
    outFile.close();
}

