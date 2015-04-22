#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/highgui/highgui.hpp>

#include <memory>

#include <QMainWindow>
#include <QTimer>

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

	bool processCamera = true;

	DatabaseCreator databaseCreatorWidget;
	FaceRecognizerSelectorWidget faceRecognizerSelectorWidget;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_pushButton_identify_clicked();
	void on_actionCreate_database_triggered();
	void on_actionSelect_face_recognizer_triggered();

private:
	void ContinueCameraProcessing();
	void UpdateCameraImage();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
