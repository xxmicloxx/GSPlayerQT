#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <API/streaminformation.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bridge = new AudioPlayerBridge(this);
    api = new API(this);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblBtn1Caption);
    effect->setBlurRadius(1);
    effect->setOffset(-1, 2);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn1Caption->setGraphicsEffect(effect);
    ui->lblBtn1Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn1Message->setAttribute(Qt::WA_TransparentForMouseEvents);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSide2_clicked()
{
    StreamInformation *streamInfo = api->getStreamKeyFromSongIDEx("35971172");
    bridge->openAndPlay(streamInfo->directUrl());
    delete streamInfo;
}
