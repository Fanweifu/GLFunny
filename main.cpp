#include "mainwindow.h"
#include "openglwidget.h"
#include<QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpenglWidget w(0,false);
    QString path = QFileDialog::getOpenFileName(Q_NULLPTR, QString("open img") , QString(" "),  QString("img(*.jpg)"));
    std::string pathstr = std::string((const char*)path.toLocal8Bit());
    if(!w.loadImg(pathstr)) QMessageBox::warning(Q_NULLPTR ,QString("error"),QString("load fail! "), QMessageBox::Accepted,QMessageBox::Accepted);

    w.show();

    return a.exec();
}
