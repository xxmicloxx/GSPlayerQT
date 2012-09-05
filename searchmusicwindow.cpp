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

    overlay = new SearchMusicOverlay();
    connect(overlay->blendOutAnimation, SIGNAL(finished()), this, SLOT(fullyBlendedOut()));

//    SearchMusicListItem *myItem1 = new SearchMusicListItem(ui->lstItems, "Call Me Maybe", "Carly Rae Jepsen", "2:53", 0);
//    QListWidgetItem *item1 = new QListWidgetItem();
//    item1->setSizeHint(QSize(374, 101));
//    ui->lstItems->addItem(item1);
//    ui->lstItems->setItemWidget(item1, myItem1);
}

SearchMusicWindow::~SearchMusicWindow()
{
    delete ui;
    delete overlay;
    disconnect(api, SIGNAL(songSearchCompleted(std::vector<Song*>)), this, SLOT(gotSongSearchResult(std::vector<Song*>)));
}

void SearchMusicWindow::setAPB(AudioPlayerBridge *apb) {
    this->apb = apb;
}

void SearchMusicWindow::setAPI(API *api) {
    this->api = api;
    connect(api, SIGNAL(songSearchCompleted(std::vector<Song*>)), this, SLOT(gotSongSearchResult(std::vector<Song*>)));
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
    std::vector<Artist*> resultVector = api->getResultsFromArtistSearch(ui->txtSearchArtist->text().toStdString());
    ui->lstItems->clear();
}

void SearchMusicWindow::searchSong(std::string text) {
    ui->centralwidget->layout()->addWidget(overlay);
    overlay->blendInAnimation->start();
    api->getResultsFromSongSearch(text);
}

void SearchMusicWindow::gotSongSearchResult(std::vector<Song*> result) {
    ui->lstItems->clear();
    for (int i = 0; i < result.size(); i++) {
        SearchMusicListItem *myItem = new SearchMusicListItem(this, result.at(i), api, apb);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(374, 101));
        ui->lstItems->addItem(item);
        ui->lstItems->setItemWidget(item, myItem);
    }
    overlay->blendOutAnimation->start();
}

void SearchMusicWindow::fullyBlendedOut() {
    ui->centralwidget->layout()->removeWidget(overlay);
}
