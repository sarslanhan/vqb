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
    vqbmainwindow.cpp \
    vqbglobal.cpp \
    completerlineedit.cpp \
    vqbinstancesconstruct.cpp \
    vqbinstancesselect.cpp \
    vqbschemaselect.cpp \
    vqbschemaconstruct.cpp
HEADERS += vqbform.h \
    querythread.h \
    constraint.h \
    combobox.h \
    sparqlhighlighter.h \
    subjecttree.h \
    querynode.h \
    vqbmainwindow.h \
    vqbglobal.h \
    completerlineedit.h \
    vqbinstancesconstruct.h \
    vqbinstancesselect.h \
    vqbschemaselect.h \
    vqbschemaconstruct.h
FORMS += vqbmainwindow.ui \
    vqbinstancesconstruct.ui \
    vqbinstancesselect.ui \
    vqbschemaconstruct.ui
OTHER_FILES += CMakeLists.txt \
    notes.txt
CONFIG += qt \
    link_pkgconfig
PKGCONFIG += soprano
INCLUDEPATH = /home/kde-devel/kde/include
