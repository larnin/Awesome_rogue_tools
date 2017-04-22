#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T16:18:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Awesome_rogue_animator
TEMPLATE = app


SOURCES += main.cpp\
    qsfmlcanvas.cpp \
    animationlist.cpp \
    animation.cpp \
    utilities.cpp \
    animationwindow.cpp \
    animationpreview.cpp \
    animationtextureview.cpp \
    quadrender.cpp \
    transition.cpp \
    Conditions/andcondition.cpp \
    Conditions/animationfinishedcondition.cpp \
    Conditions/condition.cpp \
    Conditions/notcondition.cpp \
    Conditions/orcondition.cpp \
    Conditions/propertyequalcondition.cpp \
    Conditions/propertyinferiorcondition.cpp \
    Conditions/propertyinferiororequalcondition.cpp \
    Conditions/propertynotequalcondition.cpp \
    Conditions/propertysuperiorcondition.cpp \
    Conditions/propertysuperiororequalcondition.cpp \
    Conditions/waitafteractivecondition.cpp \
    Conditions/waitcondition.cpp \
    datas.cpp

HEADERS  += \
    qsfmlcanvas.h \
    animationlist.h \
    frame.h \
    animation.h \
    utilities.h \
    animationwindow.h \
    animationpreview.h \
    ressource.h \
    animationtextureview.h \
    quadrender.h \
    transition.h \
    Conditions/andcondition.h \
    Conditions/animationfinishedcondition.h \
    Conditions/condition.h \
    Conditions/notcondition.h \
    Conditions/orcondition.h \
    Conditions/propertyequalcondition.h \
    Conditions/propertyinferiorcondition.h \
    Conditions/propertyinferiororequalcondition.h \
    Conditions/propertynotequalcondition.h \
    Conditions/propertysuperiorcondition.h \
    Conditions/propertysuperiororequalcondition.h \
    Conditions/waitafteractivecondition.h \
    Conditions/waitcondition.h \
    datas.h

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
