#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <deque>
#include <QObject>
#include "message.h"
#include <QPoint>

class MessageHandler : public QObject
{
    Q_OBJECT
public:
    MessageHandler(QWidget *parent);
    void addMessage(std::string text);

signals:
    void addedMessage(Message* message);
    void removedMessage(Message* message);

private slots:
    void messageDisappeared();

private:
    std::deque<Message*> messages;
    QWidget* parentWidget;

};

#endif // MESSAGEHANDLER_H
