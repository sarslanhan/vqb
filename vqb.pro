# -------------------------------------------------
# Project created by QtCreator 2009-08-07T12:01:26
# -------------------------------------------------
TARGET = vqb
TEMPLATE = app
SOURCES += main.cpp \
    vqbform.cpp \
    querythread.cpp \
    constraint.cpp \
    combobox.cpp
HEADERS += vqbform.h \
    querythread.h \
    constraint.h \
    combobox.h
FORMS += vqbform.ui
OTHER_FILES += CMakeLists.txt \
    notes.txt
CONFIG += qt \
    link_pkgconfig
PKGCONFIG += soprano