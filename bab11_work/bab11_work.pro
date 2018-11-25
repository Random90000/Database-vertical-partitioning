TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS_DEBUG += -pg -no-pie
QMAKE_LFLAGS_DEBUG += -pg -no-pie

SOURCES += \
        main.cpp \
    measures.cpp \
    matrix.cpp \
    bab11.cpp \
    bab.cpp

HEADERS += \
    measures.hpp \
    matrix.hpp \
    bab11.hpp \
    bab.hpp
