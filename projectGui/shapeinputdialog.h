#ifndef SHAPEINPUTDIALOG_H
#define SHAPEINPUTDIALOG_H

#include <QDialog>
#include <QSpinBox>

class ShapeInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShapeInputDialog(QWidget* parent = nullptr);
    ~ShapeInputDialog();

    double getLength() const;
    double getWidth() const;
    double getHeight() const;

private:
    QSpinBox* lengthSpinBox;
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;
};

#endif // SHAPEINPUTDIALOG_H
