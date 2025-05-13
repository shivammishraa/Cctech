
#ifndef BOOLEANOPERATIONWIDGET_H
#define BOOLEANOPERATIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector2D>
#include <QVector3D>
#include <QMouseEvent>
#include <vector>

class BooleanOperationWidget : public QWidget {
    Q_OBJECT

public:
    explicit BooleanOperationWidget(QWidget* parent = nullptr);
        std::vector<std::vector<QVector3D>> resultFaces;
        std::vector<std::vector<QVector2D>> shapeFacesA;
        std::vector<std::vector<QVector2D>> shapeFacesB;
        std::vector<std::vector<QVector2D>> result2D;
        std::vector<QVector2D> polygonA2D;
        std::vector<QVector2D> polygonB2D;

private slots:
    void onPlotShapeA();
    void onPlotShapeB();
    void onIntersectionClicked();
    void onSubtractionClicked();
    void onUnionClicked();

private:
    bool pointInsidePolygon(const std::vector<QVector2D>& polygon, const QVector2D& point, bool& inside);
    void setupUI();
    void performIntersection();
    void performSubtraction();
    void performUnion();

    // UI elements
    QComboBox* shapeAComboBox;
    QComboBox* shapeBComboBox;
    QPushButton* plotShapeAButton;
    QPushButton* plotShapeBButton;
    QPushButton* intersectionButton;
    QPushButton* subtractionButton;
    QPushButton* unionButton;

    // 2D shape data
    std::vector<std::vector<QVector2D>> shapeA2D;
    std::vector<std::vector<QVector2D>> shapeB2D;

    QVector2D translationA2D;
    QVector2D translationB2D;
    float scaleA2D;
    float scaleB2D;

    // GLWidget nested class
    class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    public:
        explicit GLWidget(BooleanOperationWidget* parent);

        void setShapes2D(const std::vector<std::vector<QVector2D>>& a,
                         const std::vector<std::vector<QVector2D>>& b,
                         const std::vector<std::vector<QVector2D>>& r);

        void setEdges(const std::vector<std::pair<QVector3D, QVector3D>>&,
                      const std::vector<std::pair<QVector3D, QVector3D>>&,
                      const std::vector<std::pair<QVector3D, QVector3D>>&);

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;

    private:
        void renderShape2D(const std::vector<QVector2D>& shape, const QVector3D& color,
                           const QVector2D& translation, float scale);
        void renderEdges(const std::vector<std::pair<QVector3D, QVector3D>>&,
                         const QVector3D&, const QVector3D&, const QVector3D&);

        QPoint lastMousePos;
        int draggingShape; // 0 for A, 1 for B, -1 for none

        std::vector<std::vector<QVector2D>> shapeA2D;
        std::vector<std::vector<QVector2D>> shapeB2D;
        std::vector<std::vector<QVector2D>> result2D;

        QVector2D translationA2D;
        QVector2D translationB2D;
        float scaleA2D;
        float scaleB2D;

        BooleanOperationWidget* parent;
    };

    GLWidget* glWidget;
};

#endif // BOOLEANOPERATIONWIDGET_H