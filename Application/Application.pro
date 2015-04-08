TARGET = faceguard

CONFIG += stl rtti warn_on

QT += gui widgets core

isEmpty(OPENCV_PATH) {
    OPENCV_PATH = $$PWD/../../opencv
}
isEmpty(OPENCV_VERSION) {
    OPENCV_VERSION = 2411
}

DESTDIR = bin

*-msvc* {
    QMAKE_CXXFLAGS += /wd4290

    INCLUDEPATH += $${OPENCV_PATH}/build/include/

    OPENCV_ARCH_DIR=x86
    contains(QMAKE_TARGET.arch, x86_64):OPENCV_ARCH_DIR=x64

    QMAKE_LIBDIR += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/lib

    Debug {
        LIBS += opencv_core$${OPENCV_VERSION}d.lib
        LIBS += opencv_highgui$${OPENCV_VERSION}d.lib
        LIBS += opencv_imgproc$${OPENCV_VERSION}d.lib
        LIBS += opencv_objdetect$${OPENCV_VERSION}d.lib
    }
    Release {
        LIBS += opencv_core$${OPENCV_VERSION}.lib
        LIBS += opencv_highgui$${OPENCV_VERSION}.lib
        LIBS += opencv_imgproc$${OPENCV_VERSION}.lib
        LIBS += opencv_objdetect$${OPENCV_VERSION}.lib
    }

    package.path = $$OUT_PWD/$$DESTDIR
    Debug {
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_core$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_highgui$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_imgproc$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_objdetect$${OPENCV_VERSION}d.dll
    }
    Release {
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_core$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_highgui$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_imgproc$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_objdetect$${OPENCV_VERSION}.dll
    }
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_eye.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_mcs_eyepair_small.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_mcs_lefteye.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_mcs_righteye.xml

    INSTALLS += package
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

TARGET.path = $$OUT_PWD/$$DESTDIR

INSTALLS += target
