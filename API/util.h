#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class Util : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;

public:
    explicit Util(QObject *parent = 0);
    static std::string getMd5FromString(std::string md5);
    static std::string getSha1FromString(std::string sha1string);
    std::string postData(std::string url, std::string data);
    
signals:
    
public slots:
    
};

#endif // UTIL_H
