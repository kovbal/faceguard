#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QPicture>

#include "facerecognizercontainer.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	databaseCreatorWidget.SetDatabase(&database);

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
		*camera >> rawCameraImage;

		bool isPreprocessSuccess = false;

		try
		{
			FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawCameraImage, true);
			cv::Mat preprocessedImage = facePreprocessor.Preprocess();

			QImage img = cvMat2QImage(preprocessedImage);
			QPixmap pixmap;
			pixmap.convertFromImage(img);
			ui->label->setPixmap(pixmap);

			isPreprocessSuccess = true;
		}
		catch (std::exception e)
		{
//			qDebug() << e.what();
		}
		catch (NoFaceFoundException)
		{
		}

		if (!isPreprocessSuccess)
		{
			QImage img = cvMat2QImage(rawCameraImage);
			QPixmap pixmap;
			pixmap.convertFromImage(img);
			ui->label->setPixmap(pixmap);
		}
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

void MainWindow::on_pushButton_addToDatabasePlan_clicked()
{
	try
	{
		FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawCameraImage, false);
		cv::Mat preprocessedImage = facePreprocessor.Preprocess();

		database.AddImage(ui->lineEdit_label->text(), preprocessedImage);
	}
	catch (std::exception e)
	{
//		qDebug() << e.what();
	}
	catch (NoFaceFoundException)
	{
	}
}

void MainWindow::on_pushButton_createDatabaseFromPlan_clicked()
{
	if (FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj)
	{
		QString saveFile = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("fdb (*.facedb)"));;

		if (!saveFile.isEmpty())
		{
			qDebug() << "Save name labels";
			database.ExportNameLabels(saveFile + ".nlabels");

			qDebug() << "Train face recognizer";
			database.Train();
			qDebug() << "Save face recognizer";
			database.Save(saveFile);
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
}

void MainWindow::on_pushButton_freezeImage_clicked()
{
	processCamera = !processCamera;
}

void MainWindow::on_actionLoad_database_triggered()
{
	if (FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj)
	{
		QString loadFile = QFileDialog::getOpenFileName(this, tr("Save file"), tr(""), tr("fdb (*.facedb)"));;

		if (!loadFile.isEmpty())
		{
			database.ImportNameLabels(loadFile + ".nlabels");
			database.Load(loadFile);
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
}

void MainWindow::on_pushButton_identifyCamera_clicked()
{
	if (FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj == nullptr)
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
	else if (database.GetNameLabels().empty())
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
	else
	{
		try
		{
			FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawCameraImage, false);
			cv::Mat preprocessedImage = facePreprocessor.Preprocess();

			int label = -1;
			double confidence = 0.0;
			FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->predict(preprocessedImage, label, confidence);

			qDebug() << label << *database.FindNameByLabel(label) << confidence;
		}
		catch (std::exception e)
		{
//			qDebug() << e.what();
		}
		catch (NoFaceFoundException)
		{
		}
	}
}

void MainWindow::on_pushButton_identifyImage_clicked()
{
	if (FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj)
	{
		QString loadFile = QFileDialog::getOpenFileName(this, tr("Save file"), tr(""), tr("Image (*.jpg *.png *.jpeg)"));;

		if (!loadFile.isEmpty())
		{
			try
			{
				cv::Mat rawImage = cv::imread(loadFile.toStdString().c_str());

				FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawImage, false);
				cv::Mat preprocessedImage = facePreprocessor.Preprocess();

				int label = -1;
				double confidence = 0.0;
				FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->predict(preprocessedImage, label, confidence);

				qDebug() << label << *database.FindNameByLabel(label) << confidence;
			}
			catch (std::exception e)
			{
	//			qDebug() << e.what();
			}
			catch (NoFaceFoundException)
			{
			}
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
}
