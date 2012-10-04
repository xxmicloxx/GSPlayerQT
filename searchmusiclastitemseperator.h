#ifndef SEARCHMUSICLASTITEMSEPERATOR_H
#define SEARCHMUSICLASTITEMSEPERATOR_H

#include <QWidget>

namespace Ui {
class SearchMusicLastItemSeperator;
}

class SearchMusicLastItemSeperator : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchMusicLastItemSeperator(QWidget *parent = 0);
    ~SearchMusicLastItemSeperator();
    
private:
    Ui::SearchMusicLastItemSeperator *ui;
};

#endif // SEARCHMUSICLASTITEMSEPERATOR_H
