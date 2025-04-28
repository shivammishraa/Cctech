#ifndef OBJ_SHAPE_H
#define OBJ_SHAPE_H

#include <vector>
#include <string>

class OBJShape
{
private:
    std::vector<std::vector<double>> vertices;
    std::vector<std::vector<int>> faces;

public:
    OBJShape(const std::string& filepath);
    std::vector<std::vector<std::vector<double>>> getTriangles() const;
};

#endif // OBJ_SHAPE_H
