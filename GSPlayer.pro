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
    API/song.cpp \
    JsonBox/Value.cpp \
    JsonBox/SolidusEscaper.cpp \
    JsonBox/Object.cpp \
    JsonBox/Indenter.cpp \
    JsonBox/IndentCanceller.cpp \
    JsonBox/Escaper.cpp \
    JsonBox/Convert.cpp \
    JsonBox/Array.cpp \
    API/artist.cpp \
    artistlistitem.cpp

HEADERS  += interactionwindow.h \
    API/util.h \
    audioplayerbridge.h \
    API/api.h \
    API/streaminformation.h \
    mainwindow.h \
    searchmusicwindow.h \
    searchmusiclistitem.h \
    API/song.h \
    JsonBox.h \
    JsonBox/Value.h \
    JsonBox/SolidusEscaper.h \
    JsonBox/OutputFilter.h \
    JsonBox/Object.h \
    JsonBox/Indenter.h \
    JsonBox/IndentCanceller.h \
    JsonBox/Grammar.h \
    JsonBox/Escaper.h \
    JsonBox/Convert.h \
    JsonBox/Array.h \
    API/artist.h \
    artistlistitem.h

FORMS    += interactionwindow.ui \
    mainwindow.ui \
    searchmusicwindow.ui \
    searchmusiclistitem.ui \
    artistlistitem.ui

LIBS += C:\Users\RL\Desktop\GSPlayer\bass.lib

HEADERS += bass.h

RESOURCES += \
    Design.qrc

RC_FILE = myWinIcon.rc

INCLUDEPATH += C:\Users\RL\Desktop\boost_1_51_0
