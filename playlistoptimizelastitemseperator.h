#ifndef PLAYLISTOPTIMIZELASTITEMSEPERATOR_H
#define PLAYLISTOPTIMIZELASTITEMSEPERATOR_H

#include <QWidget>

namespace Ui {
class PlaylistOptimizeLastItemSeperator;
}

class PlaylistOptimizeLastItemSeperator : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlaylistOptimizeLastItemSeperator(QWidget *parent = 0);
    ~PlaylistOptimizeLastItemSeperator();
    
private:
    Ui::PlaylistOptimizeLastItemSeperator *ui;
};

#endif // PLAYLISTOPTIMIZELASTITEMSEPERATOR_H
