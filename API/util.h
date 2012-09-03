#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>

class Util : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    int currentPostActionId;

public:
    explicit Util(QObject *parent = 0);
    static std::string getMd5FromString(std::string md5);
    static std::string getSha1FromString(std::string sha1string);
    void postData(std::string url, std::string data, int postActionId);
    ~Util();
    
signals:
    void dataPosted(int postActionId, std::string resultText);
    
private slots:
    void dataReceived(QNetworkReply *reply);
    
private:
    QMutex mutex;
};

#endif // UTIL_H
