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
#include "PolygonWidget.h" // Include PolygonWidget header
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
using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    stackedWidget(new QStackedWidget(this)),
    glWidget(new GLWidget(this)),
    bezierWidget(new BezierWidget(this))
{
    ui->setupUi(this); // Ensure the UI is initialized
    setWindowTitle("Shape Viewer - OpenGL & Qt");

    // Add GLWidget and BezierWidget to the stacked widget
    stackedWidget->addWidget(glWidget);
    stackedWidget->addWidget(bezierWidget);

    // Replace the OpenGL widget in the UI with the stacked widget
    ui->verticalLayout->replaceWidget(ui->openGLWidget, stackedWidget);
    delete ui->openGLWidget; // Remove the old OpenGL widget

    // Connect buttons to their respective slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_plotButton_clicked);
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::on_uploadButton_clicked);

    // Add the "Bezier Tool" button programmatically
    QPushButton* bezierButton = new QPushButton("Bezier Tool", this);
    bezierButton->setObjectName("bezierButton"); // Set an object name for debugging
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, bezierButton); // Add before the spacer
    connect(bezierButton, &QPushButton::clicked, this, &MainWindow::openBezierWidget);

    // Add the "Extrusion Tool" button programmatically
    QPushButton* extrusionButton = new QPushButton("Extrusion Tool", this);
    extrusionButton->setObjectName("extrusionButton"); // Set an object name for debugging
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, extrusionButton); // Add before the spacer
    connect(extrusionButton, &QPushButton::clicked, this, &MainWindow::openPolygonWidget);

    // Add a "Back to GL" button programmatically
    QPushButton* backButton = new QPushButton("Back to GL", this);
    backButton->setObjectName("backButton"); // Set an object name for debugging
    ui->topBarLayout->insertWidget(ui->topBarLayout->count() - 1, backButton); // Add before the spacer
    connect(backButton, &QPushButton::clicked, this, &MainWindow::openGLWidget);

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
    stackedWidget->setCurrentWidget(bezierWidget); // Switch to BezierWidget
}

void MainWindow::openGLWidget()
{
    stackedWidget->setCurrentWidget(glWidget); // Switch back to GLWidget
}

void MainWindow::openPolygonWidget()
{
    // Create a new window for the Extrusion Tool
    QWidget* extrusionWindow = new QWidget();
    extrusionWindow->setWindowTitle("Extrusion Tool");
    extrusionWindow->resize(1020, 730);

    // Create the main layout for the window
    QHBoxLayout* mainLayout = new QHBoxLayout(extrusionWindow);

    // Add the PolygonWidget (OpenGL widget)
    PolygonWidget* polygonWidget = new PolygonWidget(extrusionWindow);
    polygonWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Create the right panel with buttons and shape list
    QVBoxLayout* rightPanel = new QVBoxLayout();

    QLabel* shapeLabel = new QLabel("Shapes List:", extrusionWindow);
    QListWidget* shapeList = new QListWidget(extrusionWindow);
    QPushButton* extrusionButton = new QPushButton("Extrusion", extrusionWindow);
    QPushButton* clearShapeButton = new QPushButton("Clear Shape", extrusionWindow);
    QPushButton* deleteShapeButton = new QPushButton("Delete Shape", extrusionWindow);

    // Assemble the right panel
    rightPanel->addWidget(shapeLabel);
    rightPanel->addWidget(shapeList);
    rightPanel->addWidget(extrusionButton);
    rightPanel->addWidget(clearShapeButton);
    rightPanel->addWidget(deleteShapeButton);
    rightPanel->addStretch();

    // Add the PolygonWidget and right panel to the main layout
    mainLayout->addWidget(polygonWidget, 1);
    mainLayout->addLayout(rightPanel);

    // Connect the buttons to the PolygonWidget functionality
    connect(extrusionButton, &QPushButton::clicked, extrusionWindow, [polygonWidget, shapeList]() {
        QString shapeName = "Shape " + QString::number(shapeList->count() + 1);
        shapeList->addItem(shapeName);
        polygonWidget->addShape(shapeName);
        qDebug() << "Extrusion performed, shape added:" << shapeName;
    });

    connect(clearShapeButton, &QPushButton::clicked, extrusionWindow, [polygonWidget]() {
        polygonWidget->clearCurrentShape();
        qDebug() << "Current shape cleared. Ready to plot a new shape.";
    });

    connect(deleteShapeButton, &QPushButton::clicked, extrusionWindow, [polygonWidget, shapeList]() {
        QListWidgetItem* selectedItem = shapeList->currentItem();
        if (selectedItem) {
            QString shapeName = selectedItem->text();
            polygonWidget->deleteShape(shapeName);
            delete shapeList->takeItem(shapeList->row(selectedItem));
            qDebug() << "Shape deleted:" << shapeName;
        } else {
            qDebug() << "No shape selected to delete.";
        }
    });

    connect(shapeList, &QListWidget::itemClicked, extrusionWindow, [polygonWidget](QListWidgetItem* item) {
        polygonWidget->displayShape(item->text());
        qDebug() << "Shape displayed:" << item->text();
    });

    connect(shapeList, &QListWidget::itemDoubleClicked, extrusionWindow, [](QListWidgetItem* item) {
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    });

    // Show the Extrusion Tool window
    extrusionWindow->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    extrusionWindow->show();
}
