#include "searchmusicwindow.h"
#include "ui_searchmusicwindow.h"
#include <QGraphicsDropShadowEffect>
#include "searchmusiclistitem.h"
#include "searchmusiclistitem.h"
#include "API/song.h"
#include "API/artist.h"
#include "QtConcurrentRun"
#include "QFuture"
#include "QLayout"

SearchMusicWindow::SearchMusicWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SearchMusicWindow)
{
    ui->setupUi(this);
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

    effect = new QGraphicsDropShadowEffect(ui->lblSearchArtist);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblSearchArtist->setGraphicsEffect(effect);

    ui->txtSearchArtist->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->txtSearchSong->setAttribute(Qt::WA_MacShowFocusRect, false);

    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->size());

//    SearchMusicListItem *myItem1 = new SearchMusicListItem(ui->lstItems, "Call Me Maybe", "Carly Rae Jepsen", "2:53", 0);
//    QListWidgetItem *item1 = new QListWidgetItem();
//    item1->setSizeHint(QSize(374, 101));
//    ui->lstItems->addItem(item1);
//    ui->lstItems->setItemWidget(item1, myItem1);
}

SearchMusicWindow::~SearchMusicWindow()
{
    delete ui;
    disconnect(api, SIGNAL(songSearchCompleted(std::vector<Song*>)), this, SLOT(gotSongSearchResult(std::vector<Song*>)));
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

void SearchMusicWindow::on_btnSearchArtist_clicked()
{
    api->checkConnect();
    //std::vector<Artist*> resultVector = api->getResultsFromArtistSearch(ui->txtSearchArtist->text().toStdString());
    //ui->lstItems->clear();
}

void SearchMusicWindow::getPopularSongs() {
    working = true;
    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));
    this->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->popularGetSongs();
}

void SearchMusicWindow::searchSong(std::string text) {
    working = true;
    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));
    this->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->getResultsFromSongSearch(text);
}

void SearchMusicWindow::gotSongSearchResult(std::vector<Song*> result) {
    ui->lstItems->clear();
    for (int i = 0; i < itemVector.size(); i++) {
        delete itemVector.at(i);
        if (i % 50 == 0) {
            QApplication::processEvents();
        }
    }
    itemVector.clear();
    overlay->setItemMax(result.size());
    for (int i = 0; i < result.size(); i++) {
        QListWidgetItem *item = new QListWidgetItem(ui->lstItems);
        SearchMusicListItem *myItem = new SearchMusicListItem(ui->lstItems, result.at(i), api, apb);
        item->setSizeHint(QSize(374, 101));
        ui->lstItems->addItem(item);
        ui->lstItems->setItemWidget(item, myItem);
        itemVector.push_back(myItem);
        if (i % 35 == 0) {
            overlay->setItemsDone(i + 1);
            QApplication::processEvents();
        }
    }
    overlay->setItemsDone(result.size());
    overlay->blendOutAnimation->start();
}

void SearchMusicWindow::fullyBlendedOut() {
    this->layout()->removeWidget(overlay);
    delete overlay;
    working = false;
}

void SearchMusicWindow::on_pushButton_clicked()
{
    getPopularSongs();
}

void SearchMusicWindow::on_txtSearchSong_returnPressed()
{
    if (!working) {
        ui->btnSearchSong->click();
    }
}
