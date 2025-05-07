#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPoint>
#include <vector>
#include <QHash>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget* parent = nullptr);
    void setShapeVertices(const std::vector<std::pair<std::vector<double>, std::vector<double>>>& edges);
    void addTrianglesToEdges(const std::vector<std::vector<std::vector<double>>>& triangles);

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
    QOpenGLShaderProgram shaderProgram;

    float zoomFactor;
    float sceneX, sceneY;
    float sensitivity;
    QPoint lastMousePos;

    QVector3D shapeCenter;
    float rotationX = 0.0f; // optional: for rotating using mouse
    float rotationY = 0.0f;

    QOpenGLBuffer vertexBuffer; // VBO for vertices
    QOpenGLBuffer indexBuffer;  // VBO for indices
    QOpenGLVertexArrayObject vao; // VAO for managing VBOs

    std::vector<QVector3D> vertices; // Vertex data
    std::vector<unsigned int> indices; // Index data

    bool buffersInitialized = false; // Track if buffers are initialized
    void initializeBuffers(); // Initialize VBOs and VAO
    void updateBuffers(); // Update VBOs with new data

    void checkOpenGLError();
};

#endif // GLWIDGET_H
