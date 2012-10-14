#include "playmusicwindow.h"
#include "ui_playmusicwindow.h"
#include <QGraphicsDropShadowEffect>

PlayMusicWindow::PlayMusicWindow(QWidget *parent, PlaylistHandler *plh, API *api, CoverHelper *coverHelper, QMainWindow* mainWindow, Player *player) :
    QMainWindow(parent),
    ui(new Ui::PlayMusicWindow)
{
    posSliderMoving = false;
    this->realMainWindow = mainWindow;
    this->plh = plh;
    this->api = api;
    this->player = player;
    this->apb = new AudioPlayerBridge(realMainWindow);
    this->coverHelper = coverHelper;
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->size());
    playlistsRefreshing = false;
    QWidget* shadowArray[] = {ui->lblPlayMusic, ui->lblPlayedPlaylist};
    int count = sizeof(shadowArray) / sizeof(QWidget*);
    for (int i = 0; i < count; i++) {
        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(shadowArray[i]);
        effect->setBlurRadius(1);
        effect->setColor(QColor("#bb6008"));
        effect->setOffset(0, 1);
        shadowArray[i]->setGraphicsEffect(effect);
    }

    QWidget* clickthruArray[] = {};
    count = sizeof(clickthruArray) / sizeof(QWidget*);
    for (int i = 0; i < count; i++) {
        clickthruArray[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
    connect(player, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(playlistsChanged(std::vector<std::string>)));
    connect(player, SIGNAL(currentSongChanged()), this, SLOT(songsChanged()));
    connect(player, SIGNAL(songPositionChanged()), this, SLOT(onPositionChanged()));
    connect(coverHelper, SIGNAL(coverGotten(std::string)), this, SLOT(gotCover(std::string)));
    playlistsChanged(plh->getPlaylists());
    songsChanged();
}

void PlayMusicWindow::gotCover(std::string path) {
    Song* currentSong = player->getCurrentSong();
    if (currentSong != NULL && currentSong->getCoverArtFilename() == path) {
        ui->lblCurrentCover->setPixmap(QPixmap(QString::fromStdString("covers/" + path)));
    }
    Song* songBefore = player->getSongBefore();
    if (songBefore != NULL && songBefore->getCoverArtFilename() == path) {
        ui->lblBeforeCover->setPixmap(QPixmap(QString::fromStdString("covers/" + path)));
    }
    Song* songAfter = player->getSongAfter();
    if (songAfter != NULL && songAfter->getCoverArtFilename() == path) {
        ui->lblAfterCover->setPixmap(QPixmap(QString::fromStdString("covers/" + path)));
    }
}

void PlayMusicWindow::playlistsChanged(std::vector<std::string> playlists) {
    playlistsRefreshing = true;
    ui->cmbPlaylists->clear();
    for (unsigned int i = 0; i < playlists.size(); i++) {
        ui->cmbPlaylists->addItem(QString::fromStdString(playlists.at(i)));
    }
    std::string searchString = player->getPlaylist();
    for (int i = 0; i < ui->cmbPlaylists->count(); i++) {
        if (ui->cmbPlaylists->itemText(i).toStdString() == searchString) {
            ui->cmbPlaylists->setCurrentIndex(i);
        }
    }
    playlistsRefreshing = false;
}

void PlayMusicWindow::onPositionChanged() {
    if (posSliderMoving)
        return;
    ui->sldPosition->setMaximum(player->getLength());
    ui->sldPosition->setValue(player->getSongPosition());
}

void PlayMusicWindow::songsChanged() {
    Song* currentSong = player->getCurrentSong();
    if (currentSong != NULL) {
        ui->lblCurrentCoverBackground->setVisible(true);
        ui->lblCurrentCover->setVisible(true);
        coverHelper->getPathForCover(currentSong->getCoverArtFilename());
        ui->lblCurrentSong->setText(QString::fromStdString(currentSong->getArtistName() + " - " + currentSong->getSongName()));
    } else {
        ui->lblCurrentCoverBackground->setVisible(false);
        ui->lblCurrentCover->setVisible(false);
        ui->lblCurrentSong->setText("Kein Song abgespielt");
    }
    Song* songBefore = player->getSongBefore();
    if (songBefore != NULL) {
        ui->lblCoverBeforeBackground->setVisible(true);
        ui->lblBeforeCover->setVisible(true);
        coverHelper->getPathForCover(songBefore->getCoverArtFilename());
    } else {
        ui->lblCoverBeforeBackground->setVisible(false);
        ui->lblBeforeCover->setVisible(false);
    }
    Song* songAfter = player->getSongAfter();
    if (songAfter != NULL) {
        ui->lblCoverAfterBackground->setVisible(true);
        ui->lblAfterCover->setVisible(true);
        coverHelper->getPathForCover(songAfter->getCoverArtFilename());
    } else {
        ui->lblCoverAfterBackground->setVisible(false);
        ui->lblAfterCover->setVisible(false);
    }
}

PlayMusicWindow::~PlayMusicWindow()
{
    realMainWindow->setVisible(true);
    realMainWindow->activateWindow();
    realMainWindow->raise();
    delete ui;
}

void PlayMusicWindow::on_cmbPlaylists_currentIndexChanged(int index)
{
    if (playlistsRefreshing)
        return;
    std::string playlistName = ui->cmbPlaylists->itemText(index).toStdString();
    player->setPlaylist(playlistName);
}

void PlayMusicWindow::on_sldPosition_sliderPressed()
{
    posSliderMoving = true;
}

void PlayMusicWindow::on_sldPosition_sliderReleased()
{
    posSliderMoving = false;
}

void PlayMusicWindow::on_sldPosition_sliderMoved(int position)
{
    player->setPosition(position);
}
