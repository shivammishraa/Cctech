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

SketchGLWidget::SketchGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), selectedRegionIdx(-1), computedColor(Qt::transparent)
{
    computedRegions.clear();
}

void SketchGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
}

void SketchGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SketchGLWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    for (const auto &shape : allShapes)
    {
        for (size_t i = 0; i < shape.size(); ++i)
        {
            painter.drawLine(shape[i], shape[(i + 1) % shape.size()]);
        }
    }

    // Draw live Bézier preview while placing control points
    if (drawMode == DrawMode::Bezier && !bezierControlPoints.empty())
    {
        painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
        // Draw lines between control points
        for (size_t i = 0; i + 1 < bezierControlPoints.size(); ++i)
        {
            painter.drawLine(bezierControlPoints[i], bezierControlPoints[i + 1]);
        }

        // Draw the current Bézier preview curve with partial points
        if (bezierControlPoints.size() >= 2)
        {
            CurveSegment tempBezier;
            tempBezier.type = CurveSegment::CubicBezier;
            tempBezier.p0 = bezierControlPoints[0];
            tempBezier.p1 = bezierControlPoints.size() > 1 ? bezierControlPoints[1] : bezierControlPoints[0];
            tempBezier.p2 = bezierControlPoints.size() > 2 ? bezierControlPoints[2] : tempBezier.p1;
            tempBezier.p3 = bezierControlPoints.size() > 3 ? bezierControlPoints[3] : tempBezier.p2;

            auto previewPoints = flattenBezier(tempBezier, 30);
            for (size_t i = 0; i + 1 < previewPoints.size(); ++i)
            {
                painter.drawLine(previewPoints[i], previewPoints[i + 1]);
            }
        }
    }

    painter.setPen(QPen(Qt::darkGreen, 2));
    for (const auto &curveList : shapeCurves)
    {
        for (const auto &seg : curveList)
        {
            if (seg.type == CurveSegment::CubicBezier)
            {
                drawBezierCurve(seg, painter);
            }
        }
    }

    // Draw Bézier intersection points (if any)
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(QBrush(Qt::yellow));
    for (const auto &pt : bezierIntersections)
    {
        painter.drawEllipse(pt, 2, 2);
    }

    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 0; i + 1 < tempPoints.size(); ++i)
    {
        painter.drawLine(tempPoints[i], tempPoints[i + 1]);
    }

    for (int i = 0; i < computedRegions.size(); ++i)
    {
        QBrush brush(i == selectedRegionIdx ? QColor(255, 255, 0, 120) : QColor(100, 100, 255, 100));
        painter.setBrush(brush);
        QPolygonF poly;
        for (const auto &pt : computedRegions[i])
            poly << pt;
        painter.drawPolygon(poly);
    }
    painter.end();
}

bool pointFuzzyEqual(const QPointF &a, const QPointF &b, double eps)
{
    return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}

void SketchGLWidget::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPt = event->pos();
    selectedRegionIdx = -1;
    for (int i = 0; i < computedRegions.size(); ++i)
    {
        if (pointInPolygon(clickPt, computedRegions[i]))
        {
            selectedRegionIdx = i;
            update();
            return;
        }
    }

    if (drawMode == DrawMode::Bezier)
    {
        bezierControlPoints.push_back(clickPt);
        if (bezierControlPoints.size() == 4)
        {
            CurveSegment bezier;
            bezier.type = CurveSegment::CubicBezier;
            bezier.p0 = bezierControlPoints[0];
            bezier.p1 = bezierControlPoints[1];
            bezier.p2 = bezierControlPoints[2];
            bezier.p3 = bezierControlPoints[3];

            shapeCurves.push_back({bezier});
            bezierControlPoints.clear();
            update();
        }
        else
        {
            // Live preview update after each control point added
            update();
        }
    }
    else
    {
        if (!tempPoints.empty() && pointFuzzyEqual(clickPt, tempPoints.front()))
        {
            completeShape();
        }
        else
        {
            tempPoints.push_back(clickPt);
        }
        update();
    }
}

void SketchGLWidget::drawBezierCurve(const CurveSegment &bezier, QPainter &painter)
{
    std::vector<QPointF> points = flattenBezier(bezier, 30);
    for (size_t i = 0; i + 1 < points.size(); ++i)
        painter.drawLine(points[i], points[i + 1]);
}

std::vector<QPointF> SketchGLWidget::flattenBezier(const CurveSegment &bez, int segments)
{
    std::vector<QPointF> points;
    for (int i = 0; i <= segments; ++i)
    {
        double t = double(i) / segments;
        double u = 1 - t;

        QPointF pt =
            bez.p0 * (u * u * u) +
            bez.p1 * (3 * u * u * t) +
            bez.p2 * (3 * u * t * t) +
            bez.p3 * (t * t * t);

        points.push_back(pt);
    }
    return points;
}

