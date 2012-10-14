#include <QtGui/QApplication>
#include "browserwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrowserWindow w;
    w.show();
    
    return a.exec();
}
