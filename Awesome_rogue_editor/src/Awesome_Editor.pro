#-------------------------------------------------
#
# Project created by QtCreator 2015-12-24T18:44:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sans_titre
TEMPLATE = app


SOURCES += main.cpp \
    Gui/mainwindow.cpp \
    Gui/qsfmlcanvas.cpp \
    Map/block.cpp \
    Map/blocktype.cpp \
    Map/patern.cpp \
    Map/roomrender.cpp \
    Gui/Dock/unclosabledock.cpp \
    Utilities/quadrender.cpp \
    Gui/Dock/blockview.cpp \
    Gui/Dock/oneblockview.cpp \
    Gui/Dock/blockdock.cpp \
    Gui/Dock/paternsdock.cpp \
    Gui/centralview.cpp \
    Gui/grid.cpp \
    Gui/cursorblockrender.cpp \
    Utilities/undoinfos.cpp \
    Gui/newpaternwidget.cpp \
    Gui/selectorblockrender.cpp \
    Map/hitbox.cpp \
    Gui/TileConfig/tileconfigwidget.cpp \
    Gui/TileConfig/textureviewer.cpp \
    Utilities/configs.cpp \
    Gui/TileConfig/tileconfig.cpp \
    Gui/TileConfig/tileviewer.cpp \
    Gui/Dock/lightsdock.cpp \
    Map/light.cpp

HEADERS += \
    Gui/mainwindow.h \
    Gui/qsfmlcanvas.h \
    Map/block.h \
    Map/blocktype.h \
    Map/patern.h \
    Map/roomrender.h \
    Gui/Dock/unclosabledock.h \
    Utilities/matrix.h \
    Utilities/noncopiable.h \
    Utilities/quadrender.h \
    Utilities/ressource.h \
    Utilities/vect2convert.h \
    Gui/Dock/blockview.h \
    Gui/Dock/oneblockview.h \
    Gui/Dock/blockdock.h \
    Gui/Dock/paternsdock.h \
    Gui/centralview.h \
    Gui/grid.h \
    Gui/cursorblockrender.h \
    Utilities/undoinfos.h \
    Gui/newpaternwidget.h \
    Gui/selectorblockrender.h \
    Map/hitbox.h \
    Gui/TileConfig/tileconfigwidget.h \
    Gui/TileConfig/textureviewer.h \
    Utilities/configs.h \
    Gui/TileConfig/tileconfig.h \
    Gui/TileConfig/tileviewer.h \
    Gui/Dock/lightsdock.h \
    Map/light.h

DEFINES += SFML_STATIC

CONFIG += c++14
#QMAKE_LFLAGS += -static-libgcc -static-libstdc++

#   --- HOME
#LIBS += -LC:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/lib
#INCLUDEPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/include
#DEPENDPATH += C:/Users/Nicolas/Programation/c++/SFML/DW2_2.4_(Qt)/include

#   --- RUBIKA
LIBS += -LC:/Users/n.laurent/Desktop/perso/SFML/SFML-2.4.0-SW2/lib
INCLUDEPATH += C:/Users/n.laurent/Desktop/perso/SFML/SFML-2.4.0-SW2/include
DEPENDPATH += C:/Users/n.laurent/Desktop/perso/SFML/SFML-2.4.0-SW2/include

CONFIG(release, debug|release): LIBS += -lsfml-graphics-s \
                                        -lsfml-window-s \
                                        -lsfml-system-s \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm

CONFIG(debug, debug|release): LIBS +=   -lsfml-graphics-s-d \
                                        -lsfml-window-s-d \
                                        -lsfml-system-s-d \
                                        -lfreetype \
                                        -lgdi32 \
                                        #-lglew \
                                        -ljpeg \
                                        -lopengl32 \
                                        -lwinmm


