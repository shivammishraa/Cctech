#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <QString>
#include <memory>
#include "shape3d.h"
#include "ui_mainwindow.h"
#include <QWidget>

std::shared_ptr<Shape3D> createShapeFromDialog(const QString& shapeName, QWidget* parent);

#endif // SHAPEFACTORY_H
