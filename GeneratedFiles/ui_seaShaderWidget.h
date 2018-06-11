/********************************************************************************
** Form generated from reading UI file 'seaShaderWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEASHADERWIDGET_H
#define UI_SEASHADERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include <shaderplay.h>

QT_BEGIN_NAMESPACE

class Ui_seaShaderWidget
{
public:
    QHBoxLayout *horizontalLayout;
    ShaderPlay *openGLWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *btnBaseColor;
    QLabel *lb1;
    QLabel *lb2;
    QLabel *lb5;
    QLabel *lb4;
    QLabel *lb3;
    QSpinBox *IntIter_Geo;
    QDoubleSpinBox *dblSpeed;
    QSpinBox *intIter_Frag;
    QDoubleSpinBox *dblHeight;
    QDoubleSpinBox *dblchoppy;
    QLabel *lb6;
    QDoubleSpinBox *dblFrequency;
    QLabel *lb7;
    QPushButton *btnWaterColor;
    QLabel *lb8;
    QPushButton *btnLoad;

    void setupUi(QWidget *seaShaderWidget)
    {
        if (seaShaderWidget->objectName().isEmpty())
            seaShaderWidget->setObjectName(QStringLiteral("seaShaderWidget"));
        seaShaderWidget->resize(599, 377);
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        seaShaderWidget->setFont(font);
        horizontalLayout = new QHBoxLayout(seaShaderWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openGLWidget = new ShaderPlay(seaShaderWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setMinimumSize(QSize(253, 0));

        horizontalLayout->addWidget(openGLWidget);

        groupBox = new QGroupBox(seaShaderWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(100, 359));
        groupBox->setLayoutDirection(Qt::LeftToRight);
        groupBox->setCheckable(false);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        btnBaseColor = new QPushButton(groupBox);
        btnBaseColor->setObjectName(QStringLiteral("btnBaseColor"));

        gridLayout->addWidget(btnBaseColor, 7, 1, 1, 1);

        lb1 = new QLabel(groupBox);
        lb1->setObjectName(QStringLiteral("lb1"));
        lb1->setFont(font);

        gridLayout->addWidget(lb1, 1, 0, 1, 1);

        lb2 = new QLabel(groupBox);
        lb2->setObjectName(QStringLiteral("lb2"));

        gridLayout->addWidget(lb2, 6, 0, 1, 1);

        lb5 = new QLabel(groupBox);
        lb5->setObjectName(QStringLiteral("lb5"));

        gridLayout->addWidget(lb5, 2, 0, 1, 1);

        lb4 = new QLabel(groupBox);
        lb4->setObjectName(QStringLiteral("lb4"));

        gridLayout->addWidget(lb4, 3, 0, 1, 1);

        lb3 = new QLabel(groupBox);
        lb3->setObjectName(QStringLiteral("lb3"));

        gridLayout->addWidget(lb3, 7, 0, 1, 1);

        IntIter_Geo = new QSpinBox(groupBox);
        IntIter_Geo->setObjectName(QStringLiteral("IntIter_Geo"));
        IntIter_Geo->setMaximum(10);

        gridLayout->addWidget(IntIter_Geo, 1, 1, 1, 1);

        dblSpeed = new QDoubleSpinBox(groupBox);
        dblSpeed->setObjectName(QStringLiteral("dblSpeed"));
        dblSpeed->setMaximum(10);
        dblSpeed->setSingleStep(0.1);

        gridLayout->addWidget(dblSpeed, 5, 1, 1, 1);

        intIter_Frag = new QSpinBox(groupBox);
        intIter_Frag->setObjectName(QStringLiteral("intIter_Frag"));
        intIter_Frag->setMaximum(10);

        gridLayout->addWidget(intIter_Frag, 2, 1, 1, 1);

        dblHeight = new QDoubleSpinBox(groupBox);
        dblHeight->setObjectName(QStringLiteral("dblHeight"));
        dblHeight->setMaximum(10);
        dblHeight->setSingleStep(0.1);

        gridLayout->addWidget(dblHeight, 3, 1, 1, 1);

        dblchoppy = new QDoubleSpinBox(groupBox);
        dblchoppy->setObjectName(QStringLiteral("dblchoppy"));
        dblchoppy->setMaximum(10);
        dblchoppy->setSingleStep(0.1);

        gridLayout->addWidget(dblchoppy, 4, 1, 1, 1);

        lb6 = new QLabel(groupBox);
        lb6->setObjectName(QStringLiteral("lb6"));

        gridLayout->addWidget(lb6, 5, 0, 1, 1);

        dblFrequency = new QDoubleSpinBox(groupBox);
        dblFrequency->setObjectName(QStringLiteral("dblFrequency"));
        dblFrequency->setMaximum(10);
        dblFrequency->setSingleStep(0.1);

        gridLayout->addWidget(dblFrequency, 6, 1, 1, 1);

        lb7 = new QLabel(groupBox);
        lb7->setObjectName(QStringLiteral("lb7"));

        gridLayout->addWidget(lb7, 8, 0, 1, 1);

        btnWaterColor = new QPushButton(groupBox);
        btnWaterColor->setObjectName(QStringLiteral("btnWaterColor"));

        gridLayout->addWidget(btnWaterColor, 8, 1, 1, 1);

        lb8 = new QLabel(groupBox);
        lb8->setObjectName(QStringLiteral("lb8"));

        gridLayout->addWidget(lb8, 4, 0, 1, 1);

        btnLoad = new QPushButton(groupBox);
        btnLoad->setObjectName(QStringLiteral("btnLoad"));

        gridLayout->addWidget(btnLoad, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);


        retranslateUi(seaShaderWidget);
        QObject::connect(dblHeight, SIGNAL(valueChanged(double)), seaShaderWidget, SLOT(heightVal(double)));
        QObject::connect(dblchoppy, SIGNAL(valueChanged(double)), seaShaderWidget, SLOT(choopyVal(double)));
        QObject::connect(dblSpeed, SIGNAL(valueChanged(double)), seaShaderWidget, SLOT(speedVal(double)));
        QObject::connect(dblFrequency, SIGNAL(valueChanged(double)), seaShaderWidget, SLOT(frequecyVal(double)));
        QObject::connect(IntIter_Geo, SIGNAL(valueChanged(int)), seaShaderWidget, SLOT(intIter_GeoVal(int)));
        QObject::connect(intIter_Frag, SIGNAL(valueChanged(int)), seaShaderWidget, SLOT(intIter_FragVal(int)));
        QObject::connect(btnBaseColor, SIGNAL(clicked()), seaShaderWidget, SLOT(baseColorClick()));
        QObject::connect(btnWaterColor, SIGNAL(clicked()), seaShaderWidget, SLOT(waterColorClick()));

        QMetaObject::connectSlotsByName(seaShaderWidget);
    } // setupUi

    void retranslateUi(QWidget *seaShaderWidget)
    {
        seaShaderWidget->setWindowTitle(QApplication::translate("seaShaderWidget", "seaShaderWidget", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("seaShaderWidget", "ShaderParam", Q_NULLPTR));
        btnBaseColor->setText(QString());
        lb1->setText(QApplication::translate("seaShaderWidget", "iter_geometry", Q_NULLPTR));
        lb2->setText(QApplication::translate("seaShaderWidget", "frequency", Q_NULLPTR));
        lb5->setText(QApplication::translate("seaShaderWidget", "iter_fragment", Q_NULLPTR));
        lb4->setText(QApplication::translate("seaShaderWidget", "height", Q_NULLPTR));
        lb3->setText(QApplication::translate("seaShaderWidget", "baseColor", Q_NULLPTR));
        lb6->setText(QApplication::translate("seaShaderWidget", "speed", Q_NULLPTR));
        lb7->setText(QApplication::translate("seaShaderWidget", "waterColor", Q_NULLPTR));
        btnWaterColor->setText(QString());
        lb8->setText(QApplication::translate("seaShaderWidget", "choppy", Q_NULLPTR));
        btnLoad->setText(QApplication::translate("seaShaderWidget", "Load", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class seaShaderWidget: public Ui_seaShaderWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEASHADERWIDGET_H
