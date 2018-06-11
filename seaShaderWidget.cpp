#include "seaShaderWidget.h"

seaShaderWidget::seaShaderWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

seaShaderWidget::~seaShaderWidget()
{
    
}

void seaShaderWidget::setBtnColor(QPushButton * btn, QColor clr)
{
    QString s;
    s.sprintf("background-color:rgb(%d,%d,%d);",clr.red(),clr.green(),clr.blue());
    btn->setStyleSheet(s);
}

void vec3TOColor(glm::vec3 p, QColor &clr) {
    clr.setRgb(p.r * 255, p.g * 255, p.b * 255);
}
glm::vec3 ColorTovec3(QColor &clr) {
    return glm::vec3((float)clr.red() / 255, (float)clr.green() / 255, (float)clr.blue() / 255);
}

void seaShaderWidget::baseColorClick()
{
    QColor clr = QColorDialog::getColor(baseColor, this);
    if (clr.isValid()) {

        baseColor = clr;
        seaShader.baseColor = ColorTovec3(baseColor);
        setBtnColor(ui.btnBaseColor, baseColor);
    }
}

void seaShaderWidget::waterColorClick()
{
    QColor clr = QColorDialog::getColor(waterColor, this);
    if (clr.isValid()) {

        waterColor = clr;
        seaShader.waterColor = ColorTovec3(waterColor);
        setBtnColor(ui.btnWaterColor, waterColor);
    }
}



void seaShaderWidget::loadDefault()
{
    ui.dblHeight->setValue(seaShader.height);
    ui.dblchoppy->setValue(seaShader.choppy);
    ui.dblSpeed->setValue(seaShader.speed);
    ui.dblFrequency->setValue(seaShader.frequency);
    ui.intIter_Frag->setValue(seaShader.iter_fragment);
    ui.IntIter_Geo->setValue(seaShader.iter_geometry);

    vec3TOColor(seaShader.baseColor,baseColor);
    setBtnColor(ui.btnBaseColor, baseColor);

    vec3TOColor(seaShader.waterColor, waterColor);
    setBtnColor(ui.btnWaterColor, waterColor);
}

void seaShaderWidget::on_btnLoad_clicked()
{
    loadDefault();
    seaShader.load();
    ui.openGLWidget->BindShader(seaShader);
}
