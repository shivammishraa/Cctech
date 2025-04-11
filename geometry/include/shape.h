#ifndef SHAPE_H
#define SHAPE_H

#include <string>

using namespace std;

class Shape {
public:
    virtual ~Shape() = default;

    // Pure virtual methods to be implemented by derived classes
    virtual void plot(const string& filename) const = 0;
    virtual void saveToFile(const string& filename) const = 0;

    // New pure virtual method for translation
    virtual void translate(double dx, double dy, double dz) = 0;
};

#endif // SHAPE_H
