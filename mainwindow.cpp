#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <API/streaminformation.h>
#include <searchmusicwindow.h>
#include "playlistoptimizewindow.h"
#include "playmusicwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plh = new PlaylistHandler(this);
    bridge = new AudioPlayerBridge(this);
    coverHelper = new CoverHelper(this);
    api = new API(this);
    player = new Player(this, api, plh, bridge);
    api->checkConnect();
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
}

void MainWindow::on_btnSide2_clicked()
{

}

void MainWindow::on_btn2_clicked()
{
    SearchMusicWindow *smw = new SearchMusicWindow();
    smw->setAPI(api);
    smw->setAPB(bridge);
    smw->setPLH(plh);
    smw->setMainWindow(this);
    connect(smw, SIGNAL(destroyed()), this, SLOT(onChildClosed()));
    smw->show();
    this->setVisible(false);
}

void MainWindow::onChildClosed()
{
}

void MainWindow::on_btn3_clicked()
{
    PlaylistOptimizeWindow *pow = new PlaylistOptimizeWindow(0, coverHelper);
    pow->setAPI(api);
    pow->setPLH(plh);
    pow->setMainWindow(this);
    connect(pow, SIGNAL(destroyed()), this, SLOT(onChildClosed()));
    pow->show();
    this->setVisible(false);
}

void MainWindow::on_btn1_clicked()
{
    PlayMusicWindow *pmw = new PlayMusicWindow(0, plh, api, coverHelper, this, player);
    connect(pmw, SIGNAL(destroyed()), this, SLOT(onChildClosed()));
    pmw->show();
    this->setVisible(false);
}
