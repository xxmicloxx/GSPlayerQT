#ifndef PLAYLISTOPTIMIZEWINDOW_H
#define PLAYLISTOPTIMIZEWINDOW_H

#include <QMainWindow>
#include "API/api.h"
#include "playlisthandler.h"

namespace Ui {
class PlaylistOptimizeWindow;
}

class PlaylistOptimizeWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlaylistOptimizeWindow(QWidget *parent = 0);
    ~PlaylistOptimizeWindow();
    void setPLH(PlaylistHandler* plh);
    void setAPI(API* api);
    
private slots:

private:
    Ui::PlaylistOptimizeWindow *ui;
    PlaylistHandler *plh;
    API *api;
};

#endif // PLAYLISTOPTIMIZEWINDOW_H
