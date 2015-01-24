#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    this->Login->setText("Your login");
    this->socket = new QUdpSocket(this);
    this->socket->bind(QHostAddress::Any, 3333, QUdpSocket::ReuseAddressHint);
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readDatagrams()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readDatagrams()
{
    while (this->socket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(this->socket->pendingDatagramSize());
        this->socket->readDatagram(data.data(), data.size());
        QString message = QString::fromUtf8(data).trimmed();
        QString cur_name = "";
        int i = 1;
        while(message[i]!='>') {
            cur_name += message[i];
            i++;
        }
        bool exist = false;
        for (int i = 0; i<this->Logins->count(); i++) {
            if (this->Logins->item(i)->text()==cur_name) {
                exist = true;
            }
        }
        if (!exist) {
            new QListWidgetItem(cur_name, this->Logins);
        }
        new QListWidgetItem(message, this->Messages);

    }
}

void MainWindow::on_addUser_clicked()
{
    if (this->name!="" || this->Login->text() == "") {
        return;
    }
    this->name = this->Login->text();
    this->Login->setReadOnly(true);
    this->writeMessage->setText("");
    QString firstMessage ="<" + this->name  +  "> has entered";
    QByteArray toSend;
    toSend.append(firstMessage);
    this->socket->writeDatagram(toSend.data(), toSend.size(), QHostAddress::Broadcast, 3333);

}

void MainWindow::on_sendMessage_clicked()
{
    if (this->writeMessage->toPlainText() == "" || this->name == "") {
        return;
    }
    QString message = "<"+this->name+">:"+this->writeMessage->toPlainText();
    QByteArray toSend;
    toSend.append(message);
    this->socket->writeDatagram(toSend.data(), toSend.size(), QHostAddress::Broadcast, 3333);
    this->writeMessage->setText("");
}
