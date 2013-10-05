#-------------------------------------------------
#
# Project created by QtCreator 2013-10-02T20:38:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bfuzz
TEMPLATE = app
LIBS += -lelf -ldistorm3 -lm

SOURCES += main.cpp\
        mainwindow.cpp \
    val.cpp \
    tracer.cpp \
    target.cpp \
    tag.cpp \
    elf.cpp \
    cond.cpp \
    break.cpp \
    branch.cpp \
    access.cpp \
    tabmodel.cpp

HEADERS  += mainwindow.h \
    val.h \
    tracer.h \
    types.h \
    trace.h \
    target.h \
    tagger.h \
    tag.h \
    frame.h \
    distorm.h \
    cond.h \
    break.h \
    branch.h \
    access.h \
    myelf.h \
    tabmodel.h

FORMS    += mainwindow.ui