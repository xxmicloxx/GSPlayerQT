#ifndef AUDIOPLAYERBRIDGE_H
#define AUDIOPLAYERBRIDGE_H

#include <QObject>
#include "bass.h"
#include "API/song.h"
#include "API/streaminformation.h"

class AudioPlayerBridge : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayerBridge(QObject *parent = 0, bool first = true);
    ~AudioPlayerBridge();
    void openAndPlay(StreamInformation *si, Song *song);
    void stop();
    void togglePlayPause();
    void emit_songFinished();
    void setPosition(int pos);
    enum Status {STOPPED, PAUSED, PLAYING, STARTING, BUFFERING};
    Status getState();
    void setState(Status status);
    Status currentStatus;
    int getPosition();
    void setVolume(int vol);
    int getVolume();
    void setUseUpnp(bool upnp);
    bool getUseUpnp();
    void emitStartedPlaying();
    
signals:
    void songFinished();
    void startedPlaying();
    void playOnUpnp(StreamInformation *si, Song *song);
    void stopOnUpnp();
    void pauseOnUpnp();
    void resumeOnUpnp();
    int volumeUpnpRequest();
    void setVolumeOnUpnp(int volume);
    int positionUpnpRequest();
    void setPositionOnUpnp(int position);

private:
    HSTREAM mainHandle;
    HSYNC finishedHandle;
    int lastVol;
    bool first;
    bool useUpnp;

};

#endif // AUDIOPLAYERBRIDGE_H
