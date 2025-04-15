#include "shapeinputdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

ShapeInputDialog::ShapeInputDialog(QWidget* parent)
    : QDialog(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* lengthLabel = new QLabel("Length:");
    lengthSpinBox = new QSpinBox();
    lengthSpinBox->setRange(1, 1000);
    layout->addWidget(lengthLabel);
    layout->addWidget(lengthSpinBox);

    QLabel* widthLabel = new QLabel("Width:");
    widthSpinBox = new QSpinBox();
    widthSpinBox->setRange(1, 1000);
    layout->addWidget(widthLabel);
    layout->addWidget(widthSpinBox);

    QLabel* heightLabel = new QLabel("Height:");
    heightSpinBox = new QSpinBox();
    heightSpinBox->setRange(1, 1000);
    layout->addWidget(heightLabel);
    layout->addWidget(heightSpinBox);

    QPushButton* okButton = new QPushButton("OK");
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(okButton);
}

ShapeInputDialog::~ShapeInputDialog() {}

double ShapeInputDialog::getLength() const {
    return lengthSpinBox->value();
}

double ShapeInputDialog::getWidth() const {
    return widthSpinBox->value();
}

double ShapeInputDialog::getHeight() const {
    return heightSpinBox->value();
}
