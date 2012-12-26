#include "searchmusicwindow.h"
#include "searchmusiclastitemseperator.h"
#include "ui_searchmusicwindow.h"
#include <QGraphicsDropShadowEffect>
#include "searchmusiclistitem.h"
#include "searchmusiclistitem.h"
#include "API/song.h"
#include "API/artist.h"
#include "QtConcurrent/QtConcurrentRun"
#include "QFuture"
#include "QLayout"
#include <QTimer>
#include "coverhelper.h"
#include <QResizeEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QRect>

SearchMusicWindow::SearchMusicWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchMusicWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~(Qt::WindowFullscreenButtonHint));
    refreshingPlaylists = false;
    QRect geometry = QApplication::desktop()->screenGeometry();
    this->setGeometry((geometry.width() - this->width()) / 2, (geometry.height() - this->height()) / 2, this->width(), this->height());
    ui->lblBtnBack->setAttribute(Qt::WA_TransparentForMouseEvents);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblSearchMusic);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblSearchMusic->setGraphicsEffect(effect);

    effect = new QGraphicsDropShadowEffect(ui->lblSearchSong);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblSearchSong->setGraphicsEffect(effect);
    ui->txtSearchSong->setAttribute(Qt::WA_MacShowFocusRect, false);

    effect = new QGraphicsDropShadowEffect(ui->lblAddAll);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblAddAll->setGraphicsEffect(effect);

    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->size());

    handler = new MessageHandler(this);
    connect(handler, SIGNAL(addedMessage(Message*)), this, SLOT(addedMessage(Message*)));
    connect(handler, SIGNAL(removedMessage(Message*)), this, SLOT(deletedMessage(Message*)));
}

void SearchMusicWindow::onPlaylistChange(std::vector<std::string> vector) {
    refreshingPlaylists = true;
    ui->cmbAddAll->clear();
    ui->cmbAddAll->addItem("Bitte wählen...");
    for (unsigned int i = 0; i < vector.size(); i++) {
        ui->cmbAddAll->addItem(QString::fromStdString(vector.at(i)));
    }
    ui->cmbAddAll->addItem("Neue Playlist erstellen");
    ui->cmbAddAll->setCurrentIndex(0);
    refreshingPlaylists = false;
}

void SearchMusicWindow::setPlayer(Player *player) {
    this->player = player;
}

void SearchMusicWindow::deletedMessage(Message *message) {
    this->layout()->removeWidget(message);
}

void SearchMusicWindow::addedMessage(Message *message) {
    this->layout()->addWidget(message);
}

SearchMusicWindow::~SearchMusicWindow()
{
    realMainWindow->setVisible(true);
    realMainWindow->activateWindow();
    realMainWindow->raise();
    delete ui;
    disconnect(api, SIGNAL(songSearchCompleted(std::vector<Song*>)), this, SLOT(gotSongSearchResult(std::vector<Song*>)));
}

void SearchMusicWindow::setMainWindow(QMainWindow *mw) {
    this->realMainWindow = mw;
}

void SearchMusicWindow::setAPB(AudioPlayerBridge *apb) {
    this->apb = apb;
}

void SearchMusicWindow::setAPI(API *api) {
    this->api = api;
    connect(api, SIGNAL(songSearchCompleted(std::vector<Song*>)), this, SLOT(gotSongSearchResult(std::vector<Song*>)));
    connect(api, SIGNAL(popularSongSearchCompleted(std::vector<Song*>)), this, SLOT(gotSongSearchResult(std::vector<Song*>)));
}

void SearchMusicWindow::on_btnBack_clicked()
{
    this->close();
}

void SearchMusicWindow::on_btnSearchSong_clicked()
{
    searchSong(ui->txtSearchSong->text().toStdString());
}

void SearchMusicWindow::addTestItem() {
    handler->addMessage("test");
}

void SearchMusicWindow::createNewPlaylist() {
    bool ok = false;
    //std::string name = "test";
    std::string name = QInputDialog::getText(this->parentWidget(), "GSP - Playlist hinzufügen", "Geben sie den Namen für die Playlist ein!", QLineEdit::Normal, QString(), &ok).toStdString();
    if (ok) {
        if (name == "") {
            QMessageBox::warning(this->parentWidget(), "GSP - Playlist erstellen", "Sie müssen einen Namen eingeben");
            ui->cmbAddAll->setCurrentIndex(0);
            return;
        }
        if (!plh->createPlaylist(name)) {
            QMessageBox::warning(this->parentWidget(), "GSP - Playlist erstellen", QString::fromStdString("Die Playlist '" + name + "' existiert bereits!"));
            ui->cmbAddAll->setCurrentIndex(0);
            return;
        }
        for (int i = 0; i < ui->lstItems->count(); i++) {
            QWidget *widget = ui->lstItems->itemWidget(ui->lstItems->item(i));
            SearchMusicListItem *listItem = qobject_cast<SearchMusicListItem*>(widget);
            if (listItem != NULL) {
                //positive...
                plh->addEntry(listItem->getSong(), name);
            }
        }
        handler->addMessage("Playlist '" + name + "' erfolgreich erstellt und alle Songs erfolgreich hinzugefügt!");
    }
    ui->cmbAddAll->setCurrentIndex(0);
    return;
}

