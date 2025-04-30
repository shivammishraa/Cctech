#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QVector>
#include <QMouseEvent>
#include <QSet>
#include "Structure.h" // Include the data structures for vertices, edges, and faces

class PolygonWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit PolygonWidget(QWidget *parent = nullptr);
    ~PolygonWidget();

    void addShape(const QString &shapeName);
    void clearCurrentShape();
    void displayShape(const QString &shapeName);
    void adjustZAxis();
    void deleteShape(const QString &shapeName);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<QVector3D> polygonPoints; // Points defining the current polygon
    QVector3D polygonNormal;          // Normal vector of the polygon
    QVector<QVector3D> extrudedVertices; // Vertices after extrusion

    QVector<Face*> faces; // Use Face structure for drawing
    QVector<QString> shapeNames; // Store shape names
    QVector<QVector<Face*>> shapeData; // Store shape data
    bool polygonClosed = false;
    float extrusionHeight = 0.0f;

    float rotationX = 0.0f; // Rotation angle around the X-axis
    float rotationY = 0.0f; // Rotation angle around the Y-axis
    bool rightMousePressed = false; // Track if the right mouse button is pressed
    QPoint lastMousePos; // To track the last mouse position for rotation
    float zOffset = 0.1f; // Z-axis offset to avoid overlapping

    QVector3D screenToWorld(const QPoint &pos);
    bool isCloseToFirstPoint(const QVector3D &p);
    QVector3D computePolygonNormal(const QVector<QVector3D> &points);
    void generateExtrudedFaces(); // Generate faces for the extruded shape
};
