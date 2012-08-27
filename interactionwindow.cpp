#include "interactionwindow.h"
#include "ui_interactionwindow.h"
#include <QDebug>
#include <QtConcurrentRun>
#include "QVariantMap"
#include "API/streaminformation.h"

InteractionWindow::InteractionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InteractionWindow)
{
    ui->setupUi(this);
    api = new API(this);
    player = new AudioPlayerBridge(this);
}

InteractionWindow::~InteractionWindow()
{
    delete ui;
}

void InteractionWindow::on_pushButton_clicked()
{
    api->checkConnect();
    StreamInformation* streamInfo = api->getStreamKeyFromSongIDEx(34913847);
    player->openAndPlay(streamInfo->directUrl());
    delete streamInfo;
}
