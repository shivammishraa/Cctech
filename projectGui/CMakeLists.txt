cmake_minimum_required(VERSION 3.14)
project(GeometryVisualizer)

set(CMAKE_CXX_STANDARD 17)

# Enable Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set Qt installation path (update if needed)
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64/lib/cmake")

# Find Qt6 and OpenGL dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
find_package(OpenGL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../geometry/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../application/src
    ${CMAKE_CURRENT_SOURCE_DIR}/projectGui      
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Source files
set(SOURCES
    main.cpp
    glwidget.cpp
    BezierWidget.cpp
    PolygonWidget.cpp
    mainwindow.cpp
    shapeinputdialog.cpp
    shapefactory.cpp               
    ../geometry/src/stl_shape.cpp
    ../geometry/src/bezier.cpp
    ../geometry/src/cuboid.cpp
    ../geometry/src/cylinder.cpp
    ../geometry/src/line3D.cpp
    ../geometry/src/objToStl.cpp
    ../geometry/src/obj_shape.cpp
    ../geometry/src/plot_utils.cpp
    ../geometry/src/polygon.cpp
    ../geometry/src/polyline.cpp
    ../geometry/src/scene.cpp
    ../geometry/src/shape_handlers.cpp
    ../geometry/src/sphere.cpp
    ../geometry/src/transform_utils.cpp
    ../geometry/src/triangle.cpp
)

# Header files
set(HEADERS
    glwidget.h
    BezierWidget.h
    PolygonWidget.h
    mainwindow.h
    shapeinputdialog.h
    shape3d.h                      
    shapefactory.h                 
    ../geometry/include/stl_shape.h
    ../geometry/include/bezier.h
    ../geometry/include/cuboid.h
    ../geometry/include/cylinder.h
    ../geometry/include/line3D.h
    ../geometry/include/objToStl.h
    ../geometry/include/obj_shape.h
    ../geometry/include/plot_utils.h
    ../geometry/include/polygon.h
    ../geometry/include/polyline.h
    ../geometry/include/scene.h
    ../geometry/include/shape_handlers.h
    ../geometry/include/sphere.h
    ../geometry/include/transform_utils.h
    ../geometry/include/triangle.h
    ../geometry/include/shape.h
)

# UI files
set(UIS
    mainwindow.ui
)

# Treat this as a Windows GUI app (no console window)
set(CMAKE_WIN32_EXECUTABLE ON)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UIS})

# Link with Qt and OpenGL
target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
)
