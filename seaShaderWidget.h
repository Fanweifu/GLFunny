#pragma once

#include <QWidget>
#include <QColorDialog>
#include <QPalette>
#include <QColor>
#include "Shape\Shader\SeaShader.h"
#include "ui_seaShaderWidget.h"
class seaShaderWidget : public QWidget
{
    Q_OBJECT

public:
    seaShaderWidget(QWidget *parent = Q_NULLPTR);
    ~seaShaderWidget();
protected:
    void setBtnColor(QPushButton* btn, QColor clr);

    void loadDefault();
private slots:
    void on_btnLoad_clicked();
    void intIter_GeoVal(int val) {
        seaShader.iter_geometry = val;
    }

    void IntIter_FragVal(int val) {
        seaShader.iter_fragment = val;
    }

    void heightVal(double val) {
        seaShader.height = (float)val;
    }

    void choopyVal(double val) {
        seaShader.choppy = (float)val;
    }

    void speedVal(double val) {
        seaShader.speed = (float)val;
    }

    void frequecyVal(double val) {
        seaShader.frequency = (float)val;
    }

    void baseColorClick();

    void waterColorClick();

private:
    Ui::seaShaderWidget ui;

    SeaShader seaShader;
    QColor baseColor;
    QColor waterColor;
};
