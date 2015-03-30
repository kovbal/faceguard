
QT += gui widgets core

OPENCV_PATH = $$PWD/../../opencv
OPENCV_VERSION = 2410

*-msvc* {
    INCLUDEPATH += $${OPENCV_PATH}/build/include/

    QMAKE_LIBDIR += $${OPENCV_PATH}/build/x86/vc12/lib

    LIBS += opencv_core$${OPENCV_VERSION}.lib
    LIBS += opencv_highgui$${OPENCV_VERSION}.lib
    LIBS += opencv_imgproc$${OPENCV_VERSION}.lib
}

*g++* {
    LIBS += -lopencv_core
    LIBS += -lopencv_highgui
    LIBS += -lopencv_imgproc
}

SOURCES += \
        main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h
