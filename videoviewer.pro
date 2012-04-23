TEMPLATE = app

QT += multimedia

HEADERS = \
    src/videoplayer.h \
    src/videowidget.h \
    src/videowidgetsurface.h

SOURCES = \
    src/main.cpp \
    src/videoplayer.cpp \
    src/videowidget.cpp \
    src/videowidgetsurface.cpp

# install
target.path = build
sources.files = $$SOURCES $$HEADERS *.pro *.png images
sources.path = .
INSTALLS += target sources

DEFINES += __STDC_CONSTANT_MACROS

#symbian {
#    TARGET.UID3 = 0xA000D7C3
#    include($$PWD/../../symbianpkgrules.pri)
#}
#maemo5: include($$PWD/../../maemo5pkgrules.pri)
#contains(MEEGO_EDITION,harmattan): include($$PWD/../../harmattanpkgrules.pri)

#symbian: warning(This example might not fully work on Symbian platform)

#unix:!symbian: LIBS += -L$$PWD/../hollywood/build/ -lhollywood

#INCLUDEPATH += $$PWD/../hollywood/src
#DEPENDPATH += $$PWD/../hollywood/src

#unix:!symbian: PRE_TARGETDEPS += $$PWD/../hollywood/build/libhollywood.a

#unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-ffmpeg/build/linux/lib/ -lavcodec

#INCLUDEPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include
#DEPENDPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include

##unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-ffmpeg/build/linux/lib/ -lavfilter

##INCLUDEPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include
##DEPENDPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include

#unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-ffmpeg/build/linux/lib/ -lavformat

#INCLUDEPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include
#DEPENDPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include

#unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-ffmpeg/build/linux/lib/ -lavutil

#INCLUDEPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include
#DEPENDPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include

#unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-ffmpeg/build/linux/lib/ -lswscale

#INCLUDEPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include
#DEPENDPATH += $$PWD/../steinwurf/external-ffmpeg/build/linux/include

#unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-boost/build/linux/ -lboost_system

#INCLUDEPATH += $$PWD/../steinwurf/external-boost/boost
#DEPENDPATH += $$PWD/../steinwurf/external-boost/boost

#unix:!symbian: PRE_TARGETDEPS += $$PWD/../steinwurf/external-boost/build/linux/libboost_system.a

#unix:!symbian: LIBS += -L$$PWD/../steinwurf/external-boost/build/linux/ -lboost_thread

#INCLUDEPATH += $$PWD/../steinwurf/external-boost/build/linux
#DEPENDPATH += $$PWD/../steinwurf/external-boost/build/linux

#unix:!symbian: PRE_TARGETDEPS += $$PWD/../steinwurf/external-boost/build/linux/libboost_thread.a
