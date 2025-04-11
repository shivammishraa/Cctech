#ifndef STL_SHAPE_H
#define STL_SHAPE_H

#include <string>
#include <vector>
#include <array>

class STLShape
{
private:
    std::vector<std::array<std::array<double, 3>, 3>> triangles;

public:
    STLShape(const std::string &filepath);
    void saveToFile(const std::string &filename) const;
    void plot(const std::string &filename) const;

    const std::vector<std::array<std::array<double, 3>, 3>> &getTriangles() const
    {
        return triangles;
    }
};

#endif
