#ifndef STL_SHAPE_H
#define STL_SHAPE_H

#include <string>
#include <vector>

using namespace std;

class STLShape
{
private:
    vector<vector<vector<double>>> triangles; // Using vector instead of array

public:
    STLShape(const std::string& filepath);
    void saveToFile(const std::string& filename) const;
    void plot(const std::string& filename) const;

    // Returns the parsed triangles
    vector<vector<vector<double>>> getTriangles() const;
};

#endif
