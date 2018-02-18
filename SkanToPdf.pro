#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T10:57:54
#
#-------------------------------------------------

QT       += core gui printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SkanToPdf
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    configuredialog.cpp \
    scannedimagedisplay.cpp

HEADERS  += dialog.h \
    configuredialog.h \
    scannedimagedisplay.h

FORMS    += dialog.ui \
    configuredialog.ui \
    scannedimagedisplay.ui
LIBS += -lKF5Sane
