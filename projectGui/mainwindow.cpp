#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stl_shape.h"
#include "cuboid.h"  // Assuming this exists

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("STL Viewer - OpenGL & Qt");

    glWidget = ui->openGLWidget;

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onShapeSelected);
    connect(ui->Shapes, &QComboBox::currentTextChanged, this, &MainWindow::onShapeSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onShapeSelected()
{
    QString selected = ui->Shapes->currentText();
    if (selected == "Cuboid")
    {
        Cuboid cuboid({ 0, 0, 0 }, 2, 2, 2); // Example dimensions
        auto tris = cuboid.getTriangles();
        glWidget->loadTriangles(tris);
    }
    // Add similar blocks for other shapes
}

void MainWindow::onOpenSTLClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open STL File", "", "STL Files (*.stl)");
    if (!filePath.isEmpty())
    {
        loadAndDisplaySTL(filePath.toStdString());
    }
}

void MainWindow::loadAndDisplaySTL(const std::string& filename)
{
    STLShape shape(filename);
    auto tris = shape.getTriangles();

    qDebug() << "Loaded triangles:" << tris.size();

    if (tris.empty())
    {
        QMessageBox::warning(this, "Error", "Failed to load STL file or no triangles found.");
        return;
    }

    glWidget->loadTriangles(tris);
}
