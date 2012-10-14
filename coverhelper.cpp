#include "coverhelper.h"
#include <QFile>
#include <QDir>

CoverHelper::CoverHelper(QObject *parent) :
    QObject(parent)
{
    queueIsRunning = false;
    fileIsDownloading = false;
    manager = new QNetworkAccessManager(this);
    queueMap = new QVector<std::string>();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(realDownloaded(QNetworkReply*)));
    init();
}

CoverHelper::~CoverHelper() {
    delete queueMap;
}

void CoverHelper::init() {
    QDir coverFolder(".");
    if (!coverFolder.exists("covers")) {
        coverFolder.mkdir("covers");
    }

}

void CoverHelper::getPathForCover(std::string coverArtFilename) {
    QFile coverFile("covers/" + QString::fromStdString(coverArtFilename));
    if (coverFile.exists()) {
        gotPathForCover(coverArtFilename);
    } else {
        downloadCover(coverArtFilename);
    }
}

void CoverHelper::gotPathForCover(std::string coverArtFilename) {
    emit coverGotten(coverArtFilename);
}

void CoverHelper::downloadCover(std::string coverArtFilename) {
    queueMap->push_back(coverArtFilename);
    continueQueue();
}
void CoverHelper::downloadedCover(std::string coverArtFilename) {
    gotPathForCover(coverArtFilename);
    fileIsDownloading = false;
    continueQueue();
}

void CoverHelper::continueQueue() {
    if (queueIsRunning || fileIsDownloading || queueMap->size() == 0) {
        return;
    }
    queueIsRunning = true;
    QFile file(QString::fromStdString("covers/" + queueMap->at(0)));
    if (file.exists()) {
        queueIsRunning = false;
        gotPathForCover(queueMap->at(0));
        continueQueue();
        return;
    }
    fileIsDownloading = true;
    currentCover = queueMap->at(0);
    queueMap->pop_front();
    realDownload();
    queueIsRunning = false;
}

void CoverHelper::realDownload() {
    QUrl url;
    url.setUrl("http://images.grooveshark.com/static/albums/500_" + QString::fromStdString(currentCover));
    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
}

void CoverHelper::realDownloaded(QNetworkReply *reply) {
    QByteArray data = reply->readAll();
    QFile file(QString::fromStdString("covers/" + currentCover));
    file.open(QFile::WriteOnly);
    file.write(data);
    file.flush();
    file.close();
    reply->deleteLater();
    downloadedCover(currentCover);
}
