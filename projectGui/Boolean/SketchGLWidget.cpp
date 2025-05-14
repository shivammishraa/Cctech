#include "SketchGLWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <vector>
#include "polygon_boolean.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

SketchGLWidget::SketchGLWidget(QWidget* parent)
    : QOpenGLWidget(parent), selectedRegionIdx(-1), computedColor(Qt::transparent) {
    computedRegions.clear();
}

void SketchGLWidget::initializeGL() {
    glClearColor(1, 1, 1, 1);
}

void SketchGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void SketchGLWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    for (const auto& shape : allShapes) {
        for (size_t i = 0; i < shape.size(); ++i) {
            painter.drawLine(shape[i], shape[(i + 1) % shape.size()]);
        }
    }

    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i + 1 < tempPoints.size(); ++i) {
        painter.drawLine(tempPoints[i], tempPoints[i + 1]);
    }

    for (int i = 0; i < computedRegions.size(); ++i) {
        QBrush brush(i == selectedRegionIdx ? QColor(255, 255, 0, 120) : QColor(100, 100, 255, 100));
        painter.setBrush(brush);
        QPolygonF poly;
        for (const auto& pt : computedRegions[i])
            poly << pt;
        painter.drawPolygon(poly);
    }
    painter.end();
}

bool pointFuzzyEqual(const QPointF& a, const QPointF& b, double eps) {
    return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}

void SketchGLWidget::mousePressEvent(QMouseEvent* event) {
    QPointF clickPt = event->pos();
    selectedRegionIdx = -1;
    for (int i = 0; i < computedRegions.size(); ++i) {
        if (pointInPolygon(clickPt, computedRegions[i])) {
            selectedRegionIdx = i;
            update();
            return;
        }
    }

    if (!tempPoints.empty() && pointFuzzyEqual(clickPt, tempPoints.front())) {
        completeShape();
    } else {
        tempPoints.push_back(clickPt);
    }
    update();
}

void SketchGLWidget::completeShape() {
    if (tempPoints.size() < 3) return;

    allShapes.push_back(tempPoints);
    qDebug() << "Shape completed with" << tempPoints.size()-1 << "vertices.";
    tempPoints.clear();
    update();
}

void SketchGLWidget::performUnion() {
    if (allShapes.size() < 2) return;
    const auto& a = allShapes[0];
    const auto& b = allShapes[1];
    auto res = PolygonBoolean::unionPolygons(a, b);
    if (!res.empty()) {
        computedRegions.clear();
        computedRegions.push_back(res);
        selectedRegionIdx = 0;
        update();
    }
}

void SketchGLWidget::performIntersection() {
    if (allShapes.size() < 2) return;
    const auto& a = allShapes[0];
    const auto& b = allShapes[1];
    auto res = PolygonBoolean::intersect(a, b);
    if (!res.empty()) {
        computedRegions.clear();
        computedRegions.push_back(res);
        selectedRegionIdx = 0;
        update();
    }
}

void SketchGLWidget::performSubtraction(bool abOrder) {
    if (allShapes.size() < 2) return;
    const auto& a = abOrder ? allShapes[0] : allShapes[1];
    const auto& b = abOrder ? allShapes[1] : allShapes[0];
    auto res = PolygonBoolean::subtractPolygons(a, b);
    if (!res.empty()) {
        allShapes = { res };
        computedRegions.clear();
        selectedRegionIdx = -1;
        update();
    }
}

void SketchGLWidget::resetResult() {
    computedRegions.clear();
    selectedRegionIdx = -1;
    computedColor = Qt::transparent;
    update();
}

std::vector<std::vector<QPointF>> SketchGLWidget::polygons() const {
    return allShapes;
}

std::vector<QPointF> SketchGLWidget::currentRegionPolygon() const {
    if (selectedRegionIdx >= 0 && selectedRegionIdx < computedRegions.size())
        return computedRegions[selectedRegionIdx];
    return {};
}
