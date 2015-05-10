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
    databaseCreatorWidget(database, this),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(&databaseCreatorWidget, &DatabaseCreator::finished, this, &MainWindow::ContinueCameraProcessing);
	connect(&faceRecognizerSelectorWidget, &FaceRecognizerSelectorWidget::finished, this, &MainWindow::ContinueCameraProcessing);

	connect(&timer, &QTimer::timeout, this, &MainWindow::UpdateCameraImage);
	timer.start(30);

	camera.reset(new cv::VideoCapture(0));

	ui->label_foundName->clear();
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

            ui->label_foundName->setText(QString("Found: ") + database.FindNameByLabel(label) + QString(", confidence: ") + QString::number(confidence));
            qDebug() << label << database.FindNameByLabel(label) << confidence;
		}
        catch (std::exception e)
		{
//			qDebug() << e.what();
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

                ui->label_foundName->setText(QString("Found: ") + database.FindNameByLabel(label) + QString(", confidence: ") + QString::number(confidence));
                qDebug() << label << database.FindNameByLabel(label) << confidence;
			}
			catch (std::exception e)
			{
//				qDebug() << e.what();
			}
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Face recognizer not selected"), tr("Please select and apply a face recognizer in the \"File/Select face recognizer\" menu!"));
	}
}
