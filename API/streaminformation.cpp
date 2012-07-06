#include "streaminformation.h"
#define QT_USE_FAST_OPERATOR_PLUS

StreamInformation::StreamInformation(QObject *parent) :
    QObject(parent)
{
}

QString StreamInformation::directUrl() {
    return "http://" + ip + "/stream.php?streamKey=" + streamKey;
}

QString StreamInformation::uSecs() {
    return uSecs;
}

QString StreamInformation::streamKey() {
    return streamKey;
}

QString StreamInformation::ip() {
    return ip;
}

void StreamInformation::setUSecs(QString set) {
    uSecs = set;
}

void StreamInformation::setStreamKey(QString set) {
    streamKey = set;
}

void StreamInformation::setIp(QString set) {
    ip = set;
}