void SearchMusicWindow::getPopularSongs() {
    working = true;
    ui->cmbAddAll->setEnabled(false);
    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));
    this->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->popularGetSongs();
}

void SearchMusicWindow::getSongsByAlbum(int albumId) {
    working = true;
    ui->cmbAddAll->setEnabled(false);
    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));
    this->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->albumGetAllSongs(albumId);
}

void SearchMusicWindow::getSongsByArtist(int artistId) {
    working = true;
    ui->cmbAddAll->setEnabled(false);
    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));
    this->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->artistGetAllSongsEx(artistId);
}

void SearchMusicWindow::searchSong(std::string text) {
    working = true;
    ui->cmbAddAll->setEnabled(false);
    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));
    this->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->getResultsFromSongSearch(text);
}

void SearchMusicWindow::gotSongSearchResult(std::vector<Song*> result) {
    ui->lstItems->clear();
    for (unsigned int i = 0; i < itemVector.size(); i++) {
        delete itemVector.at(i);
        if (i % 50 == 0) {
            QApplication::processEvents();
        }
    }
    itemVector.clear();
    overlay->setItemMax(result.size());
    for (unsigned int i = 0; i < result.size(); i++) {
        QListWidgetItem *item = new QListWidgetItem(ui->lstItems);
        SearchMusicListItem *myItem = new SearchMusicListItem(ui->lstItems, result.at(i), api, apb, plh, handler, player);
        connect(myItem, SIGNAL(getSongsByArtist(int)), this, SLOT(getSongsByArtist(int)));
        connect(myItem, SIGNAL(getSongsByAlbum(int)), this, SLOT(getSongsByAlbum(int)));
        item->setSizeHint(QSize(374, 101));
        ui->lstItems->addItem(item);
        ui->lstItems->setItemWidget(item, myItem);
        itemVector.push_back(myItem);
        if (i % 35 == 0) {
            overlay->setItemsDone(i + 1);
            QApplication::processEvents();
        }
    }
    addSeperator();
    overlay->setItemsDone(result.size());
    overlay->blendOutAnimation->start();
    ui->cmbAddAll->setEnabled(true);
}

void SearchMusicWindow::addSeperator() {
    QListWidgetItem *item = new QListWidgetItem(ui->lstItems);
    SearchMusicLastItemSeperator *myItem = new SearchMusicLastItemSeperator(ui->lstItems);
    item->setSizeHint(myItem->size());
    ui->lstItems->addItem(item);
    ui->lstItems->setItemWidget(item, myItem);
}

void SearchMusicWindow::fullyBlendedOut() {
    this->layout()->removeWidget(overlay);
    delete overlay;
    working = false;
}

void SearchMusicWindow::on_txtSearchSong_returnPressed()
{
    if (!working) {
        ui->btnSearchSong->click();
    }
}

void SearchMusicWindow::setPLH(PlaylistHandler *plh) {
    this->plh = plh;
    onPlaylistChange(plh->getPlaylists());
    connect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(onPlaylistChange(std::vector<std::string>)));
}

void SearchMusicWindow::on_btnPopular_clicked()
{
    getPopularSongs();
}

void SearchMusicWindow::on_cmbAddAll_currentIndexChanged(int index)
{
    if (index == 0 || refreshingPlaylists)
        return;
    if (index == ui->cmbAddAll->count() - 1) {
        createNewPlaylist();
        return;
    }
    for (int i = 0; i < ui->lstItems->count(); i++) {
        QWidget *widget = ui->lstItems->itemWidget(ui->lstItems->item(i));
        SearchMusicListItem *listItem = qobject_cast<SearchMusicListItem*>(widget);
        if (listItem != NULL) {
            //positive...
            plh->addEntry(listItem->getSong(), ui->cmbAddAll->itemText(index).toStdString());
        }
    }
    handler->addMessage("Alle Ergebnisse wurden erfolgreich zu '" + ui->cmbAddAll->itemText(index).toStdString() + "' hinzugefügt!");
    ui->cmbAddAll->setCurrentIndex(0);
    return;
}
