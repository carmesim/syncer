TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cli.c \
        fileutils.c \
        main.c

HEADERS += \
    cli.h \
    fileutils.h
