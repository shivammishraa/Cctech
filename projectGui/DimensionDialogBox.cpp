//#include "DimensionDialogBox.h"
//#include <QLabel>
//#include <QVBoxLayout>
//#include <QPushButton>
//
//DimensionDialogBox::DimensionDialogBox(QWidget* parent)
//    : QDialog(parent),
//    widthLineEdit(new QLineEdit(this)),
//    heightLineEdit(new QLineEdit(this)),
//    depthLineEdit(new QLineEdit(this)),
//    okButton(new QPushButton("OK", this)),
//    cancelButton(new QPushButton("Cancel", this))
//{
//    // Set up dialog layout
//    QVBoxLayout* layout = new QVBoxLayout(this);
//
//    layout->addWidget(new QLabel("Width:", this));
//    layout->addWidget(widthLineEdit);
//
//    layout->addWidget(new QLabel("Height:", this));
//    layout->addWidget(heightLineEdit);
//
//    layout->addWidget(new QLabel("Depth:", this));
//    layout->addWidget(depthLineEdit);
//
//    layout->addWidget(okButton);
//    layout->addWidget(cancelButton);
//
//    // Connect buttons
//    connect(okButton, &QPushButton::clicked, this, &DimensionDialogBox::accept);
//    connect(cancelButton, &QPushButton::clicked, this, &DimensionDialogBox::reject);
//}
//
//double DimensionDialogBox::getWidth() const
//{
//    return widthLineEdit->text().toDouble();  // Convert the text input to double
//}
//
//double DimensionDialogBox::getHeight() const
//{
//    return heightLineEdit->text().toDouble();  // Convert the text input to double
//}
//
//double DimensionDialogBox::getDepth() const
//{
//    return depthLineEdit->text().toDouble();  // Convert the text input to double
//}
