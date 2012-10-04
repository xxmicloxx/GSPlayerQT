#ifndef COVERHELPER_H
#define COVERHELPER_H

#include <QObject>
#include <QVector>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class CoverHelper : public QObject
{
    Q_OBJECT
public:
    explicit CoverHelper(QObject *parent = 0);
    ~CoverHelper();
    void getPathForCover(std::string coverArtFilename);
    void init();

private:
    QVector<std::string>* queueMap;
    QNetworkAccessManager* manager;
    void downloadCover(std::string coverArtFilename);
    void realDownload();
    std::string currentCover;
    bool queueIsRunning;
    bool fileIsDownloading;

signals:
    void coverGotten(std::string path);
    
private slots:
    void continueQueue();
    void gotPathForCover(std::string coverArtFilename);
    void downloadedCover(std::string coverArtFilename);
    void realDownloaded(QNetworkReply* reply);
    
};

#endif // COVERHELPER_H
