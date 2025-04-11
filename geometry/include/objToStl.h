#ifndef OBJ_TO_STL_CONVERTER_H
#define OBJ_TO_STL_CONVERTER_H

#include <string>
#include <vector>
#include <array>
#include "stl_shape.h" // Include STLShape for plotting

class ObjToStlConverter
{
public:
    // Calls convert() and loads STLShape for future plotting
    void convertAndPlot(const std::string &objFilename, const std::string &stlFilename);
    
    // Reads OBJ, deduplicates vertices, and writes STL
    static bool convert(const std::string &objFilename, const std::string &stlFilename);

private:
    // Reads vertices and faces from OBJ file, deduplicates vertices
    static void readObj(const std::string &filename,
                        std::vector<std::array<double, 3>> &dedupedVertices,
                        std::vector<std::vector<int>> &faces);

    // Writes ASCII STL file from vertices and faces
    static void writeStl(const std::vector<std::array<double, 3>> &vertices,
                         const std::vector<std::vector<int>> &faces,
                         const std::string &filename);
};

#endif // OBJ_TO_STL_CONVERTER_H

