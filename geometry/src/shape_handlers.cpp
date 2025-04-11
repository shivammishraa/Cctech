#include "../include/shape_handlers.h" 
#include <iostream>
#include <fstream>
#include <memory>
#include "transform_utils.h"
#include "plot_utils.h"
#include "stl_shape.h"
#include "cuboid.h"
#include "sphere.h"
#include "triangle.h"
#include "cylinder.h"
#include "polygon.h"
#include "polyline.h"
#include "bezier.h"
#include "line3D.h"
#include "scene.h"
#include "objToStl.h"

using namespace std;

void handleCuboid() {
    double length, width, height;
    cout << "Enter length, width, height: ";
    cin >> length >> width >> height;

    auto cuboid = make_shared<Cuboid>(length, width, height);
    cuboid->plot("data/original_cuboid.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            cuboid->translate(dx, dy, dz);
            cuboid->plot("data/transformed_cuboid.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            cuboid->rotate(angle, axis);
            cuboid->plot("data/transformed_cuboid.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            cuboid->scale(sx, sy, sz);
            cuboid->plot("data/transformed_cuboid.dat");
        }
    } while (transformChoice != 0);
}

void handleSphere() {
    double radius;
    int segments;
    cout << "Enter radius and number of segments: ";
    cin >> radius >> segments;

    auto sphere = make_shared<Sphere>(radius, segments);
    sphere->plot("data/original_sphere.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Scale\n";
        cout << "3. Rotate\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            sphere->translate(dx, dy, dz);
            sphere->plot("data/transformed_sphere.dat");
        } else if (transformChoice == 2) {
            double factor;
            cout << "Enter scaling factor: ";
            cin >> factor;
            sphere->scale(factor);
            sphere->plot("data/transformed_sphere.dat");
        } else if (transformChoice == 3) {
            double angle;
            char axis;
            cout << "Enter rotation angle (in degrees) and axis (x/y/z): ";
            cin >> angle >> axis;
            sphere->rotate(angle, axis);
            sphere->plot("data/transformed_sphere.dat");
        }
    } while (transformChoice != 0);
}

