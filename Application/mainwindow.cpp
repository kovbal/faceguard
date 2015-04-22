#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QMessageBox>
#include <QImage>
#include <QPicture>

#include "facerecognizercontainer.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(&databaseCreatorWidget, &DatabaseCreator::finished, this, &MainWindow::ContinueCameraProcessing);
	connect(&faceRecognizerSelectorWidget, &FaceRecognizerSelectorWidget::finished, this, &MainWindow::ContinueCameraProcessing);

	connect(&timer, &QTimer::timeout, this, &MainWindow::UpdateCameraImage);
	timer.start(30);

	camera.reset(new cv::VideoCapture(0));
}

MainWindow::~MainWindow()
{
	camera->release();

	delete ui;
}

void MainWindow::on_pushButton_identify_clicked()
{
}

QImage cvMat2QImage(const cv::Mat &image)
{
	// 8-bits unsigned, NO. OF CHANNELS=1
	if (image.type() == CV_8UC1)
	{
		// Set the color table (used to translate colour indexes to qRgb values)
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; ++i)
		{
			colorTable.push_back(qRgb(i, i, i));
		}
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*) image.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_Indexed8);
		img.setColorTable(colorTable);
		return img;
	}
	// 8-bits unsigned, NO. OF CHANNELS=3
	if (image.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*) image.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, image.cols, image.rows, image.step, QImage::Format_RGB888);
		return img.rgbSwapped();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

void MainWindow::UpdateCameraImage()
{
	if (processCamera)
	{
		cv::Mat image;
		*camera >> image;

		QImage img = cvMat2QImage(image);

		QPixmap pixmap;
		pixmap.convertFromImage(img);
		ui->label->setPixmap(pixmap);
	}
}

void MainWindow::on_actionCreate_database_triggered()
{
	if (FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj)
	{
		processCamera = false;
		databaseCreatorWidget.show();
	}
	else
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
}

void MainWindow::on_actionSelect_face_recognizer_triggered()
{
	processCamera = false;
	faceRecognizerSelectorWidget.show();
}

void MainWindow::ContinueCameraProcessing()
{
	processCamera = true;
}
