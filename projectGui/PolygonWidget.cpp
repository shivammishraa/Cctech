#include "PolygonWidget.h"
#include <QDebug>
#include <cmath>

PolygonWidget::PolygonWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
    setMouseTracking(true);
}

PolygonWidget::~PolygonWidget() {
    clearCurrentShape();
}

void PolygonWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void PolygonWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void PolygonWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    QVector3D center(0.0f, 0.0f, 0.0f);
    if (!polygonPoints.isEmpty()) {
        for (const auto &point : polygonPoints) {
            center += point;
        }
        center /= polygonPoints.size();
    }

    glTranslatef(center.x(), center.y(), -3.0f);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glTranslatef(-center.x(), -center.y(), 0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 0.0f, 1.0f);

    if (!polygonPoints.isEmpty()) {
        glBegin(GL_LINE_LOOP);
        for (const auto &point : polygonPoints) {
            glVertex3f(point.x(), point.y(), point.z());
        }
        glEnd();
    }

    for (const auto &face : faces) {
        glBegin(GL_POLYGON);
        for (const auto &edge : face->edges) {
            glVertex3f(edge->v1->x, edge->v1->y, edge->v1->z);
        }
        glEnd();
    }

    if (!polygonPoints.isEmpty()) {
        QVector3D first = polygonPoints.first();
        glPointSize(8.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex3f(first.x(), first.y(), first.z());
        glEnd();
    }
}

void PolygonWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = true;
        lastMousePos = event->pos();
    } else if (event->button() == Qt::LeftButton) {
        QVector3D worldPoint = screenToWorld(event->pos());
        if (!polygonClosed) {
            if (!polygonPoints.isEmpty() && isCloseToFirstPoint(worldPoint)) {
                polygonClosed = true;
                polygonNormal = computePolygonNormal(polygonPoints);
            } else {
                polygonPoints.append(worldPoint);
            }
            update();
        }
    }
}

void PolygonWidget::mouseMoveEvent(QMouseEvent *event) {
    if (rightMousePressed) {
        float dx = event->pos().x() - lastMousePos.x();
        float dy = event->pos().y() - lastMousePos.y();
        rotationX += dy * 0.5f;
        rotationY += dx * 0.5f;
        lastMousePos = event->pos();
        update();
    } else if (polygonClosed && event->buttons() & Qt::LeftButton) {
        float dy = event->pos().y() - height() / 2.0f;
        float newHeight = std::max(0.0f, dy * 0.03f);
        if (std::abs(newHeight - extrusionHeight) > 0.001f) {
            extrusionHeight = newHeight;
            generateExtrudedFaces();
            update();
        }
    }
}

void PolygonWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = false;
    }
}

QVector3D PolygonWidget::screenToWorld(const QPoint &pos) {
    float orthoLeft = -2.0f, orthoRight = 2.0f;
    float orthoBottom = -2.0f, orthoTop = 2.0f;

    float ndcX = (2.0f * pos.x()) / width() - 1.0f;
    float ndcY = 1.0f - (2.0f * pos.y()) / height();

    float worldX = orthoLeft + (ndcX + 1.0f) * (orthoRight - orthoLeft) / 2.0f;
    float worldY = orthoBottom + (ndcY + 1.0f) * (orthoTop - orthoBottom) / 2.0f;

    return QVector3D(worldX, worldY, 0.0f);
}

bool PolygonWidget::isCloseToFirstPoint(const QVector3D &p) {
    return !polygonPoints.isEmpty() && (polygonPoints.first() - p).length() < 0.05f;
}

QVector3D PolygonWidget::computePolygonNormal(const QVector<QVector3D> &points) {
    QVector3D normal(0, 0, 0);
    for (int i = 0; i < points.size(); ++i) {
        const QVector3D &curr = points[i];
        const QVector3D &next = points[(i + 1) % points.size()];
        normal += QVector3D::crossProduct(curr, next);
    }
    return normal.normalized();
}

