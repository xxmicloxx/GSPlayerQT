#ifndef DRAGDROPLIST_H
#define DRAGDROPLIST_H

#include <QListWidget>
#include <QDropEvent>

class DragDropList : public QListWidget
{
    Q_OBJECT
public:
    explicit DragDropList(QWidget *parent = 0);
    
protected:
    virtual void dropEvent(QDropEvent* event);

signals:
    void replaceSeperator();
    
public slots:
    
};

#endif // DRAGDROPLIST_H
