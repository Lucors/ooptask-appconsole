TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += sqlite3

SOURCES += \
        main.cpp \
    bones.cpp

HEADERS += \
    bones.h