void PolygonWidget::generateExtrudedFaces() {
    if (!polygonClosed) return;

    QSet<Vertex*> uniqueVertices;
    for (auto face : faces) {
        for (auto edge : face->edges) {
            uniqueVertices.insert(edge->v1);
            uniqueVertices.insert(edge->v2);
            delete edge;
        }
        delete face;
    }
    for (auto vertex : uniqueVertices) {
        delete vertex;
    }
    faces.clear();

    QVector<QVector3D> topVertices;
    for (const auto &pt : polygonPoints) {
        topVertices.append(pt + polygonNormal * extrusionHeight);
    }

    std::vector<Edge*> topEdges;
    for (int i = 0; i < topVertices.size(); ++i) {
        Vertex* v1 = new Vertex(topVertices[i].x(), topVertices[i].y(), topVertices[i].z());
        Vertex* v2 = new Vertex(topVertices[(i + 1) % topVertices.size()].x(),
                                topVertices[(i + 1) % topVertices.size()].y(),
                                topVertices[(i + 1) % topVertices.size()].z());
        topEdges.push_back(new Edge(v1, v2));
    }
    faces.push_back(new Face(topEdges));

    for (int i = 0; i < polygonPoints.size(); ++i) {
        QVector3D b1 = polygonPoints[i];
        QVector3D b2 = polygonPoints[(i + 1) % polygonPoints.size()];
        QVector3D t1 = topVertices[i];
        QVector3D t2 = topVertices[(i + 1) % topVertices.size()];

        Vertex* vb1 = new Vertex(b1.x(), b1.y(), b1.z());
        Vertex* vb2 = new Vertex(b2.x(), b2.y(), b2.z());
        Vertex* vt1 = new Vertex(t1.x(), t1.y(), t1.z());
        Vertex* vt2 = new Vertex(t2.x(), t2.y(), t2.z());

        std::vector<Edge*> sideEdges = {
            new Edge(vb1, vb2),
            new Edge(vb2, vt2),
            new Edge(vt2, vt1),
            new Edge(vt1, vb1)
        };

        faces.push_back(new Face(sideEdges));
    }

    std::vector<Edge*> bottomEdges;
    for (int i = 0; i < polygonPoints.size(); ++i) {
        Vertex* v1 = new Vertex(polygonPoints[i].x(), polygonPoints[i].y(), polygonPoints[i].z());
        Vertex* v2 = new Vertex(polygonPoints[(i + 1) % polygonPoints.size()].x(),
                                polygonPoints[(i + 1) % polygonPoints.size()].y(),
                                polygonPoints[(i + 1) % polygonPoints.size()].z());
        bottomEdges.push_back(new Edge(v1, v2));
    }
    faces.push_back(new Face(bottomEdges));
}

void PolygonWidget::addShape(const QString &shapeName) {
    QVector<Face*> savedFaces;
    for (const auto &face : faces) {
        std::vector<Edge*> edges;
        for (const auto &edge : face->edges) {
            edges.push_back(new Edge(new Vertex(*edge->v1), new Vertex(*edge->v2)));
        }
        savedFaces.append(new Face(edges));
    }
    shapeData.append(savedFaces);
    shapeNames.append(shapeName);
    qDebug() << "Shape added to list:" << shapeName;
}

void PolygonWidget::clearCurrentShape() {
    polygonPoints.clear();
    extrudedVertices.clear();

    for (auto face : faces) {
        for (auto edge : face->edges) {
            delete edge;
        }
        delete face;
    }
    faces.clear();

    extrusionHeight = 0.0f;
    polygonClosed = false;
    rotationX = 0.0f;
    rotationY = 0.0f;
    update();
    qDebug() << "Current shape cleared. Ready to draw a new shape.";
}

void PolygonWidget::displayShape(const QString &shapeName) {
    int index = shapeNames.indexOf(shapeName);
    if (index != -1) {
        faces = shapeData[index];
        polygonClosed = true;
        extrusionHeight = 0.0f;
        rotationX = 0.0f;
        rotationY = 0.0f;
        update();
        qDebug() << "Displaying shape:" << shapeName;
    } else {
        qDebug() << "Shape not found:" << shapeName;
    }
}

void PolygonWidget::adjustZAxis() {
    for (auto &face : faces) {
        for (auto &edge : face->edges) {
            edge->v1->z -= zOffset;
            edge->v2->z -= zOffset;
        }
    }
    update();
    qDebug() << "Z-axis adjusted for overlapping.";
}

void PolygonWidget::deleteShape(const QString &shapeName) {
    int index = shapeNames.indexOf(shapeName);
    if (index != -1) {
        shapeNames.removeAt(index);
        shapeData.removeAt(index);
        qDebug() << "Shape deleted from OpenGL widget:" << shapeName;
    } else {
        qDebug() << "Shape not found for deletion:" << shapeName;
    }
}
