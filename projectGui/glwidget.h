#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>
#include <vector>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget* parent = nullptr);
    void setShapeVertices(const std::vector<std::pair<std::vector<double>, std::vector<double>>>& edges);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    bool initialized = false;
    std::vector<std::pair<QVector3D, QVector3D>> shapeEdges;
    QMatrix4x4 projection;

    float zoomFactor;
    float sceneX, sceneY;
    float sensitivity;
    QPoint lastMousePos;

    QVector3D shapeCenter;  
    float rotationX = 0.0f; // optional: for rotating using mouse
    float rotationY = 0.0f;

    void checkOpenGLError();
};

#endif // GLWIDGET_H
