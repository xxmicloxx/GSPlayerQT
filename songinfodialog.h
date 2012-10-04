#ifndef SONGINFODIALOG_H
#define SONGINFODIALOG_H

#include <QDialog>
#include "coverhelper.h"
#include "API/song.h"

namespace Ui {
class SongInfoDialog;
}

class SongInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SongInfoDialog(QWidget *parent = 0, Song *song = NULL);
    void setCoverHelper(CoverHelper *coverHelper);
    ~SongInfoDialog();
    
private slots:
    void on_btnOk_clicked();

public slots:
    void gotCover(std::string coverArtFilename);

private:
    Ui::SongInfoDialog *ui;
    CoverHelper *coverHelper;
    Song *song;
};

#endif // SONGINFODIALOG_H
