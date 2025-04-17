#include "shapefactory.h"
#include "cuboid.h"
#include "sphere.h"
#include "bezier.h"
#include "cylinder.h"
#include "shapeinputdialog.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

std::shared_ptr<Shape3D> createShapeFromDialog(const QString& shapeName, QWidget* parent) {
    if (shapeName == "Cuboid") {
        ShapeInputDialog dialog(shapeName, parent);
        if (dialog.exec() == QDialog::Accepted) {
            double l = dialog.getValue("length");
            double w = dialog.getValue("width");
            double h = dialog.getValue("height");
            if (l > 0 && w > 0 && h > 0)
                return std::make_shared<Cuboid>(l, w, h);
        }
    }
    else if (shapeName == "Sphere") {
        ShapeInputDialog dialog(shapeName, parent);
        if (dialog.exec() == QDialog::Accepted) {
            double r = dialog.getValue("radius");
            int seg = dialog.getIntValue("segments");
            if (r > 0 && seg >= 4)
                return std::make_shared<Sphere>(r, seg);
        }
    }
    else if (shapeName == "Cylinder") {
        ShapeInputDialog dialog(shapeName, parent);
        if (dialog.exec() == QDialog::Accepted) {
            double r = dialog.getValue("radius");
            double h = dialog.getValue("height");
            int seg = dialog.getIntValue("segments");
            if (r > 0 && h > 0 && seg >= 4)
                return std::make_shared<Cylinder>(r, h, seg);
        }
    }
    else if (shapeName == "Bezier") {
        ShapeInputDialog dialog(shapeName, parent);
        dialog.setWindowTitle("Bezier Curve Input");

        if (dialog.exec() == QDialog::Accepted) {
            QVector<QVector3D> controlPoints = dialog.getControlPoints();
            int interpolationCount = dialog.getInterpolationPointCount();

            if (controlPoints.size() >= 2) {
                auto bezier = std::make_shared<Bezier>();
                for (const QVector3D& pt : controlPoints) {
                    bezier->addControlPoint(pt.x(), pt.y(), pt.z());
                }
                bezier->setInterpolationCount(interpolationCount);  
                return bezier;
            }
            else {
                QMessageBox::warning(parent, "Invalid Input", "A Bezier curve requires at least 2 control points.");
            }
        }
    }

    return nullptr; 
}
