#include "audioplayerbridge.h"
#include "bass.h"

AudioPlayerBridge::AudioPlayerBridge(QObject *parent) :
    QObject(parent)
{

}

void AudioPlayerBridge::openAndPlay(QString path) {
    BASS_Init(-1, 44100, 0, 0, NULL);
    HSTREAM lol = BASS_StreamCreateURL(path.toAscii().constData(), 0, BASS_STREAM_AUTOFREE | BASS_STREAM_PRESCAN, NULL, NULL);
    BASS_ChannelPlay(lol, false);
}
