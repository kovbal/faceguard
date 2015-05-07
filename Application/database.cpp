#include "database.h"

#include <QFile>
#include <QTextStream>

#include "facerecognizercontainer.h"

Database::Database()
{
}

Database::~Database()
{
}

const QMap<QString, int>&Database::GetNameLabels() const
{
	return nameLabels;
}

void Database::ExportNameLabels(const QString fileName)
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

void Database::AddImage(const QString& label, cv::Mat& image)
{
	int tmp = -1;
	QMap<QString, int>::iterator it = nameLabels.find(label);
	if (it == nameLabels.end())
	{
		tmp = namesCounter;
		nameLabels[label] = namesCounter;
		++namesCounter;
	}
	else
	{
		tmp = it.value();
	}

	trainSrc.push_back(image);
	trainLabels.push_back(tmp);
}

void Database::Train()
{
	FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->train(trainSrc, trainLabels);
}

void Database::Save(const QString& filePath)
{
	FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->save(filePath.toStdString());
}

void Database::Load(const QString& filePath)
{
	FaceRecognizerContainer::Instance()->CurrentFaceRecognizer().obj->load(filePath.toStdString());
}

void Database::ImportNameLabels(const QString fileName)
{
	nameLabels.clear();

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
