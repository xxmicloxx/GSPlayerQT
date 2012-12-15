#ifndef TITLEPLAYDIALOG_H
#define TITLEPLAYDIALOG_H

#include "audioplayerbridge.h"
#include "API/api.h"
#include <QDialog>
#include "API/streaminformation.h"
#include <QTimer>

namespace Ui {
class TitlePlayDialog;
}

class TitlePlayDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TitlePlayDialog(QWidget *parent = 0, AudioPlayerBridge *apb = 0, API *api = 0, Song *song = 0);
    ~TitlePlayDialog();
    void makePlayButton();
    void makePauseButton();
    void play();

public slots:
    void gotSongInformation(StreamInformation *info);
    void songStarted();
    void songFinished();
    void stop();
    void pause();
    void resume();
    void timerTick();
    void songFailed(int songId);
    
private slots:
    void on_sldPosition_sliderPressed();

    void on_sldPosition_sliderReleased();

    void on_sldPosition_sliderMoved(int position);

    void on_btnStop_clicked();

    void on_sldVolume_valueChanged(int value);

    void on_btnPlay_clicked();

private:
    Ui::TitlePlayDialog *ui;
    AudioPlayerBridge *apb;
    API *api;
    Song *song;
    QTimer *posTimer;
    bool posChanging;
};

#endif // TITLEPLAYDIALOG_H
