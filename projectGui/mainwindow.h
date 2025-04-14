#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void loadAndDisplaySTL(const std::string& filename);

private slots:
    void onOpenSTLClicked();
    void onShapeSelected();

private:
    Ui::MainWindow* ui;
    GLWidget* glWidget;  // Will wrap around openGLWidget from UI
};

#endif // MAINWINDOW_H