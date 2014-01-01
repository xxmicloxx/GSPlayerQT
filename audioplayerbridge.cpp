#include "audioplayerbridge.h"
#include "boost/bind.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QDebug>

AudioPlayerBridge::AudioPlayerBridge(QObject *parent, bool first) :
    QObject(parent)
{
    if (first)
        BASS_Init(-1, 44100, 0, 0, NULL);
    useUpnp = false;
    this->first = first;
    currentStatus = STOPPED;
    lastVol = 100;
}

void AudioPlayerBridge::setUseUpnp(bool upnp) {
    if (upnp != useUpnp) {
        stop();
    }
    this->useUpnp = upnp;
}

bool AudioPlayerBridge::getUseUpnp() {
    return useUpnp;
}

void AudioPlayerBridge::emitStartedPlaying() {
    emit startedPlaying();
}

AudioPlayerBridge::~AudioPlayerBridge() {
    if (first)
        BASS_Free();
}

void CALLBACK MySyncProc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    ((AudioPlayerBridge*) user)->setState(AudioPlayerBridge::STOPPED);
    qDebug() << "song finished!";
    ((AudioPlayerBridge*) user)->emit_songFinished();
}

int AudioPlayerBridge::getPosition() {
    if (!useUpnp) {
        return BASS_ChannelBytes2Seconds(mainHandle, BASS_ChannelGetPosition(mainHandle, BASS_POS_BYTE)) * 1000;
    } else {
        return emit positionUpnpRequest();
    }
}

void AudioPlayerBridge::setPosition(int pos) {
    if (!useUpnp) {
        double timeInSecs = pos / 1000.0;
        BASS_ChannelSetPosition(mainHandle, BASS_ChannelSeconds2Bytes(mainHandle, timeInSecs), BASS_POS_BYTE);
    } else {
        emit setPositionOnUpnp(pos);
    }
}

void AudioPlayerBridge::openAndPlay(StreamInformation *si, Song *song) {
    stop();
    currentStatus = STARTING;
    if (!useUpnp) {
        mainHandle = BASS_StreamCreateURL(si->directUrl().c_str(), 0, BASS_STREAM_AUTOFREE | BASS_STREAM_PRESCAN, NULL, NULL);
        finishedHandle = BASS_ChannelSetSync(mainHandle, BASS_SYNC_END, 0, &MySyncProc, this);
        BASS_ChannelSetAttribute(mainHandle, BASS_ATTRIB_VOL, lastVol / 100.0);
        BASS_ChannelPlay(mainHandle, false);
        currentStatus = PLAYING;
        emit startedPlaying();
    } else {
        emit playOnUpnp(si, song);
    }
}

void AudioPlayerBridge::emit_songFinished() {
    emit songFinished();
}

void AudioPlayerBridge::setVolume(int vol) {
    if (!useUpnp) {
        BASS_ChannelSetAttribute(mainHandle, BASS_ATTRIB_VOL, vol / 100.0);
    } else {
        emit setVolumeOnUpnp(vol);
    }
    lastVol = vol;
}

int AudioPlayerBridge::getVolume() {
    if (!useUpnp) {
        if (currentStatus == STOPPED || currentStatus == STARTING) {
            return lastVol;
        }
        float value;
        BASS_ChannelGetAttribute(mainHandle, BASS_ATTRIB_VOL, &value);
        return value * 100;
    } else {
        // this works, it's a Qt Creator bug
        int v = emit volumeUpnpRequest();
        return v;
    }
}

void AudioPlayerBridge::stop() {
    if (currentStatus != STOPPED && currentStatus != STARTING) {
        if (!useUpnp) {
            BASS_ChannelStop(mainHandle);
            currentStatus = STOPPED;
        } else {
            emit stopOnUpnp();
        }
    }
}

void AudioPlayerBridge::setState(Status status) {
    currentStatus = status;
}

void AudioPlayerBridge::togglePlayPause() {
    if (currentStatus == PAUSED) {
        if (!useUpnp) {
            BASS_ChannelPlay(mainHandle, false);
            currentStatus = PLAYING;
        } else {
            emit resumeOnUpnp();
        }
    } else if (currentStatus == PLAYING) {
        if (!useUpnp) {
            BASS_ChannelPause(mainHandle);
            currentStatus = PAUSED;
        } else {
            emit pauseOnUpnp();
        }
    }
}

AudioPlayerBridge::Status AudioPlayerBridge::getState() {
    return currentStatus;
}
