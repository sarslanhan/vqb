# -------------------------------------------------
# Project created by QtCreator 2009-08-07T12:01:26
# -------------------------------------------------
TARGET = vqb
TEMPLATE = app
SOURCES += main.cpp \
    vqbform.cpp \
    querythread.cpp \
    constraint.cpp \
    combobox.cpp \
    sparqlhighlighter.cpp \
    subjecttree.cpp \
    querynode.cpp \
    vqbinstancesform.cpp \
    vqbmainwindow.cpp \
    vqbschemaform.cpp
HEADERS += vqbform.h \
    querythread.h \
    constraint.h \
    combobox.h \
    sparqlhighlighter.h \
    subjecttree.h \
    querynode.h \
    vqbinstancesform.h \
    vqbmainwindow.h \
    vqbschemaform.h
FORMS += vqbinstancesform.ui \
    vqbmainwindow.ui
OTHER_FILES += CMakeLists.txt \
    notes.txt
CONFIG += qt \
    link_pkgconfig
PKGCONFIG += soprano
INCLUDEPATH = /home/kde-devel/kde/include/
