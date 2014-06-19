#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include "API/streaminformation.h"
#include "searchmusicwindow.h"
#include "playlistoptimizewindow.h"
#include "playmusicwindow.h"
#include <QEventLoop>
#include "splashscreen.h"
#include <QDesktopWidget>
#include <QRect>
#include <Qt>
#include "upnpdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    SplashScreen *ss = new SplashScreen();
    QRect geometry = QApplication::desktop()->screenGeometry();
    ss->setGeometry((geometry.width() - ss->width()) / 2, (geometry.height() - ss->height()) / 2, ss->width(), ss->height());
    ss->setFixedSize(ss->size());
    ss->setVisible(true);
    plh = new PlaylistHandler(this);
    bridge = new AudioPlayerBridge(this);
    coverHelper = new CoverHelper(this);
    api = new API(this);
    player = new Player(this, api, plh, bridge);
    setupUpnp();
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~(Qt::WindowFullscreenButtonHint));
    this->setGeometry((geometry.width() - this->width()) / 2, (geometry.height() - this->height()) / 2, this->width(), this->height());
    api->checkConnect();
    QEventLoop loop;
    connect(api, SIGNAL(firstConnected()), &loop, SLOT(quit()));
    loop.exec();
    ss->close();
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblBtn1Caption);
    effect->setBlurRadius(2);
    effect->setOffset(0, 1);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn1Caption->setGraphicsEffect(effect);
    effect = new QGraphicsDropShadowEffect(ui->lblBtn2Caption);
    effect->setBlurRadius(2);
    effect->setOffset(0, 1);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn2Caption->setGraphicsEffect(effect);
    effect = new QGraphicsDropShadowEffect(ui->lblBtn3Caption);
    effect->setBlurRadius(2);
    effect->setOffset(0, 1);
    effect->setColor(QColor("#BEBEBE"));
    ui->lblBtn3Caption->setGraphicsEffect(effect);
    ui->lblBtn1Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn1Message->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn2Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn2Message->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn3Caption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblBtn3Message->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setFixedSize(this->size());
    delete ss;
    /*playButtonShortcut = new QxtGlobalShortcut(QKeySequence(Qt::Key_MediaNext),this);
    connect(playButtonShortcut, SIGNAL(activated()), this, SLOT(onBtnPlay_pressed()));*/
}

void MainWindow::onBtnPlay_pressed() {
    player->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSide2_clicked()
{
    UpnpDialog *dialog = new UpnpDialog(this, upnpController, bridge, player);
    dialog->setModal(true);
    dialog->show();
}

void MainWindow::on_btn2_clicked()
{
    SearchMusicWindow *smw = new SearchMusicWindow();
    smw->setAPI(api);
    smw->setAPB(bridge);
    smw->setPLH(plh);
    smw->setMainWindow(this);
    smw->setPlayer(player);
    connect(smw, SIGNAL(destroyed()), this, SLOT(onChildClosed()));
    smw->show();
    this->setVisible(false);
}

void MainWindow::onChildClosed()
{
}

void MainWindow::on_btn3_clicked()
{
    PlaylistOptimizeWindow *pow = new PlaylistOptimizeWindow(0, coverHelper, player);
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

void MainWindow::setupUpnp() {
    NPT_LogManager::GetDefault().Configure("plist:.level=WARNING;.handlers=ConsoleHandler;.ConsoleHandler.colors=off;.ConsoleHandler.filter=24");
    upnp = new PLT_UPnP();
    PLT_CtrlPointReference ctrlPoint(new PLT_CtrlPoint());
    upnpController = new UpnpController(this, ctrlPoint, player, bridge);
    upnp->AddCtrlPoint(ctrlPoint);
    /*upnpServer = new UpnpServer(this, ctrlPoint);
    PLT_DeviceHostReference device(upnpServer);
    upnp->AddDevice(device);
    // ignore ourselves
    ctrlPoint->IgnoreUUID(device->GetUUID());*/
    upnp->Start();

    // extra discover for stupid renderers (like windows)
    ctrlPoint->Discover(NPT_HttpUrl("255.255.255.255", 1900, "*"), "upnp:rootdevice", 1);
}
