#include "artistlistitem.h"
#include "ui_artistlistitem.h"

ArtistListItem::ArtistListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArtistListItem)
{
    ui->setupUi(this);
}

ArtistListItem::~ArtistListItem()
{
    delete ui;
}
