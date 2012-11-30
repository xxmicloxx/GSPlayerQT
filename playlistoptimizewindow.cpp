#include "playlistoptimizewindow.h"
#include "ui_playlistoptimizewindow.h"
#include "playlistoptimizelistitem.h"
#include "playlistoptimizelastitemseperator.h"
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopWidget>
#include <QRect>

PlaylistOptimizeWindow::PlaylistOptimizeWindow(QWidget *parent, CoverHelper *coverHelper, Player *player) :
    QMainWindow(parent),
    ui(new Ui::PlaylistOptimizeWindow)
{
    this->coverHelper = coverHelper;
    this->player = player;
    ui->setupUi(this);
    QRect geometry = QApplication::desktop()->screenGeometry();
    this->setGeometry((geometry.width() - this->width()) / 2, (geometry.height() - this->height()) / 2, this->width(), this->height());
    connect(ui->lstItems, SIGNAL(replaceSeperator()), this, SLOT(reimplementSeperator()));
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->size());
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblOrganizePlaylists);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblOrganizePlaylists->setGraphicsEffect(effect);
    effect = new QGraphicsDropShadowEffect(ui->lblChoosePlaylist);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblChoosePlaylist->setGraphicsEffect(effect);

    playlistsRefreshing = false;

    QLabel* transLabelArray[] = {ui->lblAddTitle, ui->lblBtnBack, ui->lblCreatePlaylist, ui->lblImport, ui->lblDeletePlaylist};
    int count = sizeof(transLabelArray) / sizeof(QLabel*); //because size of array is always the size of its contents
    for (int i = 0; i < count; i++) {
        transLabelArray[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

PlaylistOptimizeWindow::~PlaylistOptimizeWindow()
{
    realMainWindow->setVisible(true);
    realMainWindow->activateWindow();
    realMainWindow->raise();
    disconnect(plh, SIGNAL(songsChanged(std::string,std::vector<Song*>)), this, SLOT(refreshItems(std::string,std::vector<Song*>)));
    disconnect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(refreshPlaylists(std::vector<std::string>)));
    delete ui;
}

void PlaylistOptimizeWindow::setAPI(API *api) {
    this->api = api;
}

void PlaylistOptimizeWindow::setPLH(PlaylistHandler *plh) {
    this->plh = plh;
    refreshPlaylists(plh->getPlaylists());
    connect(plh, SIGNAL(playlistsChanged(std::vector<std::string>)), this, SLOT(refreshPlaylists(std::vector<std::string>)));
    connect(plh, SIGNAL(songsChanged(std::string,std::vector<Song*>)), this, SLOT(refreshItems(std::string,std::vector<Song*>)));
}

void PlaylistOptimizeWindow::addSeperator() {
    PlaylistOptimizeLastItemSeperator *myItem = new PlaylistOptimizeLastItemSeperator(ui->lstItems);
    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(544, 2));
    ui->lstItems->addItem(item);
    ui->lstItems->setItemWidget(item, myItem);
}

void PlaylistOptimizeWindow::refreshPlaylists(std::vector<std::string> playlists) {
    playlistsRefreshing = true;
    std::string oldPlaylistName = ui->cmbPlaylist->currentText().toStdString();
    ui->cmbPlaylist->clear();
    for (unsigned int i = 0; i < playlists.size(); i++) {
        ui->cmbPlaylist->addItem(QString::fromStdString(playlists.at(i)));
    }
    bool success = false;
    for (int i = 0; i < ui->cmbPlaylist->count(); i++) {
        if (ui->cmbPlaylist->itemText(i).toStdString() == oldPlaylistName) {
            ui->cmbPlaylist->setCurrentIndex(i);
            success = true;
        }
    }
    playlistsRefreshing = false;
    if (!success) {
        refreshItems(ui->cmbPlaylist->currentText().toStdString(), plh->getSongs(ui->cmbPlaylist->currentText().toStdString()));
    }
}

void PlaylistOptimizeWindow::reimplementSeperator() {
    for (int i = 0; i < ui->lstItems->count(); i++) {
        if (qobject_cast<PlaylistOptimizeLastItemSeperator*>(ui->lstItems->itemWidget(ui->lstItems->item(i))) != NULL) {
            // positive...
            delete ui->lstItems->item(i);
        }
    }
    addSeperator();
    std::vector<Song*> songVector;
    for (int i = 0; i < ui->lstItems->count(); i++) {
        if (qobject_cast<PlaylistOptimizeListItem*>(ui->lstItems->itemWidget(ui->lstItems->item(i))) != NULL) {
            //positive...
            songVector.push_back(((PlaylistOptimizeListItem*)ui->lstItems->itemWidget(ui->lstItems->item(i)))->getSong());
        }
    }
    plh->redefinePlaylist(ui->cmbPlaylist->currentText().toStdString(), songVector);
}

