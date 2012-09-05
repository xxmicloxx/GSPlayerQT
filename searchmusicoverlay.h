#ifndef SEARCHMUSICOVERLAY_H
#define SEARCHMUSICOVERLAY_H

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class SearchMusicOverlay;
}

class SearchMusicOverlay : public QWidget
{
    Q_OBJECT
    
public:
    explicit SearchMusicOverlay(QWidget *parent = 0);
    ~SearchMusicOverlay();
    QPropertyAnimation* blendInAnimation;
    QPropertyAnimation* blendOutAnimation;
    void setItemMax(int count);
    void setItemsDone(int count);
    
private:
    void updateText(int remaining);
    Ui::SearchMusicOverlay *ui;
    QGraphicsOpacityEffect *effect;
};

#endif // SEARCHMUSICOVERLAY_H
