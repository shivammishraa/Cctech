#ifndef SHAPEINPUTDIALOG_H
#define SHAPEINPUTDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QMap>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVector>
#include <QPair>

class ShapeInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShapeInputDialog(const QString& shapeType, QWidget* parent = nullptr);
    double getValue(const QString& field) const;
    int getIntValue(const QString& field) const;

    // For Bezier
    QVector<QVector3D> getControlPoints() const;
    int getInterpolationPointCount() const;

private slots:
    void updateBezierInputs(int count);
    void updatePolygonInputs(int count);

private:
    QString currentShape;
    QFormLayout* formLayout;
    QMap<QString, QWidget*> inputs;

    // For Bezier
    QVector<QDoubleSpinBox*> bezierInputsX;
    QVector<QDoubleSpinBox*> bezierInputsY;
    QVector<QDoubleSpinBox*> bezierInputsZ;
    QSpinBox* bezierCountSpinBox = nullptr;
    QSpinBox* interpolationPointsSpinBox = nullptr;
	// For Polygon
    QSpinBox* polygonSidesSpinBox;
	QVector<QDoubleSpinBox*> polygonInputsX;
	QVector<QDoubleSpinBox*> polygonInputsY;
	QVector<QDoubleSpinBox*> polygonInputsZ;


};

#endif // SHAPEINPUTDIALOG_H