void PlaylistOptimizeWindow::on_cmbPlaylist_currentIndexChanged(int index)
{
    refreshItems(ui->cmbPlaylist->currentText().toStdString(), plh->getSongs(ui->cmbPlaylist->itemText(index).toStdString()));
}

void PlaylistOptimizeWindow::setMainWindow(QMainWindow *mw) {
    this->realMainWindow = mw;
}

void PlaylistOptimizeWindow::refreshItems(std::string playlistName, std::vector<Song*> songVector) {
    if (playlistsRefreshing || playlistName != ui->cmbPlaylist->currentText().toStdString())
        return;
    //    SearchMusicListItem *myItem1 = new SearchMusicListItem(ui->lstItems, "Call Me Maybe", "Carly Rae Jepsen", "2:53", 0);
    //    QListWidgetItem *item1 = new QListWidgetItem();
    //    item1->setSizeHint(QSize(374, 101));
    //    ui->lstItems->addItem(item1);
    //    ui->lstItems->setItemWidget(item1, myItem1);
    ui->lstItems->clear();
    for (unsigned int i = 0; i < songVector.size(); i++) {
        PlaylistOptimizeListItem *myItem = new PlaylistOptimizeListItem(ui->lstItems, songVector.at(i), plh, playlistName, coverHelper);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(544, 80));
        ui->lstItems->addItem(item);
        ui->lstItems->setItemWidget(item, myItem);
    }
    addSeperator();
}

void PlaylistOptimizeWindow::on_btnCreatePlaylist_clicked()
{
    bool ok = false;
    std::string text = QInputDialog::getText(this, "GSP - Playlist erstellen", "Bitte geben sie den Namen der neuen Playlist ein:", QLineEdit::Normal, QString(), &ok).toStdString();
    if (!ok)
        return;
    if (text == "") {
        QMessageBox::warning(this, "GSP - Playlist erstellen", "Sie müssen einen Namen angeben!");
        return;
    }
    if (!plh->createPlaylist(text)) {
        QMessageBox::warning(this, "GSP - Playlist erstellen", "Diese Playlist existiert bereits!");
        return;
    }
    QMessageBox::information(this, "GSP - Playlist erstellen", "Die Playlist wurde erfolgreich erstellt!");
}

void PlaylistOptimizeWindow::on_btnDeletePlaylist_clicked()
{
    if (ui->cmbPlaylist->currentText() == "")
        return;
    switch (QMessageBox::question(this, "GSP - Playlist löschen", "Möchten sie die Playlist '" + ui->cmbPlaylist->currentText() + "' wirklich löschen?", QMessageBox::Ok, QMessageBox::Abort)) {
    case QMessageBox::Ok:
        plh->removePlaylist(ui->cmbPlaylist->currentText().toStdString());
        QMessageBox::information(this, "GSP - Playlist löschen", "Die Playlist wurde erfolgreich gelöscht!");
        break;
    }
}

void PlaylistOptimizeWindow::on_btnRename_clicked()
{
    bool ok = false;
    std::string result = QInputDialog::getText(this, "GSP - Playlist umbenennen", "Bitte geben sie den neuen Namen für '" + ui->cmbPlaylist->currentText() + "' ein:", QLineEdit::Normal, QString(), &ok).toStdString();
    if (!ok)
        return;
    if (result == "") {
        QMessageBox::warning(this, "GSP - Playlist umbenennen", "Sie müssen einen neuen Namen eingeben!");
        return;
    }
    if (plh->checkIfPlaylistExists(result)) {
        QMessageBox::warning(this, "GSP - Playlist umbenennen", "Dieser Name existiert bereits!");
        return;
    }
    plh->renamePlaylist(ui->cmbPlaylist->currentText().toStdString(), result);
    for (int i = 0; i < ui->cmbPlaylist->count(); i++) {
        if (ui->cmbPlaylist->itemText(i).toStdString() == result) {
            ui->cmbPlaylist->setCurrentIndex(i);
        }
    }
    QMessageBox::information(this, "GSP - Playlist umbenennen", "Playlist erfolgreich umbenannt!");
}

void PlaylistOptimizeWindow::on_btnPlay_clicked()
{
    if (ui->cmbPlaylist->currentText() == "")
        return;
    player->setPlaylist(ui->cmbPlaylist->currentText().toStdString());
    player->play();
}
