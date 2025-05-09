#include "BooleanOperationWidget.h"
#include "shapefactory.h"
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

BooleanOperationWidget::BooleanOperationWidget(QWidget* parent)
    : QWidget(parent), glWidget(new GLWidget(this)), translationA(0, 0, 0), translationB(0, 0, 0) {
    setupUI();
}

void BooleanOperationWidget::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* layoutA = new QHBoxLayout();
    layoutA->addWidget(new QLabel("Shape A:"));
    shapeAComboBox = new QComboBox();
    layoutA->addWidget(shapeAComboBox);

    QHBoxLayout* layoutB = new QHBoxLayout();
    layoutB->addWidget(new QLabel("Shape B:"));
    shapeBComboBox = new QComboBox();
    layoutB->addWidget(shapeBComboBox);

    QStringList shapes = {"Cuboid", "Sphere", "Cylinder", "Bezier", "Polygon", "Polyline"};
    shapeAComboBox->addItems(shapes);
    shapeBComboBox->addItems(shapes);

    plotShapeAButton = new QPushButton("Plot Shape A");
    plotShapeBButton = new QPushButton("Plot Shape B");
    performOperationButton = new QPushButton("Perform Boolean Operation");
    intersectionButton = new QPushButton("Intersection");

    layout->addLayout(layoutA);
    layout->addLayout(layoutB);
    layout->addWidget(plotShapeAButton);
    layout->addWidget(plotShapeBButton);
    layout->addWidget(glWidget);
    layout->addWidget(performOperationButton);
    layout->addWidget(intersectionButton);

    connect(plotShapeAButton, &QPushButton::clicked, this, &BooleanOperationWidget::onPlotShapeA);
    connect(plotShapeBButton, &QPushButton::clicked, this, &BooleanOperationWidget::onPlotShapeB);
    connect(performOperationButton, &QPushButton::clicked, this, &BooleanOperationWidget::onPerformOperation);
    connect(intersectionButton, &QPushButton::clicked, this, &BooleanOperationWidget::onIntersectionClicked);
}

void BooleanOperationWidget::onPlotShapeA() {
    auto shape = createShapeFromDialog(shapeAComboBox->currentText(), this);
    if (shape) {
        shapeEdgesA.clear();
        for (const auto& edge : shape->getEdges()) {
            if (edge.first.size() == 3 && edge.second.size() == 3) {
                shapeEdgesA.emplace_back(QVector3D(edge.first[0], edge.first[1], edge.first[2]),
                                         QVector3D(edge.second[0], edge.second[1], edge.second[2]));
            }
        }
        glWidget->setEdges(shapeEdgesA, shapeEdgesB);
    }
}

void BooleanOperationWidget::onPlotShapeB() {
    auto shape = createShapeFromDialog(shapeBComboBox->currentText(), this);
    if (shape) {
        shapeEdgesB.clear();
        for (const auto& edge : shape->getEdges()) {
            if (edge.first.size() == 3 && edge.second.size() == 3) {
                shapeEdgesB.emplace_back(QVector3D(edge.first[0], edge.first[1], edge.first[2]),
                                         QVector3D(edge.second[0], edge.second[1], edge.second[2]));
            }
        }
        glWidget->setEdges(shapeEdgesA, shapeEdgesB);
    }
}

void BooleanOperationWidget::onPerformOperation() {
    QStringList ops = {"A - B", "A + B (Union)", "A ∩ B (Intersection)"};
    bool ok;
    QString op = QInputDialog::getItem(this, "Choose Operation", "Operation:", ops, 0, false, &ok);
    if (ok && !op.isEmpty()) performBooleanOperation(op);
}

void BooleanOperationWidget::onIntersectionClicked() {
    performIntersection();
    glWidget->setEdges(shapeEdgesA, shapeEdgesB);
}

void BooleanOperationWidget::performBooleanOperation(const QString& op) {
    if (op == "A - B") performSubtraction();
    else if (op == "A + B (Union)") performUnion();
    else if (op == "A ∩ B (Intersection)") performIntersection();
    glWidget->setEdges(shapeEdgesA, shapeEdgesB);
}

