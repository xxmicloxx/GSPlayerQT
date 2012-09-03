#ifndef ARTISTLISTITEM_H
#define ARTISTLISTITEM_H

#include <QWidget>

namespace Ui {
class ArtistListItem;
}

class ArtistListItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit ArtistListItem(QWidget *parent = 0);
    ~ArtistListItem();
    
private:
    Ui::ArtistListItem *ui;
};

#endif // ARTISTLISTITEM_H
