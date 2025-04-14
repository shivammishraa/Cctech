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

    vector<vector<vector<double>>> getCuboidTriangles()
    {
        return triangles;
    }
};

#endif