// --------------------------
// Simulated Boolean Logic
// --------------------------

void BooleanOperationWidget::performSubtraction() {
    // Simulated subtraction: remove edges from A that match B
    std::vector<std::pair<QVector3D, QVector3D>> result;
    for (const auto& edgeA : shapeEdgesA) {
        bool foundInB = false;
        for (const auto& edgeB : shapeEdgesB) {
            if ((edgeA.first == edgeB.first && edgeA.second == edgeB.second) ||
                (edgeA.first == edgeB.second && edgeA.second == edgeB.first)) {
                foundInB = true;
                break;
            }
        }
        if (!foundInB) result.push_back(edgeA);
    }
    shapeEdgesA = result;
}

void BooleanOperationWidget::performUnion() {
    // Simple union: merge all edges
    for (const auto& edgeB : shapeEdgesB) {
        shapeEdgesA.push_back(edgeB);
    }
}

void BooleanOperationWidget::performIntersection() {
    // Simulated intersection: keep only common edges
    std::vector<std::pair<QVector3D, QVector3D>> result;
    for (const auto& edgeA : shapeEdgesA) {
        for (const auto& edgeB : shapeEdgesB) {
            if ((edgeA.first == edgeB.first && edgeA.second == edgeB.second) ||
                (edgeA.first == edgeB.second && edgeA.second == edgeB.first)) {
                result.push_back(edgeA);
                break;
            }
        }
    }
    shapeEdgesA = result;
}

// --- GLWidget ---

BooleanOperationWidget::GLWidget::GLWidget(BooleanOperationWidget* parent)
    : QOpenGLWidget(parent), parent(parent) {}

void BooleanOperationWidget::GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
}

void BooleanOperationWidget::GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = float(w) / float(h);
    float fovy = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    float top = near * tan(fovy * M_PI / 360.0f);
    float bottom = -top;
    float left = bottom * aspect;
    float right = top * aspect;
    glFrustum(left, right, bottom, top, near, far);
    glMatrixMode(GL_MODELVIEW);
}

void BooleanOperationWidget::GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0, 0, -10);
    renderEdges(edgesA, {1, 0, 0}, translationA);
    renderEdges(edgesB, {0, 0, 1}, translationB);
}

void BooleanOperationWidget::GLWidget::setEdges(const std::vector<std::pair<QVector3D, QVector3D>>& a,
                                                const std::vector<std::pair<QVector3D, QVector3D>>& b) {
    edgesA = a;
    edgesB = b;
    update();
}

void BooleanOperationWidget::GLWidget::setTranslations(const QVector3D& tA, const QVector3D& tB) {
    translationA = tA;
    translationB = tB;
    update();
}

void BooleanOperationWidget::GLWidget::renderEdges(const std::vector<std::pair<QVector3D, QVector3D>>& edges,
                                                   const QVector3D& color, const QVector3D& translation) {
    glPushMatrix();
    glTranslatef(translation.x(), translation.y(), translation.z());
    glColor3f(color.x(), color.y(), color.z());

    for (const auto& edge : edges) {
        glBegin(GL_LINES);
        glVertex3f(edge.first.x(), edge.first.y(), edge.first.z());
        glVertex3f(edge.second.x(), edge.second.y(), edge.second.z());
        glEnd();
    }

    glPopMatrix();
}

void BooleanOperationWidget::GLWidget::mousePressEvent(QMouseEvent* event) {
    lastMousePos = event->pos();
    draggingShapeA = (event->button() == Qt::LeftButton);
}

void BooleanOperationWidget::GLWidget::mouseMoveEvent(QMouseEvent* event) {
    QPoint delta = event->pos() - lastMousePos;
    QVector3D movement(float(delta.x()) / 100.0f, -float(delta.y()) / 100.0f, 0);
    if (draggingShapeA)
        translationA += movement;
    else
        translationB += movement;

    parent->translationA = translationA;
    parent->translationB = translationB;
    update();
    lastMousePos = event->pos();
}
