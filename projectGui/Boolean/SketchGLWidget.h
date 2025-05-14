#ifndef SKETCHGLWIDGET_H
#define SKETCHGLWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QColor>
#include <QVector3D>
#include <vector>
#include <QPointF>
#include <array>

class SketchGLWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit SketchGLWidget(QWidget* parent = nullptr);

    void completeShape();
    void performUnion();
    void performIntersection();
    void performSubtraction(bool abOrder);

    std::vector<std::vector<QPointF>> polygons() const;
    int currentRegionIndex() const { return selectedRegionIdx; }
    std::vector<QPointF> currentRegionPolygon() const;

    void resetResult();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    struct MeshData {
        std::vector<QVector3D> verts;
        std::vector<unsigned int> inds;
    };

    void draw2DGeometry();

    int selectedRegionIdx = -1;
    std::vector<QPointF> tempPoints;
    std::vector<std::vector<QPointF>> allShapes;
    std::vector<std::vector<QPointF>> computedRegions;
    std::vector<QPointF> highlightedRegion;
    QColor computedColor;
};

bool pointFuzzyEqual(const QPointF& a, const QPointF& b, double eps = 1e-3);

#endif // SKETCHGLWIDGET_H
