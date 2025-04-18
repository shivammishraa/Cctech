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

private slots:
    void on_plotButton_clicked();  // <-- Correctly connected in cpp

private:
    Ui::MainWindow* ui;
    GLWidget* glWidget;  // <-- Properly casted from promoted widget
};

#endif // MAINWINDOW_H
