TARGET = faceguard

CONFIG += stl rtti warn_on
CONFIG += c++11

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
        LIBS += opencv_contrib$${OPENCV_VERSION}d.lib
#        LIBS += opencv_flann$${OPENCV_VERSION}d.lib
#        LIBS += opencv_features2d$${OPENCV_VERSION}d.lib
#        LIBS += opencv_calib3d$${OPENCV_VERSION}d.lib
#        LIBS += opencv_ml$${OPENCV_VERSION}d.lib
#        LIBS += opencv_video$${OPENCV_VERSION}d.lib
    }
    Release {
        LIBS += opencv_core$${OPENCV_VERSION}.lib
        LIBS += opencv_highgui$${OPENCV_VERSION}.lib
        LIBS += opencv_imgproc$${OPENCV_VERSION}.lib
        LIBS += opencv_objdetect$${OPENCV_VERSION}.lib
        LIBS += opencv_contrib$${OPENCV_VERSION}.lib
#        LIBS += opencv_flann$${OPENCV_VERSION}.lib
#        LIBS += opencv_features2d$${OPENCV_VERSION}.lib
#        LIBS += opencv_calib3d$${OPENCV_VERSION}.lib
#        LIBS += opencv_ml$${OPENCV_VERSION}.lib
#        LIBS += opencv_video$${OPENCV_VERSION}.lib
    }

    package.path = $$OUT_PWD/$$DESTDIR
    Debug {
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_core$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_highgui$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_imgproc$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_objdetect$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_contrib$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_flann$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_features2d$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_calib3d$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_ml$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_video$${OPENCV_VERSION}d.dll
    }
    Release {
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_core$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_highgui$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_imgproc$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_objdetect$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_contrib$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_flann$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_features2d$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_calib3d$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_ml$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/build/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_video$${OPENCV_VERSION}.dll
    }
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_eye.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_mcs_eyepair_small.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_mcs_lefteye.xml
    package.files += $${OPENCV_PATH}/build/share/OpenCV/haarcascades/haarcascade_mcs_righteye.xml

    INSTALLS += package

    qt_dll.path = $$OUT_PWD/$$DESTDIR
    Debug {
    }
    Release {
        qt_dll.files += $$(QTDIR)/bin/Qt5Widgets.dll
        qt_dll.files += $$(QTDIR)/bin/Qt5Gui.dll
        qt_dll.files += $$(QTDIR)/bin/Qt5Core.dll

        qt_dll.files += $$(QTDIR)/bin/icuin53.dll
        qt_dll.files += $$(QTDIR)/bin/icuuc53.dll
        qt_dll.files += $$(QTDIR)/bin/icudt53.dll
    }

    INSTALLS += qt_dll
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    databasecreator.cpp \
    facepreprocessor.cpp \
    facepreprocessorfactory.cpp \
    facerecognizerselectorwidget.cpp \
    facerecognizercontainer.cpp \
    database.cpp

FORMS += \
    mainwindow.ui \
    databasecreator.ui \
    facerecognizerselectorwidget.ui

HEADERS += \
    mainwindow.h \
    databasecreator.h \
    facepreprocessor.h \
    facepreprocessorfactory.h \
    facerecognizerselectorwidget.h \
    facerecognizercontainer.h \
    database.h

TARGET.path = $$OUT_PWD/$$DESTDIR

INSTALLS += target
