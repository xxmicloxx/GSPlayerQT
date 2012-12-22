#include "playlistoptimizelistitem.h"
#include "ui_playlistoptimizelistitem.h"
#include "songinfodialog.h"

PlaylistOptimizeListItem::PlaylistOptimizeListItem(QWidget *parent, Song *song, PlaylistHandler *plh, std::string playlist, CoverHelper *coverHelper) :
    QWidget(parent),
    ui(new Ui::PlaylistOptimizeListItem)
{
    ui->setupUi(this);
    this->song = song;
    this->plh = plh;
    this->playlist = playlist;
    this->coverHelper = coverHelper;
    ui->lblTitle->setText(QString::fromStdString(song->getSongName()));
    ui->lblArtist->setText(QString::fromStdString(song->getArtistName()));
    QLabel* transLabelArray[] = {ui->lblTitleCaption, ui->lblTitle, ui->lblArtistCaption, ui->lblArtist};
    int count = sizeof(transLabelArray) / sizeof(QLabel*); //because size of array is always the size of its contents
    for (int i = 0; i < count; i++) {
        transLabelArray[i]->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

PlaylistOptimizeListItem::~PlaylistOptimizeListItem()
{
    delete ui;
}

void PlaylistOptimizeListItem::on_btnRemove_clicked()
{
    plh->removeSong(song, playlist);
}

void PlaylistOptimizeListItem::on_btnInfo_clicked()
{
    SongInfoDialog* songInfoDialog = new SongInfoDialog(this, song);
    songInfoDialog->setCoverHelper(coverHelper);
    songInfoDialog->setModal(true);
    songInfoDialog->show();
}

Song* PlaylistOptimizeListItem::getSong() {
    return song;
}
