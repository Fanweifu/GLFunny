#-------------------------------------------------
#
# Project created by QtCreator 2018-02-15T20:55:18
#
#-------------------------------------------------
QT       += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\        
    ui\openglwidget.cpp \
    ui\opengltest.cpp \
    mainwindow.cpp \
    math\matrices.cpp \
    shape\modelgl.cpp \
    shape\image3d.cpp \
    shape\camera.cpp \
    shape\shape.cpp \
    shape\layer.cpp \
    shape\image3dex.cpp \
    shape\vaoarray.cpp \
    shape\shader.cpp

HEADERS  += ui\openglwidget.h \
    math\matrices.h \
    math\vector.h \
    shape\modelgl.h \
    shape\image3d.h \
    shape\camera.h \
    shape\shape.h \
    shape\layer.h \
    shape\light.h \
    ui\opengltest.h \
    shape\image3dex.h \
    mainwindow.h \
    shape\vaoarray.h \
    shape\shader.h

INCLUDEPATH +=C:\opencv\mingw32\include\opencv\
              C:\opencv\mingw32\include\opencv2\
              C:\opencv\mingw32\include\
              C:\ProgramData\glewMinGW\include\



LIBS += C:\opencv\mingw32\lib\libopencv_*.a
LIBS += C:\ProgramData\glewMinGW\lib\libglew32.dll.a
LIBS += -lopengl32 \
    -lglu32 \
    -lglut \

FORMS += \
    mainwindow.ui

DISTFILES += \
    default.fsh
