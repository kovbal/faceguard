FaceGuard   {#mainpage}
=======================

This repository contains our coursework for the biometric identification course on the University of Pannonia.

Our goal is to identify people using a webcam based on a small set of training picture.

The application is written in C++ using the Qt framework and the OpenCV library.

How to compile
---------------

In order to compile the application, the Qt framework and the OpenCV library must be installed.

Under Windows the `OPENCV_PATH` and the `OPENCV_VERSION` qmake variable must be set correctly during the Makefile generation.
These values can be set in Qt Creator under the label _Additional arguments_ (_Projects_ → _Build Settings_ → _Build Steps_ → _qmake_ → _Additional arguments_).
Example values:

 - `OPENCV_PATH=../../opencv`
 - `OPENCV_VERSION=2411`