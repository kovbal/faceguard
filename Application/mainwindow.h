/*M///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015, Balázs Kovács, Gergő Róth
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the University of Pannonia nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//M*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/highgui/highgui.hpp>

#include <memory>

#include <QMainWindow>
#include <QTimer>

#include "database.h"
#include "facerecognizerselectorwidget.h"
#include "databasecreator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	std::unique_ptr<cv::VideoCapture> camera;

	QTimer timer;

	FacePreprocessorFactory preprocessorFactory;

	cv::Mat rawCameraImage;
	bool processCamera = true;

    Database database;

	DatabaseCreator databaseCreatorWidget;
	FaceRecognizerSelectorWidget faceRecognizerSelectorWidget;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_actionCreate_database_triggered();
	void on_actionSelect_face_recognizer_triggered();
	void on_pushButton_addToDatabasePlan_clicked();
	void on_pushButton_createDatabaseFromPlan_clicked();
	void on_pushButton_freezeImage_clicked();
	void on_actionLoad_database_triggered();
	void on_pushButton_identifyCamera_clicked();
	void on_pushButton_identifyImage_clicked();

private:
	void ContinueCameraProcessing();
	void UpdateCameraImage();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
