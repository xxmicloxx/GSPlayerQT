#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class Util : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager* qnam;
    QNetworkAccessManager* qnamwprox;
public:
    explicit Util(QObject *parent = 0);
    static QString getMd5FromString(QString md5);
    static QString getSha1FromString(QString sha1string);
    QString getSetCookieHeaderWithProxy(QString url);
    QString postData(QString url, QString data);
    
signals:
    
public slots:
    
};

#endif // UTIL_H
