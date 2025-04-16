#include "shapeinputdialog.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>

ShapeInputDialog::ShapeInputDialog(const QString& shapeType, QWidget* parent)
    : QDialog(parent), currentShape(shapeType) {

    setWindowTitle("Enter " + shapeType + " Parameters");
    formLayout = new QFormLayout;

    if (shapeType == "Cuboid") {
        QDoubleSpinBox* length = new QDoubleSpinBox; length->setRange(0.1, 10000); length->setValue(1.0);
        QDoubleSpinBox* width = new QDoubleSpinBox; width->setRange(0.1, 10000); width->setValue(1.0);
        QDoubleSpinBox* height = new QDoubleSpinBox; height->setRange(0.1, 10000); height->setValue(1.0);
        formLayout->addRow("Length:", length); inputs["length"] = length;
        formLayout->addRow("Width:", width); inputs["width"] = width;
        formLayout->addRow("Height:", height); inputs["height"] = height;
    }
    else if (shapeType == "Sphere") {
        QDoubleSpinBox* radius = new QDoubleSpinBox; radius->setRange(0.1, 10000); radius->setValue(1.0);
        QSpinBox* segments = new QSpinBox; segments->setRange(4, 200); segments->setValue(24);
        formLayout->addRow("Radius:", radius); inputs["radius"] = radius;
        formLayout->addRow("Segments:", segments); inputs["segments"] = segments;
    }
    else if (shapeType == "Cylinder") {
        QDoubleSpinBox* radius = new QDoubleSpinBox; radius->setRange(0.1, 10000); radius->setValue(1.0);
        QDoubleSpinBox* height = new QDoubleSpinBox; height->setRange(0.1, 10000); height->setValue(2.0);
        QSpinBox* segments = new QSpinBox; segments->setRange(4, 200); segments->setValue(24);
        formLayout->addRow("Radius:", radius); inputs["radius"] = radius;
        formLayout->addRow("Height:", height); inputs["height"] = height;
        formLayout->addRow("Segments:", segments); inputs["segments"] = segments;
    }
    // Add more shapes here like Polygon, Triangle etc.

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

double ShapeInputDialog::getValue(const QString& field) const {
    if (inputs.contains(field)) {
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(inputs[field]);
        if (spinBox) return spinBox->value();
    }
    return 0.0;
}

int ShapeInputDialog::getIntValue(const QString& field) const {
    if (inputs.contains(field)) {
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(inputs[field]);
        if (spinBox) return spinBox->value();
    }
    return 0;
}