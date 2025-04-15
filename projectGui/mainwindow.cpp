#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cuboid.h"
#include "shapeinputdialog.h"

#include <QDebug>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Shape Viewer - OpenGL & Qt");

    // Cast promoted widget to GLWidget
    glWidget = qobject_cast<GLWidget*>(ui->openGLWidget);

    // Connect only plot button
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_plotButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_plotButton_clicked()
{
    QString selectedShape = ui->Shapes->currentText(); // Using only one combo box named "Shapes"

    if (selectedShape == "Cuboid")
    {
        ShapeInputDialog dialog(this);
        dialog.setWindowTitle("Enter Cuboid Dimensions");

        if (dialog.exec() == QDialog::Accepted)
        {
            double length = dialog.getLength();
            double width = dialog.getWidth();
            double height = dialog.getHeight();

            if (length <= 0 || width <= 0 || height <= 0)
            {
                QMessageBox::warning(this, "Invalid Input", "All dimensions must be positive.");
                return;
            }

            Cuboid cuboid(length, width, height);
            vector<pair<vector<double>, vector<double>>> edges = cuboid.getCuboidEdgesAsLines();

            if (glWidget)
            {
                glWidget->setShapeVertices(edges);
                glWidget->update();
            }
            else
            {
                QMessageBox::critical(this, "Error", "OpenGL widget not initialized properly.");
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Shape Not Supported", "This shape will be available soon.");
    }
}
