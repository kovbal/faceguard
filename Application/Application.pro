TARGET = faceguard

CONFIG += stl rtti warn_on

QT += gui widgets core

isEmpty(OPENCV_PATH) {
    OPENCV_PATH = $$PWD/../../opencv
}
isEmpty(OPENCV_VERSION) {
    OPENCV_VERSION = 2410
}

DESTDIR = bin

*-msvc* {
    QMAKE_CXXFLAGS += /wd4290

    INCLUDEPATH += $${OPENCV_PATH}/build/include/

    OPENCV_ARCH_DIR=x86
    contains(QMAKE_TARGET.arch, x86_64):OPENCV_ARCH_DIR=x64

    QMAKE_LIBDIR += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/lib

    LIBS += opencv_core$${OPENCV_VERSION}.lib
    LIBS += opencv_highgui$${OPENCV_VERSION}.lib
    LIBS += opencv_imgproc$${OPENCV_VERSION}.lib
    LIBS += opencv_objdetect$${OPENCV_VERSION}.lib

    package.path = $$DESTDIR
    package.files = opencv_core$${OPENCV_VERSION}.dll opencv_highgui$${OPENCV_VERSION}.dll opencv_imgproc$${OPENCV_VERSION}.dll
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml

    INSTALLS += package
}

*-g++* {
    LIBS += -lopencv_core
    LIBS += -lopencv_highgui
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_objdetect
}

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    databasecreator.cpp \
    facepreprocessor.cpp \
    facepreprocessorfactory.cpp

FORMS += \
    mainwindow.ui \
    databasecreator.ui

HEADERS += \
    mainwindow.h \
    databasecreator.h \
    facepreprocessor.h \
    facepreprocessorfactory.h

target.path = $$DESTDIR

INSTALLS += target
