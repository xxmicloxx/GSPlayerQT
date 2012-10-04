#include "searchmusiclastitemseperator.h"
#include "ui_searchmusiclastitemseperator.h"

SearchMusicLastItemSeperator::SearchMusicLastItemSeperator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchMusicLastItemSeperator)
{
    ui->setupUi(this);
    this->setFixedHeight(2);
}

SearchMusicLastItemSeperator::~SearchMusicLastItemSeperator()
{
    delete ui;
}
