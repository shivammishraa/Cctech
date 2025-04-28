#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    // Setup OpenGL format before QApplication is created
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);
    fmt.setVersion(2, 1);  // Safe fallback
    fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    QApplication app(argc, argv);

    qDebug() << "Starting application...";
    MainWindow window;
    window.setWindowTitle("Geometery Visualizer");
    window.resize(800, 600);

    window.show();

    return app.exec();
}
