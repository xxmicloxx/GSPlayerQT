#include "playmusicwindow.h"
#include "ui_playmusicwindow.h"
#include <QGraphicsDropShadowEffect>
#include "boost/lexical_cast.hpp"
#include <QDebug>
#include <myvolumestyle.h>
#include <QDesktopWidget>
#include <QRect>
#include <QLayout>
#include <QtConcurrent/QtConcurrentRun>

PlayMusicWindow::PlayMusicWindow(QWidget *parent, PlaylistHandler *plh, API *api, CoverHelper *coverHelper, QMainWindow* mainWindow, Player *player) :
    QMainWindow(parent),
    ui(new Ui::PlayMusicWindow)
{
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(true);
    currCurrentCover = "";
    nextCurrentCover = "";
    prevCurrentCover = "";
    connect(timer, SIGNAL(timeout()), this, SLOT(timerDone()));
    posSliderMoving = false;
    this->realMainWindow = mainWindow;
    this->plh = plh;
    this->api = api;
    this->player = player;
    this->apb = new AudioPlayerBridge(realMainWindow);
    this->coverHelper = coverHelper;
    int volume = player->getVolume();
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~(Qt::WindowFullscreenButtonHint));
    messageHandler = new MessageHandler(this);
    QRect geometry = QApplication::desktop()->screenGeometry();
    this->setGeometry((geometry.width() - this->width()) / 2, (geometry.height() - this->height()) / 2, this->width(), this->height());
    ui->sldVolume->setStyle(new MyVolumeStyle);
    ui->sldVolume->setValue(volume);
    ui->sldPosition->setStyle(new MyVolumeStyle);
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
    connect(player, SIGNAL(stateChanged()), this, SLOT(refreshPlayPause()));
    connect(coverHelper, SIGNAL(coverGotten(std::string)), this, SLOT(gotCover(std::string)), Qt::DirectConnection);
    connect(messageHandler, SIGNAL(addedMessage(Message*)), this, SLOT(addedMessage(Message*)));
    connect(messageHandler, SIGNAL(removedMessage(Message*)), this, SLOT(deletedMessage(Message*)));
    connect(player, SIGNAL(songFailed()), this, SLOT(songFailed()));
    playlistsChanged(plh->getPlaylists());
    songsChanged();
    refreshPlayPause();
    wasPlaying = player->isPlaying() || player->isPaused();
}

void PlayMusicWindow::songFailed() {
    messageHandler->addMessage("The song \"" + player->getCurrentSong()->getSongName() + "\" is faulty and was skipped!");
}

void PlayMusicWindow::deletedMessage(Message *message) {
    this->layout()->removeWidget(message);
}

void PlayMusicWindow::addedMessage(Message *message) {
    this->layout()->addWidget(message);
}

void PlayMusicWindow::refreshPlayPause() {
    if (player->isPlaying()) {
        enableAll();
        songsChanged();
        makePauseButton();
        on_sldVolume_valueChanged(ui->sldVolume->value());
        ui->lblCurrentState->setText("Playing");
        wasPlaying = true;
    } else if (player->isPaused()) {
        enableAll();
        songsChanged();
        makePlayButton();
        wasPlaying = true;
        ui->lblCurrentState->setText("Paused");
    } else if (player->isStarting()) {
        disableControls();
        disablePlay();
        ui->lblCurrentState->setText("Starting...");
    } else if (player->isStopped()) {
        makePlayButton();
        ui->lblCurrentState->setText("Stopped");
        wasPlaying = false;
    }
}

void PlayMusicWindow::makePlayButton() {
    ui->btnPlay->setStyleSheet("#btnPlay {"
                               "border: none;"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Play_Button_normal.png);"
                               "}"
                               ""
                               "#btnPlay:disabled {"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Play_Button_disabled.png);"
                               "}"
                               ""
                               "#btnPlay:hover {"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Play_Button_hover.png);"
                               "}"
                               ""
                               "#btnPlay:pressed {"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Play_Button_pressed.png);"
                               "}");
}

void PlayMusicWindow::makePauseButton() {
    ui->btnPlay->setStyleSheet("#btnPlay {"
                               "border: none;"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Stop_Button_normal.png);"
                               "}"
                               ""
                               "#btnPlay:disabled {"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Stop_Button_disabled.png);"
                               "}"
                               ""
                               "#btnPlay:hover {"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Stop_Button_hover.png);"
                               "}"
                               ""
                               "#btnPlay:pressed {"
                               "background-image: url(:/theme/play/RES/theme/Musik_abspielen/Stop_Button_pressed.png);"
                               "}");
}

void PlayMusicWindow::disablePlay() {
    ui->btnPlay->setEnabled(false);
}

void PlayMusicWindow::enablePlay() {
    ui->btnPlay->setEnabled(true);
}

void PlayMusicWindow::gotCover(std::string path) {
    Song* currentSong = player->getCurrentSong();
    if (currentSong != NULL && currentSong->getCoverArtFilename() == path && currCurrentCover != currentSong->getCoverArtFilename()) {
        ui->lblCurrentCover->setPixmap(QPixmap(QString::fromStdString("covers/" + path)));
        currCurrentCover = currentSong->getCoverArtFilename();
        return;
    }
    Song* songBefore = player->getSongBefore();
    if (songBefore != NULL && songBefore->getCoverArtFilename() == path && prevCurrentCover != songBefore->getCoverArtFilename()) {
        ui->lblBeforeCover->setPixmap(QPixmap(QString::fromStdString("covers/" + path)));
        prevCurrentCover = songBefore->getCoverArtFilename();
        return;
    }
    Song* songAfter = player->getSongAfter();
    if (songAfter != NULL && songAfter->getCoverArtFilename() == path && nextCurrentCover != songAfter->getCoverArtFilename()) {
        ui->lblAfterCover->setPixmap(QPixmap(QString::fromStdString("covers/" + path)));
        nextCurrentCover = songAfter->getCoverArtFilename();
        return;
    }
}

