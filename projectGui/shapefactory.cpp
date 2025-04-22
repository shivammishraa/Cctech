#include "shapefactory.h"
#include "cuboid.h"
#include "sphere.h"
#include "polygon.h"
#include "polyline.h"
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

    else if (shapeName == "Polygon") {
        ShapeInputDialog dialog(shapeName, parent);
        dialog.setWindowTitle("Polygon Input");

        if (dialog.exec() == QDialog::Accepted) {
            int sides = dialog.getIntValue("sides");

            if (sides >= 3) {
                std::vector<std::vector<double>> vertices;

                for (int i = 0; i < sides; ++i) {
                    QString label = QString("P%1 - ").arg(i + 1);
                    double x = dialog.getValue(label + "X:");
                    double y = dialog.getValue(label + "Y:");
                    double z = dialog.getValue(label + "Z:");
                    vertices.push_back({ x, y, z });
                }

                std::shared_ptr<Shape3D> polygon = std::make_shared<Polygon>(vertices);
                return polygon;

            }
            else {
                QMessageBox::warning(parent, "Invalid Input", "A polygon must have at least 3 vertices.");
            }
        }
    }
    else if (shapeName == "Polyline") {
        ShapeInputDialog dialog(shapeName, parent);
        dialog.setWindowTitle("Polyline Input");

        if (dialog.exec() == QDialog::Accepted) {
            int count = dialog.getIntValue("points");
            if (count >= 2) {
                std::vector<std::vector<double>> vertices;
                for (int i = 0; i < count; ++i) {
                    QString label = QString("P%1 - ").arg(i + 1);
                    double x = dialog.getValue(label + "X:");
                    double y = dialog.getValue(label + "Y:");
                    double z = dialog.getValue(label + "Z:");
                    vertices.push_back({ x, y, z });
                }

                auto polyline = std::make_shared<Polyline>();
                for (const auto& p : dialog.getPolylinePoints()) {
                    polyline->addPoint(p.x(), p.y(), p.z());
                }

                return polyline;
                qDebug() << "Creating Polyline with" << vertices.size() << "points.";
            }
            else {
                QMessageBox::warning(parent, "Invalid Input", "A polyline requires at least 2 points.");
            }
        }
    }

    return nullptr; 
}
