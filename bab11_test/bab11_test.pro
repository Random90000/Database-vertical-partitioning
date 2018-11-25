include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt
CONFIG += console c++17

HEADERS += \
    tst_bab11.h

SOURCES += \
        main.cpp
