#ifndef AUDIOPLAYERBRIDGE_H
#define AUDIOPLAYERBRIDGE_H

#include <QObject>
#include "bass.h"

class AudioPlayerBridge : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerBridge(QObject *parent = 0);
    void openAndPlay(std::string path);
    void stop();
    void togglePlayPause();
    void emit_songFinished();
    enum Status {STOPPED, PAUSED, PLAYING};
    Status getState();
    void setState(Status status);
    Status currentStatus;
    
signals:
    void songFinished();

public slots:

private:
    HSTREAM mainHandle;
    HSYNC finishedHandle;
    
};

#endif // AUDIOPLAYERBRIDGE_H
