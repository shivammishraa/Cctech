#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shapefactory.h"
#include "shapeinputdialog.h"
#include "cuboid.h"
#include "sphere.h"
#include "shape3d.h"
#include <memory>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Shape Viewer - OpenGL & Qt");

    glWidget = qobject_cast<GLWidget*>(ui->openGLWidget);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_plotButton_clicked);

    // Populate shape combo box
   /* ui->Shapes->addItem("Cuboid");
    ui->Shapes->addItem("Sphere");*/
    ui->Shapes->addItem("Bezier");
    // Add more shapes here like: ui->Shapes->addItem("Cylinder");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_plotButton_clicked()
{
    QString selectedShape = ui->Shapes->currentText();
    auto shape = createShapeFromDialog(selectedShape, this);

    if (shape) {
        vector<pair<vector<double>, vector<double>>> edges = shape->getEdges();

        // Convert edges into QVector3D f ormat for OpenGL
        std::vector<QVector3D> shapeVertices;
        for (const auto& edge : edges) {
            shapeVertices.push_back(QVector3D(edge.first[0], edge.first[1], edge.first[2]));
            shapeVertices.push_back(QVector3D(edge.second[0], edge.second[1], edge.second[2]));
        }

        // Pass the vertices to GLWidget

        if (glWidget) {
            glWidget->setShapeVertices(edges);
            glWidget->update();
        }
        else {
            QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
        }
    }
}
