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
