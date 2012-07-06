#-------------------------------------------------
#
# Project created by QtCreator 2012-06-09T09:42:04
#
#-------------------------------------------------

QT       += core gui network phonon

TARGET = GSPlayer
TEMPLATE = app
CONFIG -= app_bundle


SOURCES += main.cpp\
        interactionwindow.cpp \
    API/util.cpp \
    audioplayerbridge.cpp \
    API/api.cpp \
    QJson/serializerrunnable.cpp \
    QJson/serializer.cpp \
    QJson/qobjecthelper.cpp \
    QJson/parserrunnable.cpp \
    QJson/parser.cpp \
    QJson/json_scanner.cpp \
    QJson/json_parser.cc \
    API/streaminformation.cpp

HEADERS  += interactionwindow.h \
    API/util.h \
    audioplayerbridge.h \
    API/api.h \
    QJson/stack.hh \
    QJson/serializerrunnable.h \
    QJson/serializer.h \
    QJson/qobjecthelper.h \
    QJson/qjson_export.h \
    QJson/qjson_debug.h \
    QJson/position.hh \
    QJson/parserrunnable.h \
    QJson/parser.h \
    QJson/parser_p.h \
    QJson/location.hh \
    QJson/json_scanner.h \
    QJson/json_parser.hh \
    API/streaminformation.h

FORMS    += interactionwindow.ui

LIBS += libbass.dylib

HEADERS += bass.h
