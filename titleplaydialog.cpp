#include "titleplaydialog.h"
#include "ui_titleplaydialog.h"
#include <QtConcurrent/QtConcurrentRun>
#include "myvolumestyle.h"
#include <QDesktopWidget>
#include <QRect>
#include <QMessageBox>

TitlePlayDialog::TitlePlayDialog(QWidget *parent, AudioPlayerBridge *apb, API *api, Song *song) :
    QDialog(parent),
    ui(new Ui::TitlePlayDialog)
{
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    posChanging = false;
    posTimer = new QTimer(this);
    posTimer->setInterval(200);
    posTimer->setSingleShot(false);
    connect(posTimer, SIGNAL(timeout()), this , SLOT(timerTick()));
    posTimer->start();
    this->apb = new AudioPlayerBridge(this, false);
    this->apb->setVolume(apb->getVolume());
    this->api = api;
    this->song = song;
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~(Qt::WindowFullscreenButtonHint));
    QRect geometry = QApplication::desktop()->screenGeometry();
    this->setGeometry((geometry.width() - this->width()) / 2, (geometry.height() - this->height()) / 2, this->width(), this->height());
    this->setFixedSize(this->size());
    this->setWindowTitle(QString::fromStdString(song->getSongName() + " - " + song->getArtistName()));
    ui->lblSong->setText(QString::fromStdString(song->getSongName() + " - " + song->getArtistName()));
    ui->sldVolume->setValue(this->apb->getVolume());
    ui->sldVolume->setStyle(new MyVolumeStyle);
    ui->sldPosition->setStyle(new MyVolumeStyle);
    connect(api, SIGNAL(streamKeyReady(StreamInformation*)), this, SLOT(gotSongInformation(StreamInformation*)));
    connect(this->apb, SIGNAL(startedPlaying()), this, SLOT(songStarted()));
    connect(this->apb, SIGNAL(songFinished()), this, SLOT(songFinished()));
    connect(api, SIGNAL(songError(int)), this, SLOT(songFailed(int)));
    play();
}

void TitlePlayDialog::songFailed(int songId) {
    if (songId == song->getSongId()) {
        QMessageBox::critical(this, this->windowTitle(), QString::fromStdString("The song \"" + song->getSongName() + "\" is faulty! Playback cancelled!"));
        delete this;
    }
}

void TitlePlayDialog::timerTick() {
    if (!posChanging && apb->getState() != AudioPlayerBridge::STARTING && apb->getState() != AudioPlayerBridge::STOPPED) {
        ui->sldPosition->setValue(apb->getPosition());
    }
}

void TitlePlayDialog::songStarted() {
    makePauseButton();
    ui->btnPlay->setEnabled(true);
    ui->btnStop->setEnabled(true);
    ui->sldPosition->setEnabled(true);
}

void TitlePlayDialog::songFinished() {
    makePlayButton();
    ui->btnPlay->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->sldPosition->setValue(0);
    ui->sldPosition->setEnabled(false);
}

void TitlePlayDialog::stop() {
    apb->stop();
    makePlayButton();
    ui->btnPlay->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->sldPosition->setValue(0);
    ui->sldPosition->setEnabled(false);
}

void TitlePlayDialog::pause() {
    if (apb->getState() == AudioPlayerBridge::PLAYING) {
        apb->togglePlayPause();
    }
    makePlayButton();
    ui->btnPlay->setEnabled(true);
    ui->btnStop->setEnabled(true);
    ui->sldPosition->setEnabled(true);
}

void TitlePlayDialog::resume() {
    if (apb->getState() == AudioPlayerBridge::PAUSED) {
        apb->togglePlayPause();
    }
    makePauseButton();
    ui->btnPlay->setEnabled(true);
    ui->btnStop->setEnabled(true);
    ui->sldPosition->setEnabled(true);
}

void TitlePlayDialog::gotSongInformation(StreamInformation *info) {
    if (info->getSongId() == song->getSongId()) {
        ui->sldPosition->setMaximum(info->getUSecs() / 1000);
        QtConcurrent::run(apb, &AudioPlayerBridge::openAndPlay, info->directUrl());
    }
}

void TitlePlayDialog::play() {
    makePlayButton();
    ui->btnPlay->setEnabled(false);
    ui->btnStop->setEnabled(false);
    ui->sldPosition->setValue(0);
    ui->sldPosition->setEnabled(false);
    api->getStreamKeyFromSongIDEx(song->getSongId());
}

TitlePlayDialog::~TitlePlayDialog()
{
    stop();
    delete ui;
}

void TitlePlayDialog::makePlayButton() {
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

void TitlePlayDialog::makePauseButton() {
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

void TitlePlayDialog::on_sldPosition_sliderPressed()
{
    posChanging = true;
}

void TitlePlayDialog::on_sldPosition_sliderReleased()
{
    on_sldPosition_sliderMoved(ui->sldPosition->value());
    posChanging = false;
}

void TitlePlayDialog::on_sldPosition_sliderMoved(int position)
{
    if (apb->getState() != AudioPlayerBridge::STOPPED) {
        apb->setPosition(position);
    }
}

void TitlePlayDialog::on_btnStop_clicked()
{
    stop();
}

void TitlePlayDialog::on_sldVolume_valueChanged(int value)
{
    apb->setVolume(value);
}

void TitlePlayDialog::on_btnPlay_clicked()
{
    if (apb->getState() == AudioPlayerBridge::STOPPED) {
        play();
    } else if (apb->getState() == AudioPlayerBridge::PLAYING) {
        pause();
    } else if (apb->getState() == AudioPlayerBridge::PAUSED) {
        resume();
    }
}

void TitlePlayDialog::on_btnClose_clicked()
{
    delete this;
}
