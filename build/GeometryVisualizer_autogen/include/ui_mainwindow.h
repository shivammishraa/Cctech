/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *topBarLayout;
    QLabel *shapeLabel;
    QComboBox *Shapes;
    QPushButton *pushButton;
    QPushButton *uploadButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *dimensionInputsLayout;
    GLWidget *openGLWidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(362, 105);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        topBarLayout = new QHBoxLayout();
        topBarLayout->setObjectName("topBarLayout");
        shapeLabel = new QLabel(centralwidget);
        shapeLabel->setObjectName("shapeLabel");

        topBarLayout->addWidget(shapeLabel);

        Shapes = new QComboBox(centralwidget);
        Shapes->addItem(QString());
        Shapes->addItem(QString());
        Shapes->addItem(QString());
        Shapes->addItem(QString());
        Shapes->addItem(QString());
        Shapes->addItem(QString());
        Shapes->setObjectName("Shapes");

        topBarLayout->addWidget(Shapes);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");

        topBarLayout->addWidget(pushButton);

        uploadButton = new QPushButton(centralwidget);
        uploadButton->setObjectName("uploadButton");

        topBarLayout->addWidget(uploadButton);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(topBarLayout);

        dimensionInputsLayout = new QHBoxLayout();
        dimensionInputsLayout->setObjectName("dimensionInputsLayout");

        verticalLayout->addLayout(dimensionInputsLayout);

        openGLWidget = new GLWidget(centralwidget);
        openGLWidget->setObjectName("openGLWidget");

        verticalLayout->addWidget(openGLWidget);

        verticalLayout->setStretch(2, 1);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "STL Viewer - OpenGL & Qt", nullptr));
        shapeLabel->setText(QCoreApplication::translate("MainWindow", "Select Shape:", nullptr));
        Shapes->setItemText(0, QCoreApplication::translate("MainWindow", "Cuboid", nullptr));
        Shapes->setItemText(1, QCoreApplication::translate("MainWindow", "Polygon", nullptr));
        Shapes->setItemText(2, QCoreApplication::translate("MainWindow", "Cylinder", nullptr));
        Shapes->setItemText(3, QCoreApplication::translate("MainWindow", "Triangle", nullptr));
        Shapes->setItemText(4, QCoreApplication::translate("MainWindow", "Sphere", nullptr));
        Shapes->setItemText(5, QCoreApplication::translate("MainWindow", "Polyline", nullptr));

        pushButton->setText(QCoreApplication::translate("MainWindow", "Plot Shape", nullptr));
        uploadButton->setText(QCoreApplication::translate("MainWindow", "Upload File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
