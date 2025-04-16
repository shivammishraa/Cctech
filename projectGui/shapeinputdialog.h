#ifndef SHAPEINPUTDIALOG_H
#define SHAPEINPUTDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QMap>

class ShapeInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShapeInputDialog(const QString& shapeType, QWidget *parent = nullptr);

    double getValue(const QString& field) const;
    int getIntValue(const QString& field) const;

private:
    QString currentShape;
    QFormLayout* formLayout;
    QMap<QString, QWidget*> inputs;
};

#endif // SHAPEINPUTDIALOG_H
