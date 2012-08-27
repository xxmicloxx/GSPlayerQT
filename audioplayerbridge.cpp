#include "audioplayerbridge.h"

AudioPlayerBridge::AudioPlayerBridge(QObject *parent) :
    QObject(parent)
{
    BASS_Init(-1, 44100, 0, 0, NULL);
}

void AudioPlayerBridge::openAndPlay(std::string path) {
    stop();
    mainHandle = BASS_StreamCreateURL(path.c_str(), 0, BASS_STREAM_AUTOFREE | BASS_STREAM_PRESCAN, NULL, NULL);
    BASS_ChannelPlay(mainHandle, false);
    currentStatus = PLAYING;
}

void AudioPlayerBridge::stop() {
    if (currentStatus != STOPPED) {
        BASS_ChannelStop(mainHandle);
    }
}
