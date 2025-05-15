#ifndef SHAPE3D_H
#define SHAPE3D_H

#include <vector>
#include <utility>
#include <QVector3D>
#include <QVector>

class Shape3D
{
public:
    virtual ~Shape3D() = default;
    virtual std::vector<std::pair<std::vector<double>, std::vector<double>>> getEdges() const = 0;

    // virtual std::vector<std::vector<QVector3D>> getFaces() const = 0;
};

#endif // SHAPE3D_H
