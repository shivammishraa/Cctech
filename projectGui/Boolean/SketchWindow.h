#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class SketchGLWidget;

class SketchWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SketchWindow(QWidget *parent = nullptr);

private slots:
    void handleFinishShape();
    void handleUnion();
    void handleIntersection();
    void handleSubtraction();

private:
    SketchGLWidget *glWidget;
};

#endif // SKETCHWINDOW_H
