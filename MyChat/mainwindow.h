#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextLayout>
#include <QUdpSocket>
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void readDatagrams();
    void on_addUser_clicked();
    void on_sendMessage_clicked();

private:
    Ui::MainWindow *ui;
private:
        QString name;
        QUdpSocket *socket;
};

#endif // MAINWINDOW_H