void SketchGLWidget::intersectBezierRecursive(const CurveSegment &a, const CurveSegment &b, std::vector<QPointF> &intersections, int depth)
{
    if (depth > 10)
        return;

    QRectF boxA = QRectF(a.p0, a.p3).normalized().adjusted(-1, -1, 1, 1).united(QRectF(a.p1, a.p2));
    QRectF boxB = QRectF(b.p0, b.p3).normalized().adjusted(-1, -1, 1, 1).united(QRectF(b.p1, b.p2));
    if (!boxA.intersects(boxB))
        return;

    if (QLineF(a.p0, a.p3).length() < 2 && QLineF(b.p0, b.p3).length() < 2)
    {
        QPointF mid = (a.p0 + b.p3) / 2.0;
        intersections.push_back(mid);
        return;
    }

    // Subdivide
    auto subdivide = [](const CurveSegment &seg) -> std::pair<CurveSegment, CurveSegment>
    {
        QPointF m01 = (seg.p0 + seg.p1) / 2;
        QPointF m12 = (seg.p1 + seg.p2) / 2;
        QPointF m23 = (seg.p2 + seg.p3) / 2;
        QPointF m012 = (m01 + m12) / 2;
        QPointF m123 = (m12 + m23) / 2;
        QPointF m0123 = (m012 + m123) / 2;

        CurveSegment left = {CurveSegment::CubicBezier, seg.p0, m01, m012, m0123};
        CurveSegment right = {CurveSegment::CubicBezier, m0123, m123, m23, seg.p3};
        return {left, right};
    };

    auto [a1, a2] = subdivide(a);
    auto [b1, b2] = subdivide(b);

    intersectBezierRecursive(a1, b1, intersections, depth + 1);
    intersectBezierRecursive(a1, b2, intersections, depth + 1);
    intersectBezierRecursive(a2, b1, intersections, depth + 1);
    intersectBezierRecursive(a2, b2, intersections, depth + 1);
}

void SketchGLWidget::toggleBezierMode()
{
    drawMode = (drawMode == DrawMode::Line) ? DrawMode::Bezier : DrawMode::Line;
    bezierMode = (drawMode == DrawMode::Bezier);
    bezierControlPoints.clear();
    update();
}

void SketchGLWidget::findBezierIntersections()
{
    bezierIntersections.clear();
    for (size_t i = 0; i < shapeCurves.size(); ++i)
    {
        for (const auto &a : shapeCurves[i])
        {
            if (a.type != CurveSegment::CubicBezier)
                continue;
            for (size_t j = i + 1; j < shapeCurves.size(); ++j)
            {
                for (const auto &b : shapeCurves[j])
                {
                    if (b.type != CurveSegment::CubicBezier)
                        continue;
                    intersectBezierRecursive(a, b, bezierIntersections);
                }
            }
        }
    }
    update();
}

void SketchGLWidget::completeShape()
{
    if (tempPoints.size() < 3)
        return;

    allShapes.push_back(tempPoints);
    qDebug() << "Shape completed with" << tempPoints.size() - 1 << "vertices.";
    tempPoints.clear();
    update();
}

void SketchGLWidget::performUnion()
{
    if (allShapes.size() < 2)
        return;
    const auto &a = allShapes[0];
    const auto &b = allShapes[1];
    auto res = PolygonBoolean::unionPolygons(a, b);
    if (!res.empty())
    {
        computedRegions.clear();
        computedRegions.push_back(res);
        selectedRegionIdx = 0;
        update();
    }
}

void SketchGLWidget::performIntersection()
{
    if (allShapes.size() < 2)
        return;
    const auto &a = allShapes[0];
    const auto &b = allShapes[1];
    auto res = PolygonBoolean::intersect(a, b);
    if (!res.empty())
    {
        computedRegions.clear();
        computedRegions.push_back(res);
        selectedRegionIdx = 0;
        update();
    }
}

void SketchGLWidget::performSubtraction(bool abOrder)
{
    if (allShapes.size() < 2)
        return;
    const auto &a = abOrder ? allShapes[0] : allShapes[1];
    const auto &b = abOrder ? allShapes[1] : allShapes[0];
    auto res = PolygonBoolean::subtractPolygons(a, b);
    if (!res.empty())
    {
        allShapes = {res};
        computedRegions.clear();
        selectedRegionIdx = -1;
        update();
    }
}

void SketchGLWidget::resetResult()
{
    computedRegions.clear();
    selectedRegionIdx = -1;
    computedColor = Qt::transparent;
    update();
}

std::vector<std::vector<QPointF>> SketchGLWidget::polygons() const
{
    return allShapes;
}

std::vector<QPointF> SketchGLWidget::currentRegionPolygon() const
{
    if (selectedRegionIdx >= 0 && selectedRegionIdx < computedRegions.size())
        return computedRegions[selectedRegionIdx];
    return {};
}
bool SketchGLWidget::isBezierModeActive() const
{
    return bezierMode;
}
