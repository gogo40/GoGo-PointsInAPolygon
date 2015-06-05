#-------------------------------------------------
#
# Project created by QtCreator 2015-06-04T20:09:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PointInPolygon
TEMPLATE = app


SOURCES += main.cpp\
    point_in_polygon_test.cpp \
    point_in_polygon_widget.cpp

HEADERS  += \
    point_in_polygon_test.h \
    point_in_polygon_widget.h

CONFIG += mobility
MOBILITY = 

FORMS += \
    pointinpolygon.ui

CONFIG   += c++11