void handleTriangle() {
    double x1, y1, z1, x2, y2, z2, x3, y3, z3;
    cout << "Enter vertices (x1 y1 z1), (x2 y2 z2), (x3 y3 z3): ";
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

    auto triangle = make_shared<Triangle>(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    triangle->plot("data/original_triangle.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            triangle->translate(dx, dy, dz);
            triangle->plot("data/transformed_triangle.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            triangle->rotate(angle, axis, 0, 0, 0); // Assuming rotation around origin
            triangle->plot("data/transformed_triangle.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            triangle->scale(sx, sy, sz);
            triangle->plot("data/transformed_triangle.dat");
        }
    } while (transformChoice != 0);
}

void handleCylinder() {
    double radius, height;
    int resolution;
    cout << "Enter radius, height, and resolution: ";
    cin >> radius >> height >> resolution;

    auto cylinder = make_shared<Cylinder>(radius, height, resolution);
    cylinder->plot("data/original_cylinder.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            cylinder->translate(dx, dy, dz);
            cylinder->plot("data/transformed_cylinder.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            cylinder->rotate(angle, axis);
            cylinder->plot("data/transformed_cylinder.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            cylinder->scale(sx, sy, sz);
            cylinder->plot("data/transformed_cylinder.dat");
        }
    } while (transformChoice != 0);
}

void handlePolygon() {
    int sides;
    cout << "Enter number of sides: ";
    cin >> sides;

    auto polygon = make_shared<Polygon>(sides);
    for (int i = 0; i < sides; i++) {
        double x, y, z;
        cout << "Enter vertex " << i + 1 << " (x y z): ";
        cin >> x >> y >> z;
        polygon->addVertex(x, y, z);
    }
    polygon->plot("data/original_polygon.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            polygon->translate(dx, dy, dz);
            polygon->plot("data/transformed_polygon.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            polygon->rotate(angle, axis);
            polygon->plot("data/transformed_polygon.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            polygon->scale(sx, sy, sz);
            polygon->plot("data/transformed_polygon.dat");
        }
    } while (transformChoice != 0);
}

void handleBezier() {
    int numPoints;
    cout << "Enter number of control points: ";
    cin >> numPoints;

    auto bezier = make_shared<Bezier>();
    for (int i = 0; i < numPoints; i++) {
        double x, y, z;
        cout << "Enter control point " << i + 1 << " (x y z): ";
        cin >> x >> y >> z;
        bezier->addControlPoint(x, y, z);
    }
    bezier->plot("data/original_bezier.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            bezier->translate(dx, dy, dz);
            bezier->plot("data/transformed_bezier.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            bezier->rotate(angle, axis);
            bezier->plot("data/transformed_bezier.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            bezier->scale(sx, sy, sz);
            bezier->plot("data/transformed_bezier.dat");
        }
    } while (transformChoice != 0);
}

void handleLine3D() {
    double x1, y1, z1, x2, y2, z2;
    cout << "Enter endpoints (x1 y1 z1) and (x2 y2 z2): ";
    cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;

    auto line = make_shared<Line3D>();
    line->setPoints(x1, y1, z1, x2, y2, z2);
    line->plot("data/original_line3D.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            line->translate(dx, dy, dz);
            line->plot("data/transformed_line3D.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            line->rotate(angle, axis);
            line->plot("data/transformed_line3D.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            line->scale(sx, sy, sz);
            line->plot("data/transformed_line3D.dat");
        }
    } while (transformChoice != 0);
}

void handlePolyline() {
    int numPoints;
    cout << "Enter number of points for the polyline: ";
    cin >> numPoints;

    auto polyline = make_shared<Polyline>();
    for (int i = 0; i < numPoints; ++i) {
        double x, y, z;
        cout << "Enter point " << i + 1 << " (x y z): ";
        cin >> x >> y >> z;
        polyline->addPoint(x, y, z);
    }

    polyline->plot("data/original_polyline.dat");

    int transformChoice;
    do {
        cout << "Choose a transformation:\n";
        cout << "1. Translate\n";
        cout << "2. Rotate\n";
        cout << "3. Scale\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> transformChoice;

        if (transformChoice == 1) {
            double dx, dy, dz;
            cout << "Enter translation distances (dx, dy, dz): ";
            cin >> dx >> dy >> dz;
            polyline->translate(dx, dy, dz);
            polyline->plot("data/transformed_polyline.dat");
        } else if (transformChoice == 2) {
            double angle;
            char axis;
            cout << "Enter rotation angle and axis (x, y, or z): ";
            cin >> angle >> axis;
            polyline->rotate(angle, axis);
            polyline->plot("data/transformed_polyline.dat");
        } else if (transformChoice == 3) {
            double sx, sy, sz;
            cout << "Enter scaling factors (sx, sy, sz): ";
            cin >> sx >> sy >> sz;
            polyline->scale(sx, sy, sz);
            polyline->plot("data/transformed_polyline.dat");
        }
    } while (transformChoice != 0);
}

void handleScene() {
    Scene scene;
    int subChoice;
    do {
        cout << "Choose a shape to add to the scene:\n";
        cout << "1. Add Cuboid\n";
        cout << "2. Add Sphere\n";
        cout << "3. Add Triangle\n";
        cout << "4. Add Cylinder\n";
        cout << "5. Add Polygon\n";
        cout << "6. Add Bezier Curve\n";
        cout << "7. Add 3D Line\n";
        cout << "8. Add Polyline\n";
        cout << "0. Finish\n";
        cout << "Enter your choice: ";
        cin >> subChoice;

        if (subChoice == 1) {
            double x, y, z, length, width, height;
            cout << "Enter coordinates (x, y, z) where you want to plot the cuboid: ";
            cin >> x >> y >> z;
            cout << "Enter length, width, height: ";
            cin >> length >> width >> height;
            auto cuboid = make_shared<Cuboid>(length, width, height);
            cuboid->translate(x, y, z);
            scene.addShape(cuboid);
        } else if (subChoice == 2) {
            double x, y, z, radius;
            int segments;
            cout << "Enter coordinates (x, y, z) where you want to plot the sphere: ";
            cin >> x >> y >> z;
            cout << "Enter radius and number of segments: ";
            cin >> radius >> segments;
            auto sphere = make_shared<Sphere>(radius, segments);
            sphere->translate(x, y, z);
            scene.addShape(sphere);
        } else if (subChoice == 3) {
            double x, y, z, x1, y1, z1, x2, y2, z2, x3, y3, z3;
            cout << "Enter coordinates (x, y, z) where you want to plot the triangle: ";
            cin >> x >> y >> z;
            cout << "Enter vertices (x1 y1 z1), (x2 y2 z2), (x3 y3 z3): ";
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
            auto triangle = make_shared<Triangle>(x1, y1, z1, x2, y2, z2, x3, y3, z3);
            triangle->translate(x, y, z);
            scene.addShape(triangle);
        } else if (subChoice == 4) {
            double x, y, z, radius, height;
            int resolution;
            cout << "Enter coordinates (x, y, z) where you want to plot the cylinder: ";
            cin >> x >> y >> z;
            cout << "Enter radius, height, and resolution: ";
            cin >> radius >> height >> resolution;
            auto cylinder = make_shared<Cylinder>(radius, height, resolution);
            cylinder->translate(x, y, z);
            scene.addShape(cylinder);
        } else if (subChoice == 5) {
            double x, y, z;
            int sides;
            cout << "Enter coordinates (x, y, z) where you want to plot the polygon: ";
            cin >> x >> y >> z;
            cout << "Enter number of sides: ";
            cin >> sides;
            auto polygon = make_shared<Polygon>(sides);
            for (int i = 0; i < sides; i++) {
                double vx, vy, vz;
                cout << "Enter vertex " << i + 1 << " (x y z): ";
                cin >> vx >> vy >> vz;
                polygon->addVertex(vx, vy, vz);
            }
            polygon->translate(x, y, z);
            scene.addShape(polygon);
        } else if (subChoice == 6) {
            double x, y, z;
            int numPoints;
            cout << "Enter coordinates (x, y, z) where you want to plot the Bezier curve: ";
            cin >> x >> y >> z;
            cout << "Enter number of control points: ";
            cin >> numPoints;
            auto bezier = make_shared<Bezier>();
            for (int i = 0; i < numPoints; i++) {
                double vx, vy, vz;
                cout << "Enter control point " << i + 1 << " (x y z): ";
                cin >> vx >> vy >> vz;
                bezier->addControlPoint(vx, vy, vz);
            }
            bezier->translate(x, y, z);
            scene.addShape(bezier);
        } else if (subChoice == 7) {
            double x, y, z, x1, y1, z1, x2, y2, z2;
            cout << "Enter coordinates (x, y, z) where you want to plot the line: ";
            cin >> x >> y >> z;
            cout << "Enter endpoints (x1 y1 z1) and (x2 y2 z2): ";
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            auto line = make_shared<Line3D>();
            line->setPoints(x1, y1, z1, x2, y2, z2);
            line->translate(x, y, z);
            scene.addShape(line);
        } else if (subChoice == 8) {
            double x, y, z;
            int numPoints;
            cout << "Enter coordinates (x, y, z) where you want to place the polyline: ";
            cin >> x >> y >> z;
            cout << "Enter number of points for the polyline: ";
            cin >> numPoints;
        
            auto polyline = make_shared<Polyline>();
            for (int i = 0; i < numPoints; ++i) {
                double px, py, pz;
                cout << "Enter point " << i + 1 << " (x y z): ";
                cin >> px >> py >> pz;
                polyline->addPoint(px, py, pz);
            }
        
            polyline->translate(x, y, z);
            scene.addShape(polyline);
        }
        if (subChoice != 0) {
            cout << "Do you want to add more shapes? (1 for Yes, 0 for No): ";
            cin >> subChoice;
        }
    } while (subChoice != 0);

    scene.plotScene("data/scene.dat");
}

void handleTriangularShapes(const string& filename) {
    string filepath = filename;

    ifstream fileCheck(filepath);
    if (!fileCheck) {
        cerr << "Error: STL file '" << filename << "' not found!\n";
        return;
    }
    fileCheck.close();

    STLShape shape(filepath);

    string cleanFilename = filename.substr(filename.find_last_of("/\\") + 1); 
    string outputDataFile = "data/" + cleanFilename.substr(0, cleanFilename.rfind(".")) + ".dat";

    cout << "\n→ Plotting STL file: " << filename << "\n";
    cout << "→ Saving parsed data to: " << outputDataFile << "\n";

    shape.plot(outputDataFile);


    
        // Save and plot again after transformations
        // shape.saveToFile(outputDataFile);
        // plotWithGnuplot(outputDataFile, "red", 2);
    } 



void handleOBJtoSTL(const string& filename) {
    string objPath = "assets/" + filename;
    string stlOutput = "assets/" + filename.substr(0, filename.rfind(".")) + ".stl";

    ifstream objCheck(objPath);
    if (!objCheck) {
        cerr << "Error: OBJ file '" << filename << "' not found!\n";
        return;
    }
    objCheck.close();

    cout << "Converting OBJ to STL: " << filename << " -> " << stlOutput << "\n";

    ObjToStlConverter converter;
    
    if (converter.convert(objPath, stlOutput)) {
        cout << "Conversion successful! STL file saved as: " << stlOutput << "\n";
    } else {
        cerr << "Error: Conversion failed.\n";
    }
    cout << "Debug: STL conversion finished, no auto-plotting should happen.\n";

}
