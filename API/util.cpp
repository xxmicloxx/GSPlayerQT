#include "util.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QEventLoop>
#include <QVariant>

Util::Util(QObject *parent) :
    QObject(parent)
{

}

std::string Util::getMd5FromString(std::string md5string) {
    return QString(QCryptographicHash::hash(QString::fromAscii(md5string.c_str()).toAscii(), QCryptographicHash::Md5).toHex()).toStdString();
}

std::string Util::getSha1FromString(std::string sha1string) {
    return QString(QCryptographicHash::hash(QString::fromAscii(sha1string.c_str()).toAscii(), QCryptographicHash::Sha1).toHex()).toStdString();
}

std::string Util::postData(std::string url, std::string data) {
}
