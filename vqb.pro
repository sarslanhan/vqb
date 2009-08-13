# -------------------------------------------------
# Project created by QtCreator 2009-08-07T12:01:26
# -------------------------------------------------
TARGET = vqb
TEMPLATE = app
SOURCES += main.cpp \
    vqbform.cpp \
    vqbbackend.cpp \
    querythread.cpp
HEADERS += vqbform.h \
    vqbbackend.h \
    querythread.h
FORMS += vqbform.ui
OTHER_FILES += CMakeLists.txt
CONFIG += qt \
    link_pkgconfig
PKGCONFIG += soprano
