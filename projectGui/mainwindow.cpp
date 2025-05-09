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
#include "PolygonWidget.h"
#include "BooleanOperationWidget.h"

#include <memory>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      stackedWidget(new QStackedWidget(this)),
      glWidget(new GLWidget(this)),
      bezierWidget(new BezierWidget(this))
{
    ui->setupUi(this);
    setWindowTitle("Shape Viewer - OpenGL & Qt");

    // Setup stacked widget
    stackedWidget->addWidget(glWidget);
    stackedWidget->addWidget(bezierWidget);
    ui->verticalLayout->replaceWidget(ui->openGLWidget, stackedWidget);
    delete ui->openGLWidget;

    // Button: Plot
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_plotButton_clicked);

    // Button: Upload
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::on_uploadButton_clicked);

    // Bezier Tool
    auto bezierButton = new QPushButton("Bezier Tool", this);
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, bezierButton);
    connect(bezierButton, &QPushButton::clicked, this, &MainWindow::openBezierWidget);

    // Extrusion Tool
    auto extrusionButton = new QPushButton("Extrusion Tool", this);
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, extrusionButton);
    connect(extrusionButton, &QPushButton::clicked, this, &MainWindow::openPolygonWidget);

    // Back to GL
    auto backButton = new QPushButton("Back to GL", this);
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, backButton);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::openGLWidget);

    // Boolean Operation Tool
    auto booleanOperationButton = new QPushButton("Boolean Operation Tool", this);
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, booleanOperationButton);
    connect(booleanOperationButton, &QPushButton::clicked, this, &MainWindow::openBooleanOperationWidget);

    // Shape dropdown population
    ui->Shapes->addItem("Cuboid");
    ui->Shapes->addItem("Sphere");
    ui->Shapes->addItem("Polygon");
    ui->Shapes->addItem("Polyline");
    ui->Shapes->addItem("Bezier");
    // Add more shape names here as needed
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
        auto edges = shape->getEdges();
        std::vector<QVector3D> shapeVertices;
        for (const auto& edge : edges) {
            shapeVertices.push_back(QVector3D(edge.first[0], edge.first[1], edge.first[2]));
            shapeVertices.push_back(QVector3D(edge.second[0], edge.second[1], edge.second[2]));
        }

        if (glWidget) {
            glWidget->setShapeVertices(edges);
            glWidget->update();
        } else {
            QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
        }
    } else {
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
        } else {
            QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
        }
    } else if (extension == "obj") {
        OBJShape objShape(filePath);
        auto triangles = objShape.getTriangles();
        if (glWidget) {
            glWidget->addTrianglesToEdges(triangles);
            glWidget->update();
        } else {
            QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
        }
    } else {
        QMessageBox::critical(this, "Unsupported File", "Only STL and OBJ files are supported.");
    }
    qDebug() << "File uploaded:" << fileName;

}

void MainWindow::openBezierWidget()
{
    stackedWidget->setCurrentWidget(bezierWidget);
}

void MainWindow::openGLWidget()
{
    stackedWidget->setCurrentWidget(glWidget);
}

void MainWindow::openPolygonWidget()
{
    QWidget* extrusionWindow = new QWidget();
    extrusionWindow->setWindowTitle("Extrusion Tool");
    extrusionWindow->resize(1020, 730);

    QHBoxLayout* mainLayout = new QHBoxLayout(extrusionWindow);
    PolygonWidget* polygonWidget = new PolygonWidget(extrusionWindow);
    polygonWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* rightPanel = new QVBoxLayout();
    QLabel* shapeLabel = new QLabel("Shapes List:", extrusionWindow);
    QListWidget* shapeList = new QListWidget(extrusionWindow);
    QPushButton* extrusionButton = new QPushButton("Extrusion", extrusionWindow);
    QPushButton* clearShapeButton = new QPushButton("Clear Shape", extrusionWindow);
    QPushButton* deleteShapeButton = new QPushButton("Delete Shape", extrusionWindow);

    rightPanel->addWidget(shapeLabel);
    rightPanel->addWidget(shapeList);
    rightPanel->addWidget(extrusionButton);
    rightPanel->addWidget(clearShapeButton);
    rightPanel->addWidget(deleteShapeButton);
    rightPanel->addStretch();

    mainLayout->addWidget(polygonWidget, 1);
    mainLayout->addLayout(rightPanel);

    connect(extrusionButton, &QPushButton::clicked, extrusionWindow, [polygonWidget, shapeList]() {
        QString shapeName = "Shape " + QString::number(shapeList->count() + 1);
        shapeList->addItem(shapeName);
        polygonWidget->addShape(shapeName);
    });

    connect(clearShapeButton, &QPushButton::clicked, extrusionWindow, [polygonWidget]() {
        polygonWidget->clearCurrentShape();
    });

    connect(deleteShapeButton, &QPushButton::clicked, extrusionWindow, [polygonWidget, shapeList]() {
        QListWidgetItem* selectedItem = shapeList->currentItem();
        if (selectedItem) {
            QString shapeName = selectedItem->text();
            polygonWidget->deleteShape(shapeName);
            delete shapeList->takeItem(shapeList->row(selectedItem));
        }
    });

    connect(shapeList, &QListWidget::itemClicked, extrusionWindow, [polygonWidget](QListWidgetItem* item) {
        polygonWidget->displayShape(item->text());
    });

    connect(shapeList, &QListWidget::itemDoubleClicked, extrusionWindow, [](QListWidgetItem* item) {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    });

    extrusionWindow->setAttribute(Qt::WA_DeleteOnClose);
    extrusionWindow->show();
}

void MainWindow::openBooleanOperationWidget()
{
    auto* booleanWidget = new BooleanOperationWidget();
    booleanWidget->setAttribute(Qt::WA_DeleteOnClose);
    booleanWidget->show();
}
