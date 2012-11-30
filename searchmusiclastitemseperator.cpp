#include "searchmusiclastitemseperator.h"
#include "ui_searchmusiclastitemseperator.h"

SearchMusicLastItemSeperator::SearchMusicLastItemSeperator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchMusicLastItemSeperator)
{
    ui->setupUi(this);
    this->setFixedHeight(2);
    this->setAttribute(Qt::WA_PaintOutsidePaintEvent);
}

SearchMusicLastItemSeperator::~SearchMusicLastItemSeperator()
{
    delete ui;
}
