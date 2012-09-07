#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class Message;
}

class Message : public QWidget
{
    Q_OBJECT
    
public:
    explicit Message(QWidget *parent = 0);
    ~Message();
    void setText(std::string text);
    QPropertyAnimation *blendIn;
    QPropertyAnimation *blendOut;
    QPropertyAnimation *moveUp;
    
private slots:
    void blendedIn();

private:
    QGraphicsOpacityEffect *opacity;
    Ui::Message *ui;
};

#endif // MESSAGE_H
