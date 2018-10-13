QT += core
QT -= gui

CONFIG += c++11

TARGET = noThankYou
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    deck.cpp \
    player.cpp

HEADERS += \
    deck.h \
    player.h
