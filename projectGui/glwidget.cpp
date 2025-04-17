#include "glwidget.h"
#include <QMatrix4x4>
#include <QDebug>
#include <QWheelEvent>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent), initialized(false), zoomFactor(1.0f),
    sceneX(0.0f), sceneY(0.0f), sensitivity(0.01f)
{
    qDebug() << "GLWidget constructed";
}

void GLWidget::initializeGL()
{
    qDebug() << "Initializing OpenGL...";
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    initialized = true;
}

void GLWidget::resizeGL(int w, int h)
{
    qDebug() << "Resizing OpenGL viewport to" << w << "x" << h;
    glViewport(0, 0, w, h);

    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.constData());
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Model-view transformation
    QMatrix4x4 modelview;
    modelview.setToIdentity();

    // Apply zoom (Z-axis translation) and pan (X, Y translation)
    modelview.translate(sceneX, sceneY, -5.0f * zoomFactor);
    modelview.translate(shapeCenter);
    modelview.rotate(rotationX, 1.0f, 0.0f, 0.0f); // X-axis
    modelview.rotate(rotationY, 0.0f, 1.0f, 0.0f); // Y-axis
    modelview.translate(-shapeCenter);     // move back to original position


    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelview.constData());

    glColor3f(1.0f, 1.0f, 0.0f);
    // Try using LINE_STRIP for continuous curves like Bezier
    glBegin(GL_LINE_STRIP);
    for (const auto& edge : shapeEdges)
    {
        glVertex3f(edge.first.x(), edge.first.y(), edge.first.z());
    }
    // Add the very last point from the last edge
    if (!shapeEdges.empty()) {
        glVertex3f(shapeEdges.back().second.x(), shapeEdges.back().second.y(), shapeEdges.back().second.z());
    }
    glEnd();

}

void GLWidget::setShapeVertices(const std::vector<std::pair<std::vector<double>, std::vector<double>>>& edges)
{
    shapeEdges.clear();
    QVector3D total(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const auto& edge : edges)
    {
        if (edge.first.size() == 3 && edge.second.size() == 3)
        {
            QVector3D start(edge.first[0], edge.first[1], edge.first[2]);
            QVector3D end(edge.second[0], edge.second[1], edge.second[2]);

            // Add to list of edges to draw
            shapeEdges.emplace_back(start, end);

            // For computing center
            total += start + end;
            count += 2;
        }
    }

    // Compute shape center for rotation
    shapeCenter = (count > 0) ? (total / count) : QVector3D(0, 0, 0);

    update(); // trigger paintGL
}

void GLWidget::checkOpenGLError()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << err;
    }
}

// === Mouse Zoom ===
void GLWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    if (delta > 0)
        zoomFactor *= 0.9f; // Zoom in
    else
        zoomFactor *= 1.1f; // Zoom out

    update();
}

// === Mouse Pan ===
void GLWidget::mousePressEvent(QMouseEvent* event)
{
    lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint delta = event->pos() - lastMousePos;

    if (event->buttons() & Qt::LeftButton) {
        // Pan with left mouse button
        sceneX += delta.x() * sensitivity;
        sceneY -= delta.y() * sensitivity;
    }
    else if (event->buttons() & Qt::RightButton) {
        // Rotate with right mouse button
        rotationX += delta.y();  // Up/down rotates around X-axis
        rotationY += delta.x();  // Left/right rotates around Y-axis
    }

    lastMousePos = event->pos();
    update();
}