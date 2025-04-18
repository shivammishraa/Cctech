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
    glBegin(GL_LINES);
    for (const auto& edge : shapeEdges)
    {
        const QVector3D& start = edge.first;
        const QVector3D& end = edge.second;

        glVertex3f(start.x(), start.y(), start.z());
        glVertex3f(end.x(), end.y(), end.z());
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

            shapeEdges.emplace_back(start, end);
            total += start + end;
            count += 2;
        }
    }

    if (count > 0)
        shapeCenter = total / float(count);
    else
        shapeCenter = QVector3D(0.0f, 0.0f, 0.0f);

    update();
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
    QVector<QPointF> points;

	points.push_back(event->pos());
	if (event->button() == Qt::LeftButton) {
		// Handle left button press
		qDebug() << "Left button pressed at:" << points;
	}
	else if (event->button() == Qt::RightButton) {
		// Handle right button press
		qDebug() << "Right button pressed at:" << points;
	}
	else if (event->button() == Qt::MiddleButton) {
		// Handle middle button press
		qDebug() << "Middle button pressed at:" << points;
	}
	// Print all points in the list
	for (auto x : points)
	{
		qDebug() << "Mouse Pressed at:" << x;
	}
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