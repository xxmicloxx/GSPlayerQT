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
    enum Status {STOPPED, PAUSED, PLAYING};
    Status currentStatus;
    
signals:
    
public slots:

private:
    HSTREAM mainHandle;
    
};

#endif // AUDIOPLAYERBRIDGE_H
