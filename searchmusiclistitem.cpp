#include "searchmusiclistitem.h"
#include "ui_searchmusiclistitem.h"
#include <boost/lexical_cast.hpp>

SearchMusicListItem::SearchMusicListItem(QWidget *parent, Song* song, API *api, AudioPlayerBridge *apb) :
    QWidget(parent),
    ui(new Ui::SearchMusicListItem)
{
    ui->setupUi(this);
    this->song = song;
    ui->lblTitle->setText(QString::fromStdString(song->getSongName()));
    ui->lblArtist->setText(QString::fromStdString(song->getArtistName()));
    ui->lblAlbum->setText(QString::fromStdString(song->getAlbumName()));
    ui->lblArtist->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblArtistCaption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblAlbum->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblAlbumCaption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblTitleCaption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cmbAddTo->addItem("Hinzufuegen zu...");
    ui->cmbAddTo->setCurrentIndex(0);
    this->apb = apb;
    this->api = api;
    connect(api, SIGNAL(streamKeyReady(StreamInformation*)), this, SLOT(gotStreamKey(StreamInformation*)));
}

SearchMusicListItem::~SearchMusicListItem()
{
    delete ui;
    disconnect(api, SIGNAL(streamKeyReady(StreamInformation*)), this, SLOT(gotStreamKey(StreamInformation*)));
}

void SearchMusicListItem::on_btnPlay_clicked()
{
    api->getStreamKeyFromSongIDEx(song->getSongId());
}

void SearchMusicListItem::gotStreamKey(StreamInformation *info) {
    if (info->getSongId() != song->getSongId())
        return;
    apb->openAndPlay(info->directUrl());
}
