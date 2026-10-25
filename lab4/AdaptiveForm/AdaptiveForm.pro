QT += widgets

CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = AdaptiveForm
TEMPLATE = app

SOURCES += \
    main.cpp \
    adaptivefield.cpp \
    formwindow.cpp

HEADERS += \
    adaptivefield.h \
    formwindow.h
