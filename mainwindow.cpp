#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui/openglwidget.h"

#include<QFileDialog>
#include<QDockWidget>
#include<QMessageBox>
#include<QString>
using namespace std;
using namespace Qt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* p = takeCentralWidget();
    if(p) delete p;

    setDockNestingEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){

}

void MainWindow::on_actionImage_triggered()
{
    QString path = QFileDialog::getOpenFileName(this , tr("open img") , tr(" "),  tr("img(*.jpg)"));
    std::string pathstr = string((const char*)path.toLocal8Bit());
    if(pathstr.empty()) return;


    OpenglWidget *wg = new OpenglWidget();
    if(!wg->loadImg(pathstr)) QMessageBox::warning(this ,tr("error"),tr("load failed! "), QMessageBox::Accepted,QMessageBox::Accepted);

    QDockWidget *dock = new QDockWidget(path, this );
    dock->setFeatures( QDockWidget::AllDockWidgetFeatures);


    dock->setWidget( wg );

    int cnt = m_docks.count();
    if(cnt==0) addDockWidget(LeftDockWidgetArea, dock);
    else tabifyDockWidget(m_docks[cnt-1],dock);

    m_docks.append(dock);

}
