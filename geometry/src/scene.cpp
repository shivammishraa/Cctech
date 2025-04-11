#include "scene.h"
#include <fstream>
#include <iostream>

using namespace std;

Scene::Scene() {}

void Scene::addShape(const shared_ptr<Shape>& shape) {
    shapes.push_back(shape);
}

vector<shared_ptr<Shape>>& Scene::getShapes() {
    return shapes;
}

void Scene::plotScene(const string& filename) const {
    // Clear the file initially
    ofstream clearFile(filename, ios::app);
    if (!clearFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    clearFile.close();

    // Append each shape to the file
    for (const auto& shape : shapes) {
        shape->saveToFile(filename);
    }

    // Plot the scene using GNUPLOT
    string command = 
        "gnuplot -p -e \""
        "set terminal wxt; "
        "set xlabel 'X'; "
        "set ylabel 'Y'; "
        "set zlabel 'Z'; "
        "set view equal xyz; "
        "splot '" + filename + "' with linespoints pointtype 7 linecolor 'black'\"";
    system(command.c_str());
}
