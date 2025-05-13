#ifndef BASICSHAPES2D_H
#define BASICSHAPES2D_H

#include <vector>
#include <QVector2D>

// Each shape is a vector of faces, each face is a vector of QVector2D (vertices)
class BasicShapes2D {
public:
    static std::vector<std::vector<QVector2D>> createSquare(float side);
    static std::vector<std::vector<QVector2D>> createRectangle(float width, float height);
    static std::vector<std::vector<QVector2D>> createCircle(float radius, int segments = 64);
    static std::vector<std::vector<QVector2D>> createTriangle(float base, float height);
    static std::vector<std::vector<QVector2D>> createPolygon(int sides, float radius);

    static std::vector<std::vector<QVector2D>> createShape(const QString& type, const std::vector<float>& params);

};


#endif // BASICSHAPES2D_H
