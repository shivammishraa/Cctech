#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stl_shape.h"
#include "cuboid.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("STL Viewer - OpenGL & Qt");

    // Make sure openGLWidget is promoted to GLWidget in the .ui file
    glWidget = qobject_cast<GLWidget*>(ui->openGLWidget);

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
        ::Cuboid cuboid(2, 2, 2);
        qDebug() << "Type check (has getCuboidTriangles?):" << cuboid.getCuboidTriangles().size();

        // Get the triangles from the cuboid
        auto tris = cuboid.getCuboidTriangles();

        // Convert tris from std::vector<std::array<std::array<double, 3>, 3>> to std::vector<std::vector<std::vector<double>>>
        vector<vector<vector<double>>> convertedTris;

        for (const auto& tri : tris) {
            vector<vector<double>> triangle;
            for (const auto& point : tri) {
                vector<double> vertex = { point[0], point[1], point[2] };
                triangle.push_back(vertex);
            }
            convertedTris.push_back(triangle);
        }

        // Now load the converted triangles into GLWidget
        glWidget->loadTriangles(convertedTris);
    }
    // Add other shapes here
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
    auto tris = shape.getCuboidTriangles();

    qDebug() << "Loaded triangles:" << tris.size();

    if (tris.empty())
    {
        QMessageBox::warning(this, "Error", "Failed to load STL file or no triangles found.");
        return;
    }

    // Convert tris from std::array<std::array<double, 3>, 3> to std::vector<std::vector<std::vector<double>>>
    vector<vector<vector<double>>> convertedTris;
    for (const auto& tri : tris)
    {
        vector<vector<double>> triangle;
        for (const auto& vertex : tri)
        {
            triangle.push_back({ vertex[0], vertex[1], vertex[2] });
        }
        convertedTris.push_back(triangle);
    }

    glWidget->loadTriangles(convertedTris);
}
