#include "BooleanOperationWidget.h"
#include "BasicShapes2D.h"
#include "../shapefactory.h"
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QVector2D>
#include <QPolygonF>
#include <QPainterPath>
#include <algorithm>

BooleanOperationWidget::BooleanOperationWidget(QWidget *parent)
    : QWidget(parent), glWidget(new GLWidget(this)), translationA2D(0, 0), translationB2D(0, 0), scaleA2D(1.0f), scaleB2D(1.0f)
{
    setupUI();
}

void BooleanOperationWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *layoutA = new QHBoxLayout();
    layoutA->addWidget(new QLabel("Shape A:"));
    shapeAComboBox = new QComboBox();
    layoutA->addWidget(shapeAComboBox);

    QHBoxLayout *layoutB = new QHBoxLayout();
    layoutB->addWidget(new QLabel("Shape B:"));
    shapeBComboBox = new QComboBox();
    layoutB->addWidget(shapeBComboBox);

    QStringList shapes = {"Square", "Rectangle", "Circle", "Triangle", "Polygon"};
    shapeAComboBox->addItems(shapes);
    shapeBComboBox->addItems(shapes);

    plotShapeAButton = new QPushButton("Plot Shape A");
    plotShapeBButton = new QPushButton("Plot Shape B");
    intersectionButton = new QPushButton("Intersection");
    subtractionButton = new QPushButton("Subtraction");
    unionButton = new QPushButton("Union");

    layout->addLayout(layoutA);
    layout->addLayout(layoutB);
    layout->addWidget(plotShapeAButton);
    layout->addWidget(plotShapeBButton);
    layout->addWidget(glWidget);
    layout->addWidget(intersectionButton);
    layout->addWidget(subtractionButton);
    layout->addWidget(unionButton);

    connect(plotShapeAButton, &QPushButton::clicked, this, &BooleanOperationWidget::onPlotShapeA);
    connect(plotShapeBButton, &QPushButton::clicked, this, &BooleanOperationWidget::onPlotShapeB);
    connect(intersectionButton, &QPushButton::clicked, this, &BooleanOperationWidget::onIntersectionClicked);
    connect(subtractionButton, &QPushButton::clicked, this, &BooleanOperationWidget::onSubtractionClicked);
    connect(unionButton, &QPushButton::clicked, this, &BooleanOperationWidget::onUnionClicked);
}

