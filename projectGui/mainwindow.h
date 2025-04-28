#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "BezierWidget.h" 

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
    void openBezierWidget(); 

private:
    Ui::MainWindow* ui;
    GLWidget* glWidget;
};

#endif // MAINWINDOW_H
