#include "glwidget.h"
#include <QMatrix4x4>
#include <QDebug>
#include <QWheelEvent>
#include <QHash>
#include <QMouseEvent>
#include <unordered_map>
#include <QtGlobal>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <memory>
#include <shape.h>
#include <stl_shape.h>
#include <obj_shape.h>


inline uint qHash(const QVector3D& key, uint seed = 0) noexcept
{
    return qHash(qRound(key.x() * 1000) ^ qRound(key.y() * 1000) ^ qRound(key.z() * 1000), seed);
}

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent), initialized(false), zoomFactor(1.0f),
    sceneX(0.0f), sceneY(0.0f), sensitivity(0.01f),
    vertexBuffer(QOpenGLBuffer::VertexBuffer), indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    qDebug() << "GLWidget constructed";
}

void GLWidget::initializeGL()
{
    qDebug() << "Initializing OpenGL...";
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f);
    // glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0);

    // GLfloat lightAmbient[] = { 0.0f, 0.8f, 0.2f, 1.0f };
    // GLfloat lightDiffuse[] = { 0.3f, 0.5f, 0.8f, 1.0f };
    // GLfloat lightSpecular[] = { 0.0f, 1.0f, 1.0f, 1.0f };
    // GLfloat lightPosition[] = { 0.0f, 0.2f, 1.0f, 1.0f };

    // glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    // glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);



    // GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightDiffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    glEnable(GL_NORMALIZE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    initializeBuffers();
    initialized = true;
}

void GLWidget::resizeGL(int w, int h)
{
    qDebug() << "Resizing OpenGL viewport to" << w << "x" << h;
    glViewport(0, 0, w, h);

    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 10000.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.constData());
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelview;
    modelview.setToIdentity();
    modelview.translate(sceneX, sceneY, -10.0f * zoomFactor);
    modelview.translate(shapeCenter);
    modelview.rotate(rotationX, 1.0f, 0.0f, 0.0f);
    modelview.rotate(rotationY, 0.0f, 1.0f, 0.0f);
    modelview.translate(-shapeCenter);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(modelview.constData());

    if (buffersInitialized) {
        vao.bind();
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);
        vao.release();
    }

    qDebug() << "Rendering edges count:" << shapeEdges.size();
}

void GLWidget::setShapeVertices(const std::vector<std::pair<std::vector<double>, std::vector<double>>>& edges)
{
    vertices.clear();
    indices.clear();
    QHash<QVector3D, unsigned int> vertexMap;
    unsigned int index = 0;

    QVector3D total(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const auto& edge : edges) {
        if (edge.first.size() == 3 && edge.second.size() == 3) {
            QVector3D start(edge.first[0], edge.first[1], edge.first[2]);
            QVector3D end(edge.second[0], edge.second[1], edge.second[2]);

            if (vertexMap.find(start) == vertexMap.end()) {
                vertexMap[start] = index++;
                vertices.push_back(start);
                total += start;
                count++;
            }
            if (vertexMap.find(end) == vertexMap.end()) {
                vertexMap[end] = index++;
                vertices.push_back(end);
                total += end;
                count++;
            }

            indices.push_back(vertexMap[start]);
            indices.push_back(vertexMap[end]);
        }
    }

    if (count > 0)
        shapeCenter = total / float(count);
    else
        shapeCenter = QVector3D(0.0f, 0.0f, 0.0f);

    updateBuffers();
    update();
}

void GLWidget::addTrianglesToEdges(const std::vector<std::vector<std::vector<double>>>& triangles)
{
    vertices.clear();
    indices.clear();
    QHash<QVector3D, unsigned int> vertexMap;
    unsigned int index = 0;

    for (const auto& triangle : triangles) {
        QVector3D v1((triangle[0][0]) / 10, triangle[0][1] / 10, triangle[0][2] / 10);
        QVector3D v2(triangle[1][0] / 10, triangle[1][1] / 10, triangle[1][2] / 10);
        QVector3D v3(triangle[2][0] / 10, triangle[2][1] / 10, triangle[2][2] / 10);

        for (const auto& vertex : { v1, v2, v3 }) {
            if (vertexMap.find(vertex) == vertexMap.end()) {
                vertexMap[vertex] = index++;
                vertices.push_back(vertex);
            }
        }

        indices.push_back(vertexMap[v1]);
        indices.push_back(vertexMap[v2]);
        indices.push_back(vertexMap[v2]);
        indices.push_back(vertexMap[v3]);
        indices.push_back(vertexMap[v3]);
        indices.push_back(vertexMap[v1]);
    }

    updateBuffers();
    update();
}

void GLWidget::initializeBuffers()
{
    vao.create();
    vao.bind();

    vertexBuffer.create();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vertexBuffer.bind();

    indexBuffer.create();
    indexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    indexBuffer.bind();

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(QVector3D), nullptr);

    vao.release();
    buffersInitialized = true;
}

void GLWidget::updateBuffers()
{
    if (!buffersInitialized) initializeBuffers();

    vertexBuffer.bind();
    vertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    indexBuffer.bind();
    indexBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));
}

void GLWidget::checkOpenGLError()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        qDebug() << "OpenGL error:" << err;
    }
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    zoomFactor *= (delta > 0) ? 0.9f : 1.1f;
    update();
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint delta = event->pos() - lastMousePos;

    if (event->buttons() & Qt::LeftButton) {
        sceneX += delta.x() * sensitivity;
        sceneY -= delta.y() * sensitivity;
    }
    else if (event->buttons() & Qt::RightButton) {
        rotationX += delta.y();
        rotationY += delta.x();
    }

    lastMousePos = event->pos();
    update();
}