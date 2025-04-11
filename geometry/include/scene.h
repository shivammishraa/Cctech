#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <string>
#include "shape.h"

using namespace std;

class Scene {
private:
    vector<shared_ptr<Shape>> shapes; // Store shapes as shared pointers

public:
    Scene();

    void addShape(const shared_ptr<Shape>& shape); // Add a shape to the scene
    void plotScene(const string& filename) const;  // Plot all shapes in the scene

    // New method to access shapes
    vector<shared_ptr<Shape>>& getShapes(); // Get reference to the list of shapes
};

#endif 