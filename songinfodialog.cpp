#include "songinfodialog.h"
#include "ui_songinfodialog.h"
#include <QGraphicsDropShadowEffect>

SongInfoDialog::SongInfoDialog(QWidget *parent, Song *song) :
    QDialog(parent),
    ui(new Ui::SongInfoDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->song = song;
    ui->lblSong->setText(QString::fromStdString(song->getSongName()));
    ui->lblArtist->setText(QString::fromStdString(song->getArtistName()));
    ui->lblAlbum->setText(QString::fromStdString(song->getAlbumName()));
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(ui->lblCaption);
    effect->setBlurRadius(1);
    effect->setColor(QColor("#bb6008"));
    effect->setOffset(0, 1);
    ui->lblCaption->setGraphicsEffect(effect);
}

void SongInfoDialog::setCoverHelper(CoverHelper *coverHelper) {
    this->coverHelper = coverHelper;
    connect(coverHelper, SIGNAL(coverGotten(std::string)), this, SLOT(gotCover(std::string)));
    std::string coverPath = song->getCoverArtFilename();
    if (coverPath == "")
        coverPath = "album.png";
    coverHelper->getPathForCover(coverPath);
}

void SongInfoDialog::gotCover(std::string coverArtFilename) {
    if (coverArtFilename != song->getCoverArtFilename()) {
        return;
    }
    ui->lblCover->setPixmap(QPixmap(QString::fromStdString("covers/" + coverArtFilename)));
}

SongInfoDialog::~SongInfoDialog()
{
    delete ui;
}

void SongInfoDialog::on_btnOk_clicked()
{
    this->close();
}
