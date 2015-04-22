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

void exportNameLabels(const QString fileName, const QMap<QString, int> &nameLabels)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream stream(&file);

		QMapIterator<QString, int> it(nameLabels);
		while (it.hasNext())
		{
			it.next();

			stream << it.key() << " " << it.value() << "\n";
		}
	}
}

void importNameLabels(const QString fileName, QMap<QString, int> &nameLabels)
{
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);

		QString name;
		int label;

		while (!stream.atEnd())
		{
			stream >> name;
			stream >> label;

			nameLabels[name] = label;
		}
	}
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

		QMap<QString, int> nameLabels;
		int namesCounter = 0;

		std::vector<cv::Mat> trainSrc;
		std::vector<int> trainLabels;

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

				int label = -1;
				QMap<QString, int>::iterator it = nameLabels.find(name);
				if (it == nameLabels.end())
				{
					label = namesCounter;
					nameLabels[name] = namesCounter;
					++namesCounter;
				}
				else
				{
					label = it.value();
				}

				try
				{
					cv::Mat rawImage = cv::imread(fileInfo.filePath().toStdString());

					FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawImage);
					cv::Mat preprocessedImage = facePreprocessor.Preprocess();

					QDir dir("");
					dir.rmdir("preprocessed");
					dir.mkdir("preprocessed");
					cv::imwrite(("preprocessed/" + fileInfo.fileName()).toStdString(), preprocessedImage);

					trainSrc.push_back(preprocessedImage);
					trainLabels.push_back(label);
				}
				catch (NoFaceFoundException)
				{
					qDebug() << "Could not find face" << fileInfo.filePath();
				}
			}
		}

		qDebug() << "Save name labels";
		exportNameLabels(saveFile + ".nlabels", nameLabels);

		qDebug() << "Train face recognizer";
		FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->train(trainSrc, trainLabels);
		qDebug() << "Save face recognizer";
		FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->save(saveFile.toStdString());
	}
	else
	{
		QMessageBox::critical(this, tr("Unspecified input and output"), tr("Please specify the folder with the images, and the output file of the created database!"));
	}
}
