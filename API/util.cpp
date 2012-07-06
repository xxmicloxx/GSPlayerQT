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
    qnam = new QNetworkAccessManager(this);
    qnamwprox = new QNetworkAccessManager(this);
    qnamwprox->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, "177.43.44.79", 3128));
}

QString Util::getMd5FromString(QString md5string) {
    return QString(QCryptographicHash::hash(md5string.toAscii(), QCryptographicHash::Md5).toHex());
}

QString Util::getSha1FromString(QString sha1string) {
    return QString(QCryptographicHash::hash(sha1string.toAscii(), QCryptographicHash::Sha1).toHex());
}

QString Util::getSetCookieHeaderWithProxy(QString url) {
    QNetworkRequest requ = QNetworkRequest(url);
    requ.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11");
    QNetworkReply* reply = qnamwprox->get(requ);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();
    QByteArray lol = reply->rawHeader("Set-Cookie");
    reply->close();
    return QString(lol);
}

QString Util::postData(QString url, QString data) {
    QNetworkRequest requ = QNetworkRequest(url);
    requ.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    requ.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11");
    QByteArray byteArray = data.toAscii();
    QNetworkReply* reply = qnam->post(requ, byteArray);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    reply->deleteLater();
    QByteArray result = reply->readAll();
    reply->close();
    return QString(result);
}
