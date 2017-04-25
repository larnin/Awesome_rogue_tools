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
    animation.cpp \
    utilities.cpp \
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
    datas.cpp \
    UI/Animation/animationlist.cpp \
    UI/Animation/animationpreview.cpp \
    UI/Animation/animationtextureview.cpp \
    UI/Animation/animationwindow.cpp \
    UI/Animator/animatoranimationlist.cpp \
    UI/Animator/animatorwindow.cpp \
    UI/qsfmlcanvas.cpp \
    UI/mainwindow.cpp \
    UI/Animator/animatorcentralview.cpp

HEADERS  += \
    frame.h \
    animation.h \
    utilities.h \
    ressource.h \
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
    datas.h \
    UI/Animation/animationlist.h \
    UI/Animation/animationpreview.h \
    UI/Animation/animationtextureview.h \
    UI/Animation/animationwindow.h \
    UI/Animator/animatoranimationlist.h \
    UI/Animator/animatorwindow.h \
    UI/mainwindow.h \
    UI/qsfmlcanvas.h \
    UI/Animator/animatorcentralview.h \
    vect2convert.h

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
