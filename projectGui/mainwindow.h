#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "glwidget.h"
#include "BezierWidget.h" // Include BezierWidget header
#include "PolygonWidget.h" // Include PolygonWidget header

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_plotButton_clicked();
    void on_uploadButton_clicked();
    void openBezierWidget(); // Slot to switch to BezierWidget
    void openGLWidget();     // Slot to switch back to GLWidget
    void openPolygonWidget(); // Slot to open the PolygonWidget window

private:
    Ui::MainWindow* ui;
    QStackedWidget* stackedWidget; // Stacked widget to manage views
    GLWidget* glWidget;
    BezierWidget* bezierWidget;
};

#endif // MAINWINDOW_H
