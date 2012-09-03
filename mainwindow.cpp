#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <API/streaminformation.h>
#include <searchmusicwindow.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bridge = new AudioPlayerBridge();
    api = new API();
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblBtn1Caption);
    effect->setBlurRadius(1);
    effect->setOffset(-1, 2);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn1Caption->setGraphicsEffect(effect);
    effect = new QGraphicsDropShadowEffect(ui->lblBtn2Caption);
    effect->setBlurRadius(1);
    effect->setOffset(-1, 2);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn2Caption->setGraphicsEffect(effect);
    effect = new QGraphicsDropShadowEffect(ui->lblBtn3Caption);
    effect->setBlurRadius(1);
    effect->setOffset(-1, 2);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn3Caption->setGraphicsEffect(effect);
    ui->lblBtn1Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn1Message->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn2Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn2Message->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn3Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn3Message->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete bridge;
    delete api;
}

void MainWindow::on_btnSide2_clicked()
{
    api->getSessionID();
}

void MainWindow::on_btn2_clicked()
{
    SearchMusicWindow *smw = new SearchMusicWindow();
    smw->setAPI(api);
    smw->setAPB(bridge);
    connect(smw, SIGNAL(destroyed()), this, SLOT(onChildClosed()), Qt::DirectConnection);
    smw->show();
    this->setVisible(false);
}

void MainWindow::onChildClosed()
{
    this->setVisible(true);
}
