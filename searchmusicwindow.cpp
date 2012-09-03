#include "searchmusicwindow.h"
#include "ui_searchmusicwindow.h"
#include <QGraphicsDropShadowEffect>
#include "searchmusiclistitem.h"
#include "searchmusiclistitem.h"
#include "API/song.h"
#include "API/artist.h"
#include "QtConcurrentRun"
#include "QFuture"

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
}

void SearchMusicWindow::setAPB(AudioPlayerBridge *apb) {
    this->apb = apb;
}

void SearchMusicWindow::setAPI(API *api) {
    this->api = api;
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

    ((QFuture<void>) QtConcurrent::run(api, &API::checkConnect)).waitForFinished();
    std::vector<Song*> resultVector = api->getResultsFromSongSearch(text);
    ui->lstItems->clear();
    for (int i = 0; i < resultVector.size(); i++) {
        SearchMusicListItem *myItem = new SearchMusicListItem(ui->lstItems, (Song*) resultVector.at(i), api, apb);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(374, 101));
        ui->lstItems->addItem(item);
        ui->lstItems->setItemWidget(item, myItem);
    }
}
