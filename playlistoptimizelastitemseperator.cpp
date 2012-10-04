#include "playlistoptimizelastitemseperator.h"
#include "ui_playlistoptimizelastitemseperator.h"

PlaylistOptimizeLastItemSeperator::PlaylistOptimizeLastItemSeperator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistOptimizeLastItemSeperator)
{
    ui->setupUi(this);
    this->setFixedHeight(2);
}

PlaylistOptimizeLastItemSeperator::~PlaylistOptimizeLastItemSeperator()
{
    delete ui;
}
