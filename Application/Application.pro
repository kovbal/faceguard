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

used_opencv_libs = core highgui imgproc objdetect contrib
*-msvc* {
    QMAKE_CXXFLAGS += /wd4290

    INCLUDEPATH += $${OPENCV_PATH}/include/

    OPENCV_ARCH_DIR=x86
    contains(QMAKE_TARGET.arch, x86_64):OPENCV_ARCH_DIR=x64

    !win*-msvc2013: warning("OpenCV libraries might be wrong (other version than VC12)")

    QMAKE_LIBDIR += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/lib

    for (opencv_lib, used_opencv_libs) {
        Debug: LIBS += opencv_$${opencv_lib}$${OPENCV_VERSION}d.lib
        Release: LIBS += opencv_$${opencv_lib}$${OPENCV_VERSION}.lib
    }

    required_opencv_libs = core highgui imgproc objdetect contrib flann features2d calib3d ml video
    package.path = $$OUT_PWD/$$DESTDIR
    for (opencv_lib, required_opencv_libs) {
        Debug: package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_$${opencv_lib}$${OPENCV_VERSION}d.dll
        Release: package.files += $${OPENCV_PATH}/$${OPENCV_ARCH_DIR}/vc12/bin/opencv_$${opencv_lib}$${OPENCV_VERSION}.dll
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
    # Qt uses an Unicode handler library, named ICU, which is made up of multiple DLLs
    # the problem with it is that each Qt version uses a different version, so their name change quite often
    # e.g. icuin52.dll -> icuin53.dll
    # so, we have this little "code" that always picks which is needed
    unicodeSupportDlls = icuin icuuc icudt
    for (dll, unicodeSupportDlls) {
        dllPath = $$files($$[QT_INSTALL_PREFIX]/bin/$${dll}??.dll)
        qt_dll.files += $$dllPath
    }

    # the next section will add every Qt DLL which we use:

    # a Qt module may be defined multiple times, so this is needed
    uniqueQtModules = $$unique(QT)

    # if we are building a release version, the release DLLs are needed for each module
    CONFIG(release, debug|release) {
        for(qtmodule, uniqueQtModules) {
            # the "Test" module's binary is named "testlib"...
            equals(qtmodule, "testlib") {
                qt_dll.files += $$[QT_INSTALL_PREFIX]/bin/Qt$${QT_MAJOR_VERSION}Test.dll
            } else {
                # every other module is simple:
                qt_dll.files += $$[QT_INSTALL_PREFIX]/bin/Qt$${QT_MAJOR_VERSION}$${qtmodule}.dll
            }
        }
    }

    # if we are building a debug build, debug DLLs are needed (note the "d" at the end)
    CONFIG(debug, debug|release) {
        for(qtmodule, uniqueQtModules) {
            # the "Test" module's binary is named "testlib"...
            equals(qtmodule, "testlib") {
                qt_dll.files += $$[QT_INSTALL_PREFIX]/bin/Qt$${QT_MAJOR_VERSION}Testd.dll
            } else {
                # every other module is simple:
                qt_dll.files += $$[QT_INSTALL_PREFIX]/bin/Qt$${QT_MAJOR_VERSION}$${qtmodule}d.dll
            }
        }
    }

    # platform plugins must be placed in the "platforms" directory, so a separate entry is needed
    platform_plugins.path = $$OUT_PWD/$$DESTDIR/platforms
    CONFIG(release, debug|release) {
        platform_plugins.files += $$[QT_INSTALL_PREFIX]/plugins/platforms/qwindows.dll
    }

    CONFIG(debug, debug|release) {
        platform_plugins.files += $$[QT_INSTALL_PREFIX]/plugins/platforms/qwindowsd.dll
    }

    INSTALLS += qt_dll
    INSTALLS += platform_plugins
}

unix {
    for (opencv_lib, used_opencv_libs) {
        LIBS += -lopencv_$${opencv_lib}
    }
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
