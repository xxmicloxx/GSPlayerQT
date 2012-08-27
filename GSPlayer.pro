#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T09:42:04
#
#-------------------------------------------------

QT       += core gui network phonon

TARGET = GSPlayer
TEMPLATE = app


SOURCES += main.cpp\
        interactionwindow.cpp \
    API/util.cpp \
    audioplayerbridge.cpp \
    API/api.cpp \
    API/streaminformation.cpp \
    mainwindow.cpp \
    searchmusicwindow.cpp \
    searchmusiclistitem.cpp \
    API/song.cpp

HEADERS  += interactionwindow.h \
    API/util.h \
    audioplayerbridge.h \
    API/api.h \
    API/streaminformation.h \
    mainwindow.h \
    searchmusicwindow.h \
    searchmusiclistitem.h \
    API/song.h

FORMS    += interactionwindow.ui \
    mainwindow.ui \
    searchmusicwindow.ui \
    searchmusiclistitem.ui

LIBS += libbass.dylib
LIBS += libqjson.0.dylib
LIBS += -L. -lcrypto -lssl -ljsonbox
LIBS += -L/Users/ml/Downloads/boost_1_50_0/build/lib -lboost_system -lboost_date_time -lboost_regex -lboost_thread -lboost_serialization
LIBS += -L/Users/ml/Downloads/cpp-netlib-0.9.4/build/libs/network/src -lcppnetlib-client-connections -lcppnetlib-server-parsers -lcppnetlib-uri


HEADERS += bass.h

RESOURCES += \
    Design.qrc

RC_FILE = myWinIcon.rc

INCLUDEPATH += /Users/ml/Downloads/boost_1_50_0
INCLUDEPATH += /Users/ml/Downloads/cpp-netlib-0.9.4
INCLUDEPATH += /Users/ml/Downloads/jsonbox-0.4.3-src/include
