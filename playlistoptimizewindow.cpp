#include "playlistoptimizewindow.h"
#include "ui_playlistoptimizewindow.h"
#include <QGraphicsDropShadowEffect>

PlaylistOptimizeWindow::PlaylistOptimizeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlaylistOptimizeWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->size());

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblOrganizePlaylists);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblOrganizePlaylists->setGraphicsEffect(effect);
    effect = new QGraphicsDropShadowEffect(ui->lblChoosePlaylist);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblChoosePlaylist->setGraphicsEffect(effect);

    QLabel* transLabelArray[] = {ui->lblAddTitle, ui->lblBtnBack, ui->lblCreatePlaylist, ui->lblImport, ui->lblDeletePlaylist};
    int count = sizeof(transLabelArray) / sizeof(QLabel*); //because size of array is always the size of its contents
    for (int i = 0; i < count; i++) {
        transLabelArray[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

PlaylistOptimizeWindow::~PlaylistOptimizeWindow()
{
    delete ui;
}

void PlaylistOptimizeWindow::setAPI(API *api) {
    this->api = api;
}

void PlaylistOptimizeWindow::setPLH(PlaylistHandler *plh) {
    this->plh = plh;
    std::vector<std::string> playlistVector = plh->getPlaylists();
    for (unsigned int i = 0; i < playlistVector.size(); i++) {
        ui->cmbPlaylist->addItem(QString::fromStdString(playlistVector.at(i)));
    }
}
