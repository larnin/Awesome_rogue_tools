#-------------------------------------------------
#
# Project created by QtCreator 2016-09-22T13:53:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Awesome_rogue_block_animator
TEMPLATE = app


SOURCES += main.cpp\
    qsfmlcanvas.cpp \
    textureviewer.cpp \
    tilesetdata.cpp \
    quadrender.cpp \
    animatorconfig.cpp \
    textureviewerwidget.cpp \
    grid.cpp \
    blockanimator.cpp \
    animationviewer.cpp \
    tile.cpp

HEADERS  += \
    qsfmlcanvas.h \
    textureviewer.h \
    tilesetdata.h \
    quadrender.h \
    ressource.h \
    vect2convert.h \
    animation.h \
    rotation.h \
    animatorconfig.h \
    textureviewerwidget.h \
    grid.h \
    tile.h \
    blockanimator.h \
    animationviewer.h

CONFIG += c++14
#QMAKE_CXXFLAGS += -Wsign-conversion

# SFML
DEFINES += SFML_STATIC
LIBS += -LC:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/lib

CONFIG(release, debug|release): LIBS += -lsfml-graphics-s \
                                        -lsfml-window-s \
                                        -lsfml-audio-s \
                                        -lsfml-system-s \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm \
                                        -lopenal32 \
                                        -lflac \
                                        -lvorbisenc \
                                        -lvorbisfile \
                                        -lvorbis \
                                        -logg

CONFIG(debug, debug|release): LIBS +=   -lsfml-graphics-s-d \
                                        -lsfml-window-s-d \
                                        -lsfml-audio-s-d \
                                        -lsfml-system-s-d \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm \
                                        -lopenal32 \
                                        -lflac \
                                        -lvorbisenc \
                                        -lvorbisfile \
                                        -lvorbis \
                                        -logg

INCLUDEPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/include
DEPENDPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/include
