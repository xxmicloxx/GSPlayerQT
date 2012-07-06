#include "interactionwindow.h"
#include "ui_interactionwindow.h"
#include <QDebug>
#include "audioplayerbridge.h"
#include <QtConcurrentRun>
#include "QVariantMap"

InteractionWindow::InteractionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InteractionWindow)
{
    ui->setupUi(this);
    api = new API(this);
    connected = false;
}

InteractionWindow::~InteractionWindow()
{
    delete ui;
}

void InteractionWindow::on_pushButton_clicked()
{
    checkConnect();

}

void InteractionWindow::init() {
    sessionID = api->getSessionID();
    token = api->getCommunicationToken(sessionID);
}

void InteractionWindow::checkConnect() {
    if (!connected) {
        connected = true;
        init();
    }
}
