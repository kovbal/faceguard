FaceGuard 
=======================

This repository contains our coursework for the biometric identification course on the [University of Pannonia](http://www.uni-pannon.hu/).

Our goal was to identify people using a webcam based on a small set of training picture.

The application is written in C++ using the [Qt framework](http://www.qt.io/download-open-source/) and the [OpenCV](http://opencv.org/downloads.html) library.
It was tested under Windows 8.1 and Fedora 19.

How to compile
---------------

In order to compile the application, the [Qt framework](http://www.qt.io/download-open-source/) and the [OpenCV](http://opencv.org/downloads.html) library must be installed.

Under Windows the `OPENCV_PATH` and the `OPENCV_VERSION` qmake variable must be set correctly during the Makefile generation.
These values can be set in Qt Creator under the label _Additional arguments_ (_Projects_ → _Build Settings_ → _Build Steps_ → _qmake_ → _Additional arguments_).
Example values:
 - `OPENCV_PATH=../../opencv`
 - `OPENCV_VERSION=2411`

After compilation, the following files must be placed in the executable's directory:
- `haarcascade_frontalface_default.xml`
- `haarcascade_eye.xml`
- `haarcascade_mcs_eyepair_small.xml`
- `haarcascade_mcs_lefteye.xml`
- `haarcascade_mcs_righteye.xml`

The generated Makefile's _install_ target copies all neccessary files to the executable's directory, including the aforementioned XMLs and the required Qt and OpenCV DLLs.
The easiest way to run the _install_ step is to add a "custom build step" to Qt Creator (_Projects_ → _Build Steps_ → _Add Build Step_ → _"Make arguments":_ install).

License
-------------

Our application is licensed under the BSD license as it is stated in [LICENSE.txt](LICENSE.txt).