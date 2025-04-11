#include "glwidget.h"
#include <QDebug>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent) {
    qDebug() << "GLWidget constructed";
}

void GLWidget::initializeGL()
{
    qDebug() << "Initializing OpenGL...";
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void GLWidget::resizeGL(int w, int h)
{
    qDebug() << "Resizing OpenGL viewport to" << w << "x" << h;
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    qDebug() << "Painting GL...";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(45, 1, 0, 0);
    glRotated(45, 0, 1, 0);

    glBegin(GL_TRIANGLES);
    glColor3f(0.3f, 0.7f, 1.0f);
    for (const auto& tri : triangles)
    {
        for (const auto& vertex : tri)
        {
            glVertex3d(vertex[0], vertex[1], vertex[2]);
        }
    }
    glEnd();
}
void GLWidget::loadTriangles(const std::vector<std::array<std::array<double, 3>, 3>>& tris)
{
    triangles = tris;
    update();
}