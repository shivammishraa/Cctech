#include <iostream>
#include <map>
#include <functional>
#include "../../geometry/include/shape_handlers.h"
#include "../../geometry/include/objToStl.h"

using namespace std;

void menu() {
    cout << "Select an option:\n";
    cout << "1. Plot Shapes\n";
    cout << "2. Plot STL File\n";
    cout << "3. Convert OBJ to STL\n";
    cout << "Enter choice: ";
}

void shapeMenu() {
    cout << "Select a shape to plot:\n";
    cout << "1. Cuboid\n";
    cout << "2. Sphere\n";
    cout << "3. Triangle\n";
    cout << "4. Cylinder\n";
    cout << "5. Polygon\n";
    cout << "6. Bezier Curve\n";
    cout << "7. 3D Line\n";
    cout << "8. Polyline\n";
    cout << "9. Scene (Combination of Shapes)\n";
    cout << "Enter choice: ";
}

void handleSTLFile() {
    string filename;
    cout << "Enter STL file name (from assets/): ";
    cin >> filename;
    handleTriangularShapes("assets/" + filename);
}

void handleOBJToSTLConversion() {
    string objFilename, stlFilename;
    
    cout << "Enter OBJ file name (from assets/): ";
    cin >> objFilename;
    
    cout << "Enter STL output file name (to be saved in assets/): ";
    cin >> stlFilename;

    string objPath = "assets/" + objFilename;
    string stlPath = "assets/" + stlFilename;

    ObjToStlConverter converter;
    converter.convertAndPlot(objPath, stlPath);
}

int main() {
    map<int, function<void()>> shapeHandlers = {
        {1, handleCuboid},
        {2, handleSphere},
        {3, handleTriangle},
        {4, handleCylinder},
        {5, handlePolygon},
        {6, handleBezier},
        {7, handleLine3D},
        {8, handlePolyline},
        {9, handleScene}
    };

    menu();
    int mainChoice;
    cin >> mainChoice;

    if (mainChoice == 1) {
        shapeMenu();
        int shapeChoice;
        cin >> shapeChoice;

        if (shapeHandlers.find(shapeChoice) != shapeHandlers.end()) {
            shapeHandlers[shapeChoice]();
        } else {
            cout << "Invalid choice!\n";
        }
    } else if (mainChoice == 2) {
        handleSTLFile();
    } else if (mainChoice == 3) {
        handleOBJToSTLConversion();
    } else {
        cout << "Invalid choice!\n";
    }

    return 0;
}
