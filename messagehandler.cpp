#include "messagehandler.h"
#include <iterator>
#include <QRect>

MessageHandler::MessageHandler(QWidget *parent) : QObject(parent)
{
    parentWidget = parent;
}

void MessageHandler::addMessage(std::string text) {
    Message* message = new Message();
    QPoint point(0, (message->height() * messages.size()));
    message->move(point);
    message->setText(text);
    messages.push_back(message);
    connect(message->blendOut, SIGNAL(finished()), this, SLOT(messageDisappeared()));
    emit addedMessage(message);
    message->blendIn->start();
}

void MessageHandler::messageDisappeared() {
    emit removedMessage(messages[0]);
    delete messages[0];
    messages.pop_front();
    for (int i = 0; i < messages.size(); i++) {
        Message* currMessage = (Message*) messages[i];
        currMessage->moveUp->setEndValue(QRect(0, i * currMessage->height(), currMessage->width(), currMessage->height()));
        currMessage->moveUp->setEasingCurve(QEasingCurve::InOutCubic);
    }
    for (int i = 0; i < messages.size(); i++) {
        Message* currMessage = (Message*) messages[i];
        if (currMessage->moveUp->state() != QPropertyAnimation::Running) {
            currMessage->moveUp->start();
        }
    }
}