std::vector<std::vector<QVector2D>> getShapeFromDialog(const QString &shapeType, QWidget *parent)
{
    bool ok;
    if (shapeType == "Square")
    {
        float side = QInputDialog::getDouble(parent, "Square Side", "Enter side length:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        return BasicShapes2D::createSquare(side);
    }
    else if (shapeType == "Rectangle")
    {
        float width = QInputDialog::getDouble(parent, "Rectangle Width", "Enter width:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        float height = QInputDialog::getDouble(parent, "Rectangle Height", "Enter height:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        return BasicShapes2D::createRectangle(width, height);
    }
    else if (shapeType == "Circle")
    {
        float radius = QInputDialog::getDouble(parent, "Circle Radius", "Enter radius:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        return BasicShapes2D::createCircle(radius);
    }
    else if (shapeType == "Triangle")
    {
        float base = QInputDialog::getDouble(parent, "Triangle Base", "Enter base:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        float height = QInputDialog::getDouble(parent, "Triangle Height", "Enter height:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        return BasicShapes2D::createTriangle(base, height);
    }
    else if (shapeType == "Polygon")
    {
        int sides = QInputDialog::getInt(parent, "Polygon Sides", "Enter number of sides:", 5, 3, 100, 1, &ok);
        if (!ok)
            return {};
        float radius = QInputDialog::getDouble(parent, "Polygon Radius", "Enter radius:", 1.0, 0.01, 1000, 2, &ok);
        if (!ok)
            return {};
        return BasicShapes2D::createPolygon(sides, radius);
    }
    return {};
}

void BooleanOperationWidget::onPlotShapeA()
{
    shapeA2D = getShapeFromDialog(shapeAComboBox->currentText(), this);
    result2D.clear();
    glWidget->setShapes2D(shapeA2D, shapeB2D, result2D);
}

void BooleanOperationWidget::onPlotShapeB()
{
    shapeB2D = getShapeFromDialog(shapeBComboBox->currentText(), this);
    result2D.clear();
    glWidget->setShapes2D(shapeA2D, shapeB2D, result2D);
}

// Helper for face comparison (with tolerance)
bool compareFace2D(const std::vector<QVector2D> &a, const std::vector<QVector2D> &b, float tol = 0.0001f)
{
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i)
    {
        if ((a[i] - b[i]).length() > tol)
            return false;
    }
    return true;
}

// bool pointInsidePolygon(const QVector2D& point, const std::vector<QVector2D>& polygon) {
//     int count = 0;
//     for (size_t i = 0; i < polygon.size(); ++i) {
//         QVector2D a = polygon[i];
//         QVector2D b = polygon[(i + 1) % polygon.size()];
//         if ((a.y() > point.y()) != (b.y() > point.y())) {
//             float x = (b.x() - a.x()) * (point.y() - a.y()) / (b.y() - a.y()) + a.x();
//             if (point.x() < x) count++;
//         }
//     }
//     return count % 2 == 1;
// }

void BooleanOperationWidget::performSubtraction()
{
    qInfo("Performing Subtraction");
    result2D.clear();
    if (shapeA2D.empty() || shapeB2D.empty())
        return;

    QPolygonF polyA, polyB;
    for (const auto &v : shapeA2D[0])
        polyA << QPointF(v.x() + translationA2D.x(), v.y() + translationA2D.y());
    for (const auto &v : shapeB2D[0])
        polyB << QPointF(v.x() + translationB2D.x(), v.y() + translationB2D.y());

    QPainterPath pathA, pathB;
    pathA.addPolygon(polyA);
    pathB.addPolygon(polyB);
    QPainterPath subPath = pathA.subtracted(pathB);

    result2D.clear();
    for (int i = 0; i < subPath.elementCount();)
    {
        std::vector<QVector2D> face;
        QPainterPath::Element e = subPath.elementAt(i);
        face.emplace_back(e.x, e.y);
        int start = i;
        ++i;
        while (i < subPath.elementCount())
        {
            QPainterPath::Element e2 = subPath.elementAt(i);
            face.emplace_back(e2.x, e2.y);
            if (e2.isMoveTo())
                break;
            ++i;
        }
        if (face.size() > 2)
            result2D.push_back(face);
    }
}

void BooleanOperationWidget::performUnion()
{
    qInfo("Performing Union");
    result2D.clear();
    if (shapeA2D.empty() || shapeB2D.empty())
        return;

    // Only support single-face shapes for now (most basic shapes)
    // Convert QVector2D to QPointF for QPolygonF/QRegion
    QPolygonF polyA, polyB;
    for (const auto &v : shapeA2D[0])
        polyA << QPointF(v.x() + translationA2D.x(), v.y() + translationA2D.y());
    for (const auto &v : shapeB2D[0])
        polyB << QPointF(v.x() + translationB2D.x(), v.y() + translationB2D.y());

    // Convert to QRegion (requires integer coordinates)
    QPainterPath pathA, pathB;
    pathA.addPolygon(polyA);
    pathB.addPolygon(polyB);
    QPainterPath unionPath = pathA.united(pathB);

    // Convert back to vector<vector<QVector2D>>
    result2D.clear();
    for (int i = 0; i < unionPath.elementCount();)
    {
        std::vector<QVector2D> face;
        QPainterPath::Element e = unionPath.elementAt(i);
        face.emplace_back(e.x, e.y);
        int start = i;
        ++i;
        while (i < unionPath.elementCount())
        {
            QPainterPath::Element e2 = unionPath.elementAt(i);
            face.emplace_back(e2.x, e2.y);
            if (e2.isMoveTo())
                break;
            ++i;
        }
        if (face.size() > 2)
            result2D.push_back(face);
    }
}

void BooleanOperationWidget::performIntersection()
{
    qInfo("Performing Intersection");
    result2D.clear();
    if (shapeA2D.empty() || shapeB2D.empty())
        return;

    QPolygonF polyA, polyB;
    for (const auto &v : shapeA2D[0])
        polyA << QPointF(v.x() + translationA2D.x(), v.y() + translationA2D.y());
    for (const auto &v : shapeB2D[0])
        polyB << QPointF(v.x() + translationB2D.x(), v.y() + translationB2D.y());

    QPainterPath pathA, pathB;
    pathA.addPolygon(polyA);
    pathB.addPolygon(polyB);
    QPainterPath intersectPath = pathA.intersected(pathB);

    result2D.clear();
    for (int i = 0; i < intersectPath.elementCount();)
    {
        std::vector<QVector2D> face;
        QPainterPath::Element e = intersectPath.elementAt(i);
        face.emplace_back(e.x, e.y);
        int start = i;
        ++i;
        while (i < intersectPath.elementCount())
        {
            QPainterPath::Element e2 = intersectPath.elementAt(i);
            face.emplace_back(e2.x, e2.y);
            if (e2.isMoveTo())
                break;
            ++i;
        }
        if (face.size() > 2)
            result2D.push_back(face);
    }
}

void BooleanOperationWidget::onIntersectionClicked()
{
    performIntersection();
    glWidget->setShapes2D({}, {}, result2D);
}

void BooleanOperationWidget::onSubtractionClicked()
{
    performSubtraction();
    glWidget->setShapes2D({}, {}, result2D);
}

void BooleanOperationWidget::onUnionClicked()
{
    performUnion();
    glWidget->setShapes2D({}, {}, result2D);
}

// ================= GLWidget ================

BooleanOperationWidget::GLWidget::GLWidget(BooleanOperationWidget *parent)
    : QOpenGLWidget(parent), parent(parent), translationA2D(0, 0), translationB2D(0, 0), scaleA2D(1.0f), scaleB2D(1.0f) {}

void BooleanOperationWidget::GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void BooleanOperationWidget::GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = float(w) / float(h);
    float left = -aspect;
    float right = aspect;
    float bottom = -1.0f;
    float top = 1.0f;
    glOrtho(left, right, bottom, top, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void BooleanOperationWidget::GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if (!result2D.empty())
    {
        for (const auto &face : result2D)
            renderShape2D(face, {0, 1, 0}, {0, 0}, 1.0f);
    }
    else
    {
        for (const auto &face : shapeA2D)
            renderShape2D(face, {1, 0, 0}, translationA2D, scaleA2D);
        for (const auto &face : shapeB2D)
            renderShape2D(face, {0, 0, 1}, translationB2D, scaleB2D);
    }
}

void BooleanOperationWidget::GLWidget::setShapes2D(
    const std::vector<std::vector<QVector2D>> &a,
    const std::vector<std::vector<QVector2D>> &b,
    const std::vector<std::vector<QVector2D>> &r)
{
    shapeA2D = a;
    shapeB2D = b;
    result2D = r;
    translationA2D = parent->translationA2D;
    translationB2D = parent->translationB2D;
    scaleA2D = parent->scaleA2D;
    scaleB2D = parent->scaleB2D;
    update();
}

void BooleanOperationWidget::GLWidget::renderShape2D(const std::vector<QVector2D> &shape, const QVector3D &color, const QVector2D &translation, float scale)
{
    glColor3f(color.x(), color.y(), color.z());
    glBegin(GL_POLYGON);
    for (const auto &pt : shape)
    {
        glVertex2f((pt.x() + translation.x()) * scale, (pt.y() + translation.y()) * scale);
    }
    glEnd();
    // Draw outline for better visibility
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    for (const auto &pt : shape)
    {
        glVertex2f((pt.x() + translation.x()) * scale, (pt.y() + translation.y()) * scale);
    }
    glEnd();
}

void BooleanOperationWidget::GLWidget::setEdges(const std::vector<std::pair<QVector3D, QVector3D>> &, const std::vector<std::pair<QVector3D, QVector3D>> &, const std::vector<std::pair<QVector3D, QVector3D>> &)
{
    // Not used in 2D mode
}

void BooleanOperationWidget::GLWidget::renderEdges(const std::vector<std::pair<QVector3D, QVector3D>> &, const QVector3D &, const QVector3D &, const QVector3D &)
{
    // Not used in 2D mode
}

void BooleanOperationWidget::GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
    if (event->button() == Qt::LeftButton)
        draggingShape = 0;
    else if (event->button() == Qt::RightButton)
        draggingShape = 1;
    else
        draggingShape = -1;
}

void BooleanOperationWidget::GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (draggingShape < 0)
        return;
    QPoint delta = event->pos() - lastMousePos;
    QVector2D movement(float(delta.x()) / 100.0f, -float(delta.y()) / 100.0f);
    if (draggingShape == 0)
    {
        translationA2D += movement;
        parent->translationA2D = translationA2D;
    }
    else if (draggingShape == 1)
    {
        translationB2D += movement;
        parent->translationB2D = translationB2D;
    }
    update();
    lastMousePos = event->pos();
}

void BooleanOperationWidget::GLWidget::wheelEvent(QWheelEvent *event)
{
    float scaleDelta = (event->angleDelta().y() > 0) ? 1.1f : 0.9f;
    if (draggingShape == 0)
    {
        parent->scaleA2D *= scaleDelta;
        scaleA2D *= scaleDelta;
    }
    else if (draggingShape == 1)
    {
        parent->scaleB2D *= scaleDelta;
        scaleB2D *= scaleDelta;
    }
    update();
}