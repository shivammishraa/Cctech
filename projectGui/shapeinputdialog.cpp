#include "shapeinputdialog.h"
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector3D>

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
    else if (shapeType == "Bezier") {
        bezierCountSpinBox = new QSpinBox;
        bezierCountSpinBox->setRange(2, 50);
        bezierCountSpinBox->setValue(4);
        formLayout->addRow("Control Point Count:", bezierCountSpinBox);

        interpolationPointsSpinBox = new QSpinBox;
        interpolationPointsSpinBox->setRange(10, 1000);
        interpolationPointsSpinBox->setValue(100);
        formLayout->addRow("Interpolation Points:", interpolationPointsSpinBox);

        connect(bezierCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeInputDialog::updateBezierInputs);

        updateBezierInputs(bezierCountSpinBox->value());
    }
    else if (shapeType == "Polygon") {
        QSpinBox* sidesSpinBox = new QSpinBox;
        sidesSpinBox->setRange(3, 50);  // At least 3 sides
        sidesSpinBox->setValue(3);      // Default

        formLayout->addRow("Number of Sides:", sidesSpinBox);
        inputs["sides"] = sidesSpinBox;

        polygonSidesSpinBox = sidesSpinBox;

        connect(sidesSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShapeInputDialog::updatePolygonInputs);

        updatePolygonInputs(sidesSpinBox->value());  // Initial call
    }

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ShapeInputDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShapeInputDialog::reject);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

// ? Properly moved outside constructor
void ShapeInputDialog::updateBezierInputs(int count) {
    for (int i = 0; i < bezierInputsX.size(); ++i) {
        formLayout->removeRow(formLayout->rowCount() - 1);
        formLayout->removeRow(formLayout->rowCount() - 1);
        formLayout->removeRow(formLayout->rowCount() - 1);
    }
    bezierInputsX.clear();
    bezierInputsY.clear();
    bezierInputsZ.clear();

    for (int i = 0; i < count; ++i) {
        QDoubleSpinBox* x = new QDoubleSpinBox; x->setRange(-10000, 10000); x->setValue(i * 1.0);
        QDoubleSpinBox* y = new QDoubleSpinBox; y->setRange(-10000, 10000); y->setValue(0);
        QDoubleSpinBox* z = new QDoubleSpinBox; z->setRange(-10000, 10000); z->setValue(0);

        formLayout->addRow(QString("P%1 - X:").arg(i + 1), x);
        formLayout->addRow(QString("P%1 - Y:").arg(i + 1), y);
        formLayout->addRow(QString("P%1 - Z:").arg(i + 1), z);

        bezierInputsX.append(x);
        bezierInputsY.append(y);
        bezierInputsZ.append(z);
    }
}

void ShapeInputDialog::updatePolygonInputs(int count) {
    // Clear old widgets from layout
    while (formLayout->rowCount() > 1) {  // Keep first row (Number of Sides)
        QLayoutItem* item = formLayout->takeAt(formLayout->rowCount() - 1);
        if (item) {
            QWidget* widget = item->widget();
            if (widget) widget->deleteLater();
            delete item;
        }
    }

    // Also clear from inputs map
    QStringList keysToRemove;
    for (const QString& key : inputs.keys()) {
        if (key.startsWith("P")) {
            keysToRemove << key;
        }
    }
    for (const QString& key : keysToRemove) {
        inputs.remove(key);
    }

    polygonInputsX.clear();
    polygonInputsY.clear();
    polygonInputsZ.clear();

    for (int i = 0; i < count; ++i) {
        QDoubleSpinBox* x = new QDoubleSpinBox; x->setRange(-10000, 10000); x->setValue(i);
        QDoubleSpinBox* y = new QDoubleSpinBox; y->setRange(-10000, 10000); y->setValue(0);
        QDoubleSpinBox* z = new QDoubleSpinBox; z->setRange(-10000, 10000); z->setValue(0);

        QString labelPrefix = QString("P%1 - ").arg(i + 1);
        QString xKey = labelPrefix + "X:";
        QString yKey = labelPrefix + "Y:";
        QString zKey = labelPrefix + "Z:";

        formLayout->addRow(xKey, x); inputs[xKey] = x;
        formLayout->addRow(yKey, y); inputs[yKey] = y;
        formLayout->addRow(zKey, z); inputs[zKey] = z;

        polygonInputsX.append(x);
        polygonInputsY.append(y);
        polygonInputsZ.append(z);
    }
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

QVector<QVector3D> ShapeInputDialog::getControlPoints() const {
    QVector<QVector3D> controlPoints;
    int count = bezierInputsX.size();
    for (int i = 0; i < count; ++i) {
        float x = bezierInputsX[i]->value();
        float y = bezierInputsY[i]->value();
        float z = bezierInputsZ[i]->value();
        controlPoints.append(QVector3D(x, y, z));
    }
    return controlPoints;
}

int ShapeInputDialog::getInterpolationPointCount() const {
    return interpolationPointsSpinBox ? interpolationPointsSpinBox->value() : 100;
}
