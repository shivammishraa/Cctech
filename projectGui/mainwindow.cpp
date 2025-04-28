#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shapefactory.h"
#include "shapeinputdialog.h"
#include "cuboid.h"
#include "sphere.h"
#include "shape3d.h"
#include "stl_shape.h"
#include "obj_shape.h"
#include "bezierwidget.h"
#include <memory>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
using namespace std;
using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Ensure the UI is initialized
    setWindowTitle("Shape Viewer - OpenGL & Qt");

    glWidget = qobject_cast<GLWidget*>(ui->openGLWidget);

    // Connect buttons to their respective slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_plotButton_clicked);
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::on_uploadButton_clicked);

    // Add the "Bezier Tool" button programmatically
    QPushButton* bezierButton = new QPushButton("Bezier Tool", this);
    bezierButton->setObjectName("bezierButton"); // Set an object name for debugging
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, bezierButton); // Add before the spacer
    connect(bezierButton, &QPushButton::clicked, this, &MainWindow::openBezierWidget);

    qDebug() << "Buttons connected to slots.";

    // Populate shape combo box
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

        // Convert edges into QVector3D format for OpenGL
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
    else {
        QMessageBox::warning(this, "Invalid Input", "Failed to create the selected shape.");
    }
}

void MainWindow::on_uploadButton_clicked() {

    qDebug() << "Upload button clicked.";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("3D Files (*.stl *.obj);;All Files (*)"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "Please select a valid STL or OBJ file.");
        return;
    }

    std::string filePath = fileName.toStdString();
    size_t dotPos = filePath.find_last_of(".");
    if (dotPos == std::string::npos) {
        QMessageBox::critical(this, "Invalid File", "The selected file does not have a valid extension.");
        return;
    }

    std::string extension = filePath.substr(dotPos + 1);
    if (extension == "stl") {
        STLShape stlShape(filePath);
        auto triangles = stlShape.getTriangles();
        if (glWidget) {
            glWidget->addTrianglesToEdges(triangles);
            glWidget->update();
        }
        else {
            QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
        }
    }
    else if (extension == "obj") {
        OBJShape objShape(filePath);
        auto triangles = objShape.getTriangles();
        if (glWidget) {
            glWidget->addTrianglesToEdges(triangles);
            glWidget->update();
        }
        else {
            QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
        }
    }
    else {
        QMessageBox::critical(this, "Unsupported File", "Only STL and OBJ files are supported.");
    }
    qDebug() << "File uploaded:" << fileName;

}

void MainWindow::openBezierWidget()
{
    BezierWidget* bezierWidget = new BezierWidget();
    bezierWidget->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    bezierWidget->show();
}
