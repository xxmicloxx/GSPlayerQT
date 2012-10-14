#ifndef AUDIOPLAYERBRIDGE_H
#define AUDIOPLAYERBRIDGE_H

#include <QObject>
#include "bass.h"

class AudioPlayerBridge : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerBridge(QObject *parent = 0);
    ~AudioPlayerBridge();
    void openAndPlay(std::string path);
    void stop();
    void togglePlayPause();
    void emit_songFinished();
    void setPosition(int pos);
    enum Status {STOPPED, PAUSED, PLAYING};
    Status getState();
    void setState(Status status);
    Status currentStatus;
    int getPosition();
    
signals:
    void songFinished();

public slots:

private:
    HSTREAM mainHandle;
    HSYNC finishedHandle;
    
};

#endif // AUDIOPLAYERBRIDGE_H
