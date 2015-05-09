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

	DatabaseCreator databaseCreatorWidget;
	FaceRecognizerSelectorWidget faceRecognizerSelectorWidget;

	Database database;

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
