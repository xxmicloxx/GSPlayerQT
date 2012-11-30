#ifndef AUDIOPLAYERBRIDGE_H
#define AUDIOPLAYERBRIDGE_H

#include <QObject>
#include "bass.h"

class AudioPlayerBridge : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerBridge(QObject *parent = 0, bool first = true);
    ~AudioPlayerBridge();
    void openAndPlay(std::string path);
    void stop();
    void togglePlayPause();
    void emit_songFinished();
    void setPosition(int pos);
    enum Status {STOPPED, PAUSED, PLAYING, STARTING};
    Status getState();
    void setState(Status status);
    Status currentStatus;
    int getPosition();
    void setVolume(int vol);
    int getVolume();
    
signals:
    void songFinished();
    void startedPlaying();

private:
    HSTREAM mainHandle;
    HSYNC finishedHandle;
    int lastVol;
    bool first;

};

#endif // AUDIOPLAYERBRIDGE_H
