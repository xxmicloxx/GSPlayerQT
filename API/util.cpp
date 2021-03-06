#define BOOST_NETWORK_ENABLE_HTTPS
#include "util.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QEventLoop>
#include <QVariant>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

Util::Util(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager();
    currentPostActionId = -1;
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(dataReceived(QNetworkReply*)));
}

Util::~Util() {
    delete manager;
}

std::string Util::getMd5FromString(std::string md5string) {
    return QString(QCryptographicHash::hash(QString::fromAscii(md5string.c_str()).toAscii(), QCryptographicHash::Md5).toHex()).toStdString();
}

std::string Util::getSha1FromString(std::string sha1string) {
    return QString(QCryptographicHash::hash(QString::fromAscii(sha1string.c_str()).toAscii(), QCryptographicHash::Sha1).toHex()).toStdString();
}

void Util::postData(std::string url, std::string data, int postActionId) {
    mutex.lock();
    currentPostActionId = postActionId;
    QUrl urlObj;
    urlObj.setUrl(QString::fromStdString(url));
    QNetworkRequest request;
    request.setUrl(urlObj);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11");
    manager->post(request, QString::fromStdString(data).toUtf8());
}

void Util::dataReceived(QNetworkReply *reply) {
    reply->deleteLater();
    QByteArray array = reply->readAll();
    reply->close();
    int clonedId = currentPostActionId;
    currentPostActionId = -1;
    mutex.unlock();
    emit dataPosted(clonedId, QString(array).toStdString());
}