void PlayMusicWindow::timerDone() {
    player->play();
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
    if (searchString == "") {
        disablePlay();
    } else {
        enablePlay();
    }
    playlistsRefreshing = false;
}

void PlayMusicWindow::onPositionChanged() {
    QString seconds = QString::fromStdString(boost::lexical_cast<std::string>(player->getSongPosition() / 1000 % 60));
    QString minutes = QString::fromStdString(boost::lexical_cast<std::string>(player->getSongPosition() / 1000 / 60));
    while (seconds.count() < 2) {
        seconds = "0" + seconds;
    }
    QString songPos = minutes + ":" + seconds;
    seconds = QString::fromStdString(boost::lexical_cast<std::string>(player->getLength() / 1000 % 60));
    minutes = QString::fromStdString(boost::lexical_cast<std::string>(player->getLength() / 1000 / 60));
    while (seconds.count() < 2) {
        seconds = "0" + seconds;
    }
    QString length = minutes + ":" + seconds;
    ui->lblTime->setText(songPos + " / " + length);
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
        if (currentSong->getCoverArtFilename() != currentCoverFile) {
            ui->lblCurrentCover->setPixmap(QPixmap(":/theme/gen/RES/loader.png"));
            coverHelper->getPathForCover(currentSong->getCoverArtFilename());
            currentCoverFile = currentSong->getCoverArtFilename();
        }
        if (!player->isStopped()) {
            enableAll();
            wasPlaying = true;
        } else {
            makePlayButton();
            enablePlay();
            disableControls();
        }
        ui->lblCurrentSong->setText(QString::fromStdString(currentSong->getArtistName() + " - " + currentSong->getSongName()));
    } else {
        ui->lblCurrentCoverBackground->setVisible(false);
        ui->lblCurrentCover->setVisible(false);
        ui->lblCurrentSong->setText("No song playing");
        makePlayButton();
        enablePlay();
        disableControls();
    }
    Song* songBefore = player->getSongBefore();
    if (songBefore != NULL) {
        ui->lblCoverBeforeBackground->setVisible(true);
        ui->lblBeforeCover->setVisible(true);
        if (songBefore->getCoverArtFilename() != prevCoverFile) {
            ui->lblBeforeCover->setPixmap(QPixmap(":/theme/gen/RES/loader.png"));
            coverHelper->getPathForCover(songBefore->getCoverArtFilename());
            prevCoverFile = songBefore->getCoverArtFilename();
        }
        ui->btnPrev->setEnabled(true);
    } else {
        ui->lblCoverBeforeBackground->setVisible(false);
        ui->lblBeforeCover->setVisible(false);
        ui->btnPrev->setEnabled(false);
    }
    Song* songAfter = player->getSongAfter();
    if (songAfter != NULL) {
        ui->lblCoverAfterBackground->setVisible(true);
        ui->lblAfterCover->setVisible(true);
        if (songAfter->getCoverArtFilename() != nextCoverFile) {
            ui->lblAfterCover->setPixmap(QPixmap(":/theme/gen/RES/loader.png"));
            coverHelper->getPathForCover(songAfter->getCoverArtFilename());
            nextCoverFile = songAfter->getCoverArtFilename();
        }
        ui->btnNext->setEnabled(true);
    } else {
        ui->lblCoverAfterBackground->setVisible(false);
        ui->lblAfterCover->setVisible(false);
        ui->btnNext->setEnabled(false);
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
    on_sldPosition_sliderMoved(ui->sldPosition->value());
    posSliderMoving = false;
}

void PlayMusicWindow::on_sldPosition_sliderMoved(int position)
{
    player->setPosition(position);
}

void PlayMusicWindow::enableAll() {
    ui->btnNext->setEnabled(true);
    ui->btnPrev->setEnabled(true);
    ui->btnStop->setEnabled(true);
    enablePlay();
}

void PlayMusicWindow::disableControls() {
    ui->btnNext->setEnabled(false);
    ui->btnPrev->setEnabled(false);
    ui->btnStop->setEnabled(false);
}

void PlayMusicWindow::on_btnPlay_clicked()
{
    if (!player->isPlaying()) {
        player->play();
    } else if (player->isPlaying()) {
        player->pause();
    }
}

void PlayMusicWindow::on_btnStop_clicked()
{
    player->stopNoReset();
    wasPlaying = false;
}

void PlayMusicWindow::on_btnNext_clicked()
{
    bool wasPlaying = player->isPlaying() || player->isPaused() || wasPlaying;
    player->nextNoPlay();
    this->wasPlaying = wasPlaying;
    if (this->wasPlaying)
        timer->start();
}

void PlayMusicWindow::on_btnPrev_clicked()
{
    bool wasPlaying = player->isPlaying() || player->isPaused() || wasPlaying;
    player->prevNoPlay();
    this->wasPlaying = wasPlaying;
    if (this->wasPlaying)
        timer->start();
}

void PlayMusicWindow::on_sldVolume_valueChanged(int value)
{
    player->setVolume(value);
}
