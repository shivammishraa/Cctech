#include "shapefactory.h"
#include "cuboid.h"
#include "sphere.h"
#include "cylinder.h"
#include "shapeinputdialog.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

std::shared_ptr<Shape3D> createShapeFromDialog(const QString& shapeName, QWidget* parent) {
    ShapeInputDialog dialog(shapeName, parent);
    if (dialog.exec() == QDialog::Accepted) {
        if (shapeName == "Cuboid") {
            double l = dialog.getValue("length");
            double w = dialog.getValue("width");
            double h = dialog.getValue("height");
            if (l > 0 && w > 0 && h > 0)
                return std::make_shared<Cuboid>(l, w, h);
        }
        else if (shapeName == "Sphere") {
            double r = dialog.getValue("radius");
            int seg = dialog.getIntValue("segments");
            if (r > 0 && seg >= 4)
                return std::make_shared<Sphere>(r, seg);
        }
        else if (shapeName == "Cylinder") {
            double r = dialog.getValue("radius");
            double h = dialog.getValue("height");
            int seg = dialog.getIntValue("segments");
            if (r > 0 && h > 0 && seg >= 4)
                return std::make_shared<Cylinder>(r, h, seg);
        }
    }
    return nullptr;
}
