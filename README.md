FaceGuard 
=======================

This repository contains our coursework for the biometric identification course on the [University of Pannonia](http://www.uni-pannon.hu/).

Our goal was to identify people using a webcam based on a small set of training picture.

The application is written in C++ using the [Qt framework](http://www.qt.io/download-open-source/) and the [OpenCV](http://opencv.org/downloads.html) library.

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

License
-------------

Our application is licensed under the BSD license as it is stated in [LICENSE.TXT](LICENSE.TXT).