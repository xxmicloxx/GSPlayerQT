#include "searchmusiclistitem.h"
#include "ui_searchmusiclistitem.h"
#include <boost/lexical_cast.hpp>
#include <QInputDialog>
#include <QMessageBox>
#include "mousewheeldisabler.h"
#include <QLayout>
#include <QtConcurrent/QtConcurrentRun>
#include "titleplaydialog.h"

SearchMusicListItem::SearchMusicListItem(QWidget *parent, Song* song, API *api, AudioPlayerBridge *apb, PlaylistHandler *plh, MessageHandler* messageHandler, Player *player) :
    QWidget(parent),
    ui(new Ui::SearchMusicListItem)
{
    ui->setupUi(this);
    playerWasPlaying = false;
    this->song = song;
    this->player = player;
    ui->lblTitle->setText(QString::fromStdString(song->getSongName()));
    ui->lblArtist->setText(QString::fromStdString(song->getArtistName()));
    ui->lblAlbum->setText(QString::fromStdString(song->getAlbumName()));
    ui->lblArtist->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblArtistCaption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblAlbum->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblAlbumCaption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->lblTitleCaption->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cmbAddTo->setCurrentIndex(0);
    MouseWheelDisabler* mwd = new MouseWheelDisabler(ui->cmbAddTo);
    ui->cmbAddTo->installEventFilter(mwd);
    connect(mwd, SIGNAL(playlistAddEvent()), this, SLOT(createNewPlaylist()));
    this->apb = apb;
    this->api = api;
    this->plh = plh;
    this->messageHandler = messageHandler;
    onPlaylistChange(plh->getPlaylists());
    connect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(onPlaylistChange(std::vector<std::string>)));
}

Song* SearchMusicListItem::getSong() {
    return song;
}

void SearchMusicListItem::onPlaylistChange(std::vector<std::string> vector) {
    working = true;
    ui->cmbAddTo->clear();
    ui->cmbAddTo->addItem("Hinzufügen zu...");
    for (unsigned int i = 0; i < vector.size(); i++) {
        ui->cmbAddTo->addItem(QString::fromStdString(vector.at(i)));
    }
    ui->cmbAddTo->addItem("Neue Playlist erstellen");
    ui->cmbAddTo->setCurrentIndex(0);
    working = false;
}

SearchMusicListItem::~SearchMusicListItem()
{
    delete ui;
    disconnect(api, SIGNAL(streamKeyReady(StreamInformation*)), this, SLOT(gotStreamKey(StreamInformation*)));
    disconnect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(onPlaylistChange(std::vector<std::string>)));
}

void SearchMusicListItem::on_btnPlay_clicked()
{
    TitlePlayDialog *tpd = new TitlePlayDialog(this->parentWidget(), apb, api, song);
    tpd->setModal(false);
    if (player->isPlaying()) {
        playerWasPlaying = true;
        player->pause();
    }
    tpd->exec();
    if (playerWasPlaying) {
        player->play();
        playerWasPlaying = false;
    }
}

void SearchMusicListItem::gotStreamKey(StreamInformation *info) {
//    if (info->getSongId() != song->getSongId())
//        return;
//    QtConcurrent::run(apb, &AudioPlayerBridge::openAndPlay, info->directUrl());
}

void SearchMusicListItem::on_cmbAddTo_currentIndexChanged(int index)
{
    if (index == 0 || working)
        return;
    if (index == ui->cmbAddTo->count() - 1) {
        createNewPlaylist();
        return;
    }
    if (!plh->addEntry(song, ui->cmbAddTo->itemText(index).toStdString())) {
        QMessageBox::warning(this->parentWidget(), "GSP - Song hinzufügen", QString::fromStdString("'" + song->getSongName() + "' existiert bereits in '" + ui->cmbAddTo->itemText(index).toStdString() + "'!"));
        ui->cmbAddTo->setCurrentIndex(0);
        return;
    }
    messageHandler->addMessage("'" + song->getSongName() + "' wurde erfolgreich zu '" + ui->cmbAddTo->itemText(index).toStdString() + "' hinzugefügt!");
    ui->cmbAddTo->setCurrentIndex(0);
    return;
}

void SearchMusicListItem::createNewPlaylist() {
    bool ok;
    std::string name = QInputDialog::getText(this->parentWidget(), "GSP - Playlist hinzufügen", "Geben sie den Namen für die Playlist ein!", QLineEdit::Normal, QString(), &ok).toStdString();
    if (ok) {
        if (name == "") {
            QMessageBox::warning(this->parentWidget(), "GSP - Playlist erstellen", "Sie müssen einen Namen eingeben");
            ui->cmbAddTo->setCurrentIndex(0);
            return;
        }
        if (!plh->createPlaylist(name)) {
            QMessageBox::warning(this->parentWidget(), "GSP - Playlist erstellen", QString::fromStdString("Die Playlist '" + name + "' existiert bereits!"));
            ui->cmbAddTo->setCurrentIndex(0);
            return;
        }
        plh->addEntry(song, name);
        messageHandler->addMessage("Playlist '" + name + "' erfolgreich erstellt und '" + song->getSongName() + "' erfolgreich hinzugefügt!");
    }
    ui->cmbAddTo->setCurrentIndex(0);
}
