#-------------------------------------------------
#
# Project created by QtCreator 2018-02-15T20:55:18
#
#-------------------------------------------------
QT       += core gui
#QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shaderfun
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


HEADERS += ./ui/ShaderPlay.h \
    ./ui/openglwidget.h \
    ./mainwindow.h \
    ./shape/camera.h \
    ./shape/image3d.h \
    ./shape/image3dex.h \
    ./shape/layer.h \
    ./shape/light.h \
    ./shape/modelgl.h \
    ./shape/shader.h \
    ./shape/shape.h \
    ./shape/vaoarray.h \
    ./Shape/Shader/SeaShader.h \
    ./seaShaderWidget.h

SOURCES += ./shape/camera.cpp \
    ./shape/image3d.cpp \
    ./shape/image3dex.cpp \
    ./shape/layer.cpp \
    ./shape/modelgl.cpp \
    ./shape/shader.cpp \
    ./shape/shape.cpp \
    ./shape/vaoarray.cpp \
    ./main.cpp \
    ./mainwindow.cpp \
    ./ui/ShaderPlay.cpp \
    ./ui/openglwidget.cpp \
    ./Shape/Shader/SeaShader.cpp \
    ./seaShaderWidget.cpp

FORMS += ./mainwindow.ui \
    ./seaShaderWidget.ui

INCLUDEPATH +=C:\opencv3.2\include\
              C:\glew\include\
              C:\glm\


LIBS += C:\opencv3.2\x86\vc14\lib\opencv_world320.lib
LIBS += C:\glew\lib\Release\Win32\glew32.lib
#    LIBS += -lopengl32 \
#    -lglu32 \
#    -lglut \


DISTFILES += \
    default.fsh
