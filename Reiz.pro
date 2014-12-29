#-------------------------------------------------
#
# Project created by QtCreator 2014-12-24T14:29:11
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = Reiz
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11
LIBS *= -lsfml-system -lsfml-graphics -ltmx-loader

TEMPLATE = app


SOURCES += main.cpp \
    theserver.cpp \
    clientthread.cpp \
    bulletcalculator.cpp \
    broadcaster.cpp \
    batchprocessor.cpp \
    bullet.cpp

HEADERS += \
    theserver.h \
    clientthread.h \
    bulletcalculator.h \
    broadcaster.h \
    batchprocessor.h \
    bullet.h
