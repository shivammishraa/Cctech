#include "glwidget.h"
#include <QDebug>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
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

void GLWidget::loadTriangles(const std::vector<std::vector<std::vector<double>>>& tris)
{
    triangles.clear();
    for (const auto& tri : tris)
    {
        std::array<std::array<double, 3>, 3> arrTri;
        for (int i = 0; i < 3; ++i)
        {
            arrTri[i][0] = tri[i][0];
            arrTri[i][1] = tri[i][1];
            arrTri[i][2] = tri[i][2];
        }
        triangles.push_back(arrTri);
    }
    update();
}
