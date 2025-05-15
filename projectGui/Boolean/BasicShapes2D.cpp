#include "BasicShapes2D.h"
#include <cmath>

// Each shape is a vector of faces, each face is a vector of QVector2D (vertices)

std::vector<std::vector<QVector2D>> BasicShapes2D::createSquare(float side)
{
    float h = side / 2.0f;
    std::vector<QVector2D> face = {
        QVector2D(-h, -h),
        QVector2D(h, -h),
        QVector2D(h, h),
        QVector2D(-h, h)};
    return {face};
}

std::vector<std::vector<QVector2D>> BasicShapes2D::createRectangle(float width, float height)
{
    float hw = width / 2.0f, hh = height / 2.0f;
    std::vector<QVector2D> face = {
        QVector2D(-hw, -hh),
        QVector2D(hw, -hh),
        QVector2D(hw, hh),
        QVector2D(-hw, hh)};
    return {face};
}

std::vector<std::vector<QVector2D>> BasicShapes2D::createCircle(float radius, int segments)
{
    std::vector<QVector2D> face;
    for (int i = 0; i < segments; ++i)
    {
        float angle = 2.0f * M_PI * float(i) / float(segments);
        face.emplace_back(radius * cos(angle), radius * sin(angle));
    }
    return {face};
}

std::vector<std::vector<QVector2D>> BasicShapes2D::createTriangle(float base, float height)
{
    float hb = base / 2.0f;
    std::vector<QVector2D> face = {
        QVector2D(-hb, 0),
        QVector2D(hb, 0),
        QVector2D(0, height)};
    return {face};
}

std::vector<std::vector<QVector2D>> BasicShapes2D::createPolygon(int sides, float radius)
{
    std::vector<QVector2D> face;
    for (int i = 0; i < sides; ++i)
    {
        float angle = 2.0f * M_PI * float(i) / float(sides);
        face.emplace_back(radius * cos(angle), radius * sin(angle));
    }
    return {face};
}

std::vector<std::vector<QVector2D>> BasicShapes2D::createShape(const QString &type, const std::vector<float> &params)
{
    if (type == "Square" && params.size() >= 1)
    {
        return createSquare(params[0]);
    }
    else if (type == "Rectangle" && params.size() >= 2)
    {
        return createRectangle(params[0], params[1]);
    }
    else if (type == "Circle" && params.size() >= 1)
    {
        return createCircle(params[0]);
    }
    else if (type == "Triangle" && params.size() >= 2)
    {
        return createTriangle(params[0], params[1]);
    }
    else if (type == "Polygon" && params.size() >= 2)
    {
        return createPolygon(static_cast<int>(params[0]), params[1]);
    }

    return {}; // Return empty shape if invalid input
}