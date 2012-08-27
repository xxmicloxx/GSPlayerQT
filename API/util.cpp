#define BOOST_NETWORK_ENABLE_HTTPS
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
    manager = new QNetworkAccessManager(this);
}

std::string Util::getMd5FromString(std::string md5string) {
    return QString(QCryptographicHash::hash(QString::fromAscii(md5string.c_str()).toAscii(), QCryptographicHash::Md5).toHex()).toStdString();
}

std::string Util::getSha1FromString(std::string sha1string) {
    return QString(QCryptographicHash::hash(QString::fromAscii(sha1string.c_str()).toAscii(), QCryptographicHash::Sha1).toHex()).toStdString();
}

std::string Util::postData(std::string url, std::string data) {
    QUrl urlObj;
    urlObj.setUrl(QString::fromStdString(url));
    QNetworkRequest request;
    request.setUrl(urlObj);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11");
    QEventLoop loop;
    QNetworkReply *reply = manager->post(request, QString::fromStdString(data).toAscii());
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();
    QByteArray array = reply->readAll();
    reply->close();
    return QString(array).toStdString();
}
