#include "audioplayerbridge.h"
#include "boost/bind.hpp"
#include <QtConcurrentRun>
#include <QDebug>

AudioPlayerBridge::AudioPlayerBridge(QObject *parent) :
    QObject(parent)
{
    BASS_Init(-1, 44100, 0, 0, NULL);
    currentStatus = STOPPED;
}

AudioPlayerBridge::~AudioPlayerBridge() {
    BASS_Free();
}

void CALLBACK MySyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    ((AudioPlayerBridge*) user)->setState(AudioPlayerBridge::STOPPED);
    qDebug() << "song finished!";
    ((AudioPlayerBridge*) user)->emit_songFinished();
}

int AudioPlayerBridge::getPosition() {
    return BASS_ChannelBytes2Seconds(mainHandle, BASS_ChannelGetPosition(mainHandle, BASS_POS_BYTE)) * 1000;
}

void AudioPlayerBridge::setPosition(int pos) {
    double timeInSecs = pos / 1000.0;
    BASS_ChannelSetPosition(mainHandle, BASS_ChannelSeconds2Bytes(mainHandle, timeInSecs), BASS_POS_BYTE);
}

void AudioPlayerBridge::openAndPlay(std::string path) {
    stop();
    mainHandle = BASS_StreamCreateURL(path.c_str(), 0, BASS_STREAM_AUTOFREE | BASS_STREAM_PRESCAN, NULL, NULL);
    finishedHandle = BASS_ChannelSetSync(mainHandle, BASS_SYNC_END, 0, &MySyncProc, this);
    currentStatus = PLAYING;
    BASS_ChannelPlay(mainHandle, false);
}

void AudioPlayerBridge::emit_songFinished() {
    emit songFinished();
}

void AudioPlayerBridge::stop() {
    if (currentStatus != STOPPED) {
        BASS_ChannelStop(mainHandle);
        currentStatus = STOPPED;
    }
}

void AudioPlayerBridge::setState(Status status) {
    currentStatus = status;
}

void AudioPlayerBridge::togglePlayPause() {
    if (currentStatus == PAUSED) {
        BASS_ChannelPlay(mainHandle, false);
        currentStatus = PLAYING;
    } else if (currentStatus == PLAYING) {
        BASS_ChannelPause(mainHandle);
        currentStatus = PAUSED;
    }
}

AudioPlayerBridge::Status AudioPlayerBridge::getState() {
    return currentStatus;
}
