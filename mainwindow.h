#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionImage_triggered();

    void on_actionSea_s_triggered();

private:
    Ui::MainWindow *ui;
    QList<QDockWidget*> m_docks;
    void init();

};

#endif // MAINWINDOW_H
