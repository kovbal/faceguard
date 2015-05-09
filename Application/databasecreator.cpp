#include "databasecreator.h"
#include "ui_databasecreator.h"

#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QRegExp>
#include <QMap>

#include <opencv2/opencv.hpp>

#include <vector>

#include "facerecognizercontainer.h"

DatabaseCreator::DatabaseCreator(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::DatabaseCreator)
{
    ui->setupUi(this);
}

DatabaseCreator::~DatabaseCreator()
{
	delete ui;
}

void DatabaseCreator::SetDatabase(Database* database)
{
	this->database = database;
}

void DatabaseCreator::on_pushButton_read_browse_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this, tr("Directory"));

    ui->lineEdit_read->setText(str);
}

void DatabaseCreator::on_pushButton_save_browse_clicked()
{
	QString str = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Valami (*.facedb)"));

    ui->lineEdit_save->setText(str);
}

void DatabaseCreator::on_pushButton_create_clicked()
{
	QString directory = ui->lineEdit_read->text();
	QString saveFile = ui->lineEdit_save->text();

	if (!directory.isEmpty() && !saveFile.isEmpty())
	{
		QRegExp fileNameRe("(\\w+_\\w+)_\\d+\\.*");

		QList<QFileInfo> files;

		QDirIterator it(directory, QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);
		while (it.hasNext())
		{
			it.next();

			files.append(it.fileInfo());
		}

		int progressCounter = 0;
		QListIterator<QFileInfo> fileIt(files);
		while (fileIt.hasNext())
		{
			qDebug() << progressCounter << files.size();

			const QFileInfo &fileInfo = fileIt.next();
			++progressCounter;

			if (fileNameRe.indexIn(fileInfo.fileName()) != -1)
			{
				QString name = fileNameRe.cap(1);

				try
				{
					cv::Mat rawImage = cv::imread(fileInfo.filePath().toStdString());

					FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawImage, false);
					cv::Mat preprocessedImage = facePreprocessor.Preprocess();

					QDir dir("");
					dir.rmdir("preprocessed");
					dir.mkdir("preprocessed");
					cv::imwrite(("preprocessed/" + fileInfo.fileName()).toStdString(), preprocessedImage);

					database->AddImage(name, preprocessedImage);
				}
				catch (NoFaceFoundException)
				{
//					qDebug() << "Could not find face" << fileInfo.filePath();
				}
			}
		}

		qDebug() << "Save name labels";
		database->ExportNameLabels(saveFile + ".nlabels");

		qDebug() << "Train face recognizer";
		database->Train();
		qDebug() << "Save face recognizer";
		database->Save(saveFile);
	}
	else
	{
		QMessageBox::critical(this, tr("Unspecified input and output"), tr("Please specify the folder with the images, and the output file of the created database!"));
	}
}
