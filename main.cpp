#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <QTextStream>
#include <QTextCodec>

QTextStream stream(fopen("log", "w"));

void myMessageOutput(QtMsgType type, const char *msg)
 {
     //in this function, you can write the message to any stream!
    stream << QString(msg);
    stream << QString("\n");
 }

int main(int argc, char *argv[])
{
    qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8") );
    MainWindow w;
    w.show();
    int i = a.exec();
    stream.flush();
    return i;
}
