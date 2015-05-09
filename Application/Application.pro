TARGET = faceguard

CONFIG += stl rtti warn_on c++11

QT += gui widgets core

isEmpty(OPENCV_PATH) {
    OPENCV_PATH = $$PWD/../../opencv
    unix {
        OPENCV_PATH = /usr
    }
}
isEmpty(OPENCV_VERSION) {
    OPENCV_VERSION = 2411
}

win*:OPENCV_PATH = $$OPENCV_PATH/build

DESTDIR = bin

*-msvc* {
    QMAKE_CXXFLAGS += /wd4290

    INCLUDEPATH += $${OPENCV_PATH}/include/

    OPENCV_ARCH_DIR=x86
    contains(QMAKE_TARGET.arch, x86_64):OPENCV_ARCH_DIR=x64

    QMAKE_LIBDIR += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/lib

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
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_core$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_highgui$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_imgproc$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_objdetect$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_contrib$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_flann$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_features2d$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_calib3d$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_ml$${OPENCV_VERSION}d.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_video$${OPENCV_VERSION}d.dll
    }
    Release {
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_core$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_highgui$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_imgproc$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_objdetect$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_contrib$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_flann$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_features2d$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_calib3d$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_ml$${OPENCV_VERSION}.dll
        package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_video$${OPENCV_VERSION}.dll
    }

    INSTALLS += package
}

cascades.path = $$OUT_PWD/$$DESTDIR
cascades.files += $${OPENCV_PATH}/share/opencv/haarcascades/haarcascade_frontalface_default.xml
cascades.files += $${OPENCV_PATH}/share/opencv/haarcascades/haarcascade_eye.xml
cascades.files += $${OPENCV_PATH}/share/opencv/haarcascades/haarcascade_mcs_eyepair_small.xml
cascades.files += $${OPENCV_PATH}/share/opencv/haarcascades/haarcascade_mcs_lefteye.xml
cascades.files += $${OPENCV_PATH}/share/opencv/haarcascades/haarcascade_mcs_righteye.xml
INSTALLS += cascades

win* {
    qt_dll.path = $$OUT_PWD/$$DESTDIR
    Debug {
        qt_dll.files += $$(QTDIR)/bin/Qt5Widgetsd.dll
        qt_dll.files += $$(QTDIR)/bin/Qt5Guid.dll
        qt_dll.files += $$(QTDIR)/bin/Qt5Cored.dll
    }
    Release {
        qt_dll.files += $$(QTDIR)/bin/Qt5Widgets.dll
        qt_dll.files += $$(QTDIR)/bin/Qt5Gui.dll
        qt_dll.files += $$(QTDIR)/bin/Qt5Core.dll
    }
    qt_dll.files += $$(QTDIR)/bin/icuin53.dll
    qt_dll.files += $$(QTDIR)/bin/icuuc53.dll
    qt_dll.files += $$(QTDIR)/bin/icudt53.dll

    INSTALLS += qt_dll
}

unix {
    LIBS += -lopencv_core
    LIBS += -lopencv_highgui
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_objdetect
    LIBS += -lopencv_contrib
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

#target.path = $$OUT_PWD/$$DESTDIR
#INSTALLS += target
