#ifndef SHAPE_HANDLERS_H
#define SHAPE_HANDLERS_H
#include <string>

void handleCuboid();
void handleSphere();
void handleTriangle();
void handleCylinder();
void handlePolygon();
void handleBezier();
void handleLine3D();
void handlePolyline();
void handleScene();
void handleTriangularShapes(const std::string& filename);
void handleOBJtoSTL(const std::string& filename); // New function to handle OBJ conversion
#endif
