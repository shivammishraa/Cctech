#ifndef BOOLEANOPERATIONWIDGET_H
#define BOOLEANOPERATIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class BooleanOperationWidget : public QWidget {
    Q_OBJECT

public:
    explicit BooleanOperationWidget(QWidget* parent = nullptr);

private:
    class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    public:
        GLWidget(BooleanOperationWidget* parent);
        void setEdges(const std::vector<std::pair<QVector3D, QVector3D>>& edgesA,
                      const std::vector<std::pair<QVector3D, QVector3D>>& edgesB);
        void setTranslations(const QVector3D& transA, const QVector3D& transB);

    protected:
        void initializeGL() override;
        void paintGL() override;
        void resizeGL(int w, int h) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

    private:
        BooleanOperationWidget* parent;
        QPoint lastMousePos;
        bool draggingShapeA = true;
        std::vector<std::pair<QVector3D, QVector3D>> edgesA, edgesB;
        QVector3D translationA, translationB;
        void renderEdges(const std::vector<std::pair<QVector3D, QVector3D>>& edges, const QVector3D& color, const QVector3D& translation);
    };

    GLWidget* glWidget;
    QComboBox* shapeAComboBox;
    QComboBox* shapeBComboBox;
    QPushButton* plotShapeAButton;
    QPushButton* plotShapeBButton;
    QPushButton* performOperationButton;
    QPushButton* intersectionButton;

    std::vector<std::pair<QVector3D, QVector3D>> shapeEdgesA;
    std::vector<std::pair<QVector3D, QVector3D>> shapeEdgesB;
    QVector3D translationA;
    QVector3D translationB;

    void setupUI();
    void performBooleanOperation(const QString& op);
    void performSubtraction();
    void performUnion();
    void performIntersection();

private slots:
    void onPlotShapeA();
    void onPlotShapeB();
    void onPerformOperation();
    void onIntersectionClicked();
};

#endif // BOOLEANOPERATIONWIDGET_H
