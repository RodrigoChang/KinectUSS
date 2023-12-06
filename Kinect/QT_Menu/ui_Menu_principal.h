/********************************************************************************
** Form generated from reading UI file 'Menu_principalxlMvjZ.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MENU_PRINCIPALXLMVJZ_H
#define MENU_PRINCIPALXLMVJZ_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectUSS
{
public:
    QAction *actionDepth;
    QAction *actionIr_Camera;
    QWidget *centralwidget;
    QFrame *frame;
    QWidget *gridLayoutWidget_2;
    QGridLayout *Coef_Distorsion_Grid;
    QSlider *Coef_Distorsion_Slider_2;
    QSlider *Coef_Distorsion_Slider_1;
    QLabel *Label_Coef_Distorsion_2;
    QLabel *label_8;
    QLabel *Label_Coef_Distorsion_1;
    QLabel *label_10;
    QLabel *Label_Coef_Distorsion_3;
    QSlider *Coef_Distorsion_Slider_3;
    QLabel *label_9;
    QWidget *gridLayoutWidget;
    QGridLayout *Cameras_Grid;
    QLabel *Label_Focal_X_Ir;
    QSlider *Slider_focal_X_Ir;
    QLabel *label_6;
    QSlider *Slider_principal_X_Ir;
    QLabel *Label_Focal_Y_Ir;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_3;
    QSlider *Slider_principal_Y_Ir;
    QLabel *label_2;
    QSlider *Slider_focal_Y_Ir;
    QPushButton *Reset_Kinect;
    QMenuBar *menubar;
    QMenu *menuOpciones;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *KinectUSS)
    {
        if (KinectUSS->objectName().isEmpty())
            KinectUSS->setObjectName("KinectUSS");
        KinectUSS->resize(820, 573);
        actionDepth = new QAction(KinectUSS);
        actionDepth->setObjectName("actionDepth");
        actionIr_Camera = new QAction(KinectUSS);
        actionIr_Camera->setObjectName("actionIr_Camera");
        centralwidget = new QWidget(KinectUSS);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setAutoFillBackground(false);
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(10, 0, 761, 241));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayoutWidget_2 = new QWidget(frame);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(370, 10, 381, 191));
        Coef_Distorsion_Grid = new QGridLayout(gridLayoutWidget_2);
        Coef_Distorsion_Grid->setObjectName("Coef_Distorsion_Grid");
        Coef_Distorsion_Grid->setContentsMargins(0, 0, 0, 0);
        Coef_Distorsion_Slider_2 = new QSlider(gridLayoutWidget_2);
        Coef_Distorsion_Slider_2->setObjectName("Coef_Distorsion_Slider_2");
        Coef_Distorsion_Slider_2->setOrientation(Qt::Horizontal);

        Coef_Distorsion_Grid->addWidget(Coef_Distorsion_Slider_2, 1, 1, 1, 1);

        Coef_Distorsion_Slider_1 = new QSlider(gridLayoutWidget_2);
        Coef_Distorsion_Slider_1->setObjectName("Coef_Distorsion_Slider_1");
        Coef_Distorsion_Slider_1->setOrientation(Qt::Horizontal);

        Coef_Distorsion_Grid->addWidget(Coef_Distorsion_Slider_1, 0, 1, 1, 1);

        Label_Coef_Distorsion_2 = new QLabel(gridLayoutWidget_2);
        Label_Coef_Distorsion_2->setObjectName("Label_Coef_Distorsion_2");

        Coef_Distorsion_Grid->addWidget(Label_Coef_Distorsion_2, 1, 0, 1, 1);

        label_8 = new QLabel(gridLayoutWidget_2);
        label_8->setObjectName("label_8");

        Coef_Distorsion_Grid->addWidget(label_8, 0, 2, 1, 1);

        Label_Coef_Distorsion_1 = new QLabel(gridLayoutWidget_2);
        Label_Coef_Distorsion_1->setObjectName("Label_Coef_Distorsion_1");

        Coef_Distorsion_Grid->addWidget(Label_Coef_Distorsion_1, 0, 0, 1, 1);

        label_10 = new QLabel(gridLayoutWidget_2);
        label_10->setObjectName("label_10");

        Coef_Distorsion_Grid->addWidget(label_10, 1, 2, 1, 1);

        Label_Coef_Distorsion_3 = new QLabel(gridLayoutWidget_2);
        Label_Coef_Distorsion_3->setObjectName("Label_Coef_Distorsion_3");

        Coef_Distorsion_Grid->addWidget(Label_Coef_Distorsion_3, 2, 0, 1, 1);

        Coef_Distorsion_Slider_3 = new QSlider(gridLayoutWidget_2);
        Coef_Distorsion_Slider_3->setObjectName("Coef_Distorsion_Slider_3");
        Coef_Distorsion_Slider_3->setOrientation(Qt::Horizontal);

        Coef_Distorsion_Grid->addWidget(Coef_Distorsion_Slider_3, 2, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_2);
        label_9->setObjectName("label_9");

        Coef_Distorsion_Grid->addWidget(label_9, 2, 2, 1, 1);

        gridLayoutWidget = new QWidget(frame);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 311, 191));
        Cameras_Grid = new QGridLayout(gridLayoutWidget);
        Cameras_Grid->setObjectName("Cameras_Grid");
        Cameras_Grid->setContentsMargins(0, 0, 0, 0);
        Label_Focal_X_Ir = new QLabel(gridLayoutWidget);
        Label_Focal_X_Ir->setObjectName("Label_Focal_X_Ir");
        Label_Focal_X_Ir->setLayoutDirection(Qt::LeftToRight);
        Label_Focal_X_Ir->setAlignment(Qt::AlignCenter);

        Cameras_Grid->addWidget(Label_Focal_X_Ir, 0, 0, 1, 1);

        Slider_focal_X_Ir = new QSlider(gridLayoutWidget);
        Slider_focal_X_Ir->setObjectName("Slider_focal_X_Ir");
        Slider_focal_X_Ir->setOrientation(Qt::Horizontal);

        Cameras_Grid->addWidget(Slider_focal_X_Ir, 0, 1, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName("label_6");

        Cameras_Grid->addWidget(label_6, 3, 2, 1, 1);

        Slider_principal_X_Ir = new QSlider(gridLayoutWidget);
        Slider_principal_X_Ir->setObjectName("Slider_principal_X_Ir");
        Slider_principal_X_Ir->setOrientation(Qt::Horizontal);

        Cameras_Grid->addWidget(Slider_principal_X_Ir, 2, 1, 1, 1);

        Label_Focal_Y_Ir = new QLabel(gridLayoutWidget);
        Label_Focal_Y_Ir->setObjectName("Label_Focal_Y_Ir");
        Label_Focal_Y_Ir->setAlignment(Qt::AlignCenter);

        Cameras_Grid->addWidget(Label_Focal_Y_Ir, 1, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName("label_5");
        label_5->setAlignment(Qt::AlignCenter);

        Cameras_Grid->addWidget(label_5, 3, 0, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName("label");

        Cameras_Grid->addWidget(label, 1, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName("label_4");

        Cameras_Grid->addWidget(label_4, 0, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName("label_3");

        Cameras_Grid->addWidget(label_3, 2, 2, 1, 1);

        Slider_principal_Y_Ir = new QSlider(gridLayoutWidget);
        Slider_principal_Y_Ir->setObjectName("Slider_principal_Y_Ir");
        Slider_principal_Y_Ir->setOrientation(Qt::Horizontal);

        Cameras_Grid->addWidget(Slider_principal_Y_Ir, 3, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        Cameras_Grid->addWidget(label_2, 2, 0, 1, 1);

        Slider_focal_Y_Ir = new QSlider(gridLayoutWidget);
        Slider_focal_Y_Ir->setObjectName("Slider_focal_Y_Ir");
        Slider_focal_Y_Ir->setOrientation(Qt::Horizontal);

        Cameras_Grid->addWidget(Slider_focal_Y_Ir, 1, 1, 1, 1);

        Reset_Kinect = new QPushButton(frame);
        Reset_Kinect->setObjectName("Reset_Kinect");
        Reset_Kinect->setGeometry(QRect(10, 210, 311, 21));
        KinectUSS->setCentralWidget(centralwidget);
        menubar = new QMenuBar(KinectUSS);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 820, 22));
        menuOpciones = new QMenu(menubar);
        menuOpciones->setObjectName("menuOpciones");
        KinectUSS->setMenuBar(menubar);
        statusbar = new QStatusBar(KinectUSS);
        statusbar->setObjectName("statusbar");
        KinectUSS->setStatusBar(statusbar);

        menubar->addAction(menuOpciones->menuAction());
        menuOpciones->addAction(actionDepth);
        menuOpciones->addAction(actionIr_Camera);

        retranslateUi(KinectUSS);

        QMetaObject::connectSlotsByName(KinectUSS);
    } // setupUi

    void retranslateUi(QMainWindow *KinectUSS)
    {
        KinectUSS->setWindowTitle(QCoreApplication::translate("KinectUSS", "MainWindow", nullptr));
        actionDepth->setText(QCoreApplication::translate("KinectUSS", "Depth", nullptr));
        actionIr_Camera->setText(QCoreApplication::translate("KinectUSS", "Ir Camera", nullptr));
        Label_Coef_Distorsion_2->setText(QCoreApplication::translate("KinectUSS", "Radial distortion coef 2", nullptr));
        label_8->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        Label_Coef_Distorsion_1->setText(QCoreApplication::translate("KinectUSS", "Radial distortion coef 1", nullptr));
        label_10->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        Label_Coef_Distorsion_3->setText(QCoreApplication::translate("KinectUSS", "Radial distortion coef 3", nullptr));
        label_9->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        Label_Focal_X_Ir->setText(QCoreApplication::translate("KinectUSS", "Punto Focal X Ir", nullptr));
        label_6->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        Label_Focal_Y_Ir->setText(QCoreApplication::translate("KinectUSS", "Punto Focal Y Ir", nullptr));
        label_5->setText(QCoreApplication::translate("KinectUSS", "Punto principal Y Ir", nullptr));
        label->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        label_4->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("KinectUSS", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("KinectUSS", "Punto principal X Ir", nullptr));
        Reset_Kinect->setText(QCoreApplication::translate("KinectUSS", "Reiniciar Kinect", nullptr));
        menuOpciones->setTitle(QCoreApplication::translate("KinectUSS", "Opciones", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KinectUSS: public Ui_KinectUSS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MENU_PRINCIPALXLMVJZ_H
