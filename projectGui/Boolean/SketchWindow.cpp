#include "SketchWindow.h"
#include "SketchGLWidget.h"
#include "GLWidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>

SketchWindow::SketchWindow(QWidget* parent)
    : QMainWindow(parent) {
    glWidget = new SketchGLWidget(this);

    QPushButton* btnFinish = new QPushButton("Finish Shape");
    QPushButton* btnUnion = new QPushButton("Union");
    QPushButton* btnIntersect = new QPushButton("Intersection");
    QPushButton* btnSubtract = new QPushButton("Subtraction");
    QPushButton* btnClear = new QPushButton("Clear Result");

    connect(btnFinish, &QPushButton::clicked, this, &SketchWindow::handleFinishShape);
    connect(btnUnion, &QPushButton::clicked, this, &SketchWindow::handleUnion);
    connect(btnIntersect, &QPushButton::clicked, this, &SketchWindow::handleIntersection);
    connect(btnSubtract, &QPushButton::clicked, this, &SketchWindow::handleSubtraction);
    connect(btnClear, &QPushButton::clicked, glWidget, &SketchGLWidget::resetResult);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnFinish);
    btnLayout->addWidget(btnUnion);
    btnLayout->addWidget(btnIntersect);
    btnLayout->addWidget(btnSubtract);
    btnLayout->addWidget(btnClear);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addLayout(btnLayout);

    QWidget* central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    setWindowTitle("2D Sketch Tool");
    resize(800, 600);
}

void SketchWindow::handleFinishShape() { glWidget->completeShape(); }
void SketchWindow::handleUnion() { glWidget->performUnion(); }
void SketchWindow::handleIntersection() { glWidget->performIntersection(); }

void SketchWindow::handleSubtraction() {
    QStringList choices = { "A - B", "B - A" };
    bool ok;
    QString selected = QInputDialog::getItem(this, "Subtraction", "Choose operation:", choices, 0, false, &ok);
    if (ok && !selected.isEmpty()) {
        if (selected == "A - B") {
            glWidget->performSubtraction(true);
        } else if (selected == "B - A") {
            glWidget->performSubtraction(false);
        }
    }
}

