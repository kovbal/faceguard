#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QMap>

#include <vector>

#include <opencv2/opencv.hpp>

class Database
{
	QMap<QString, int> nameLabels;
	int namesCounter = 0;

	std::vector<cv::Mat> trainSrc;
	std::vector<int> trainLabels;

public:
	Database();
	~Database();

	const QMap<QString, int>& GetNameLabels() const;

	void ExportNameLabels(const QString fileName);
	void ImportNameLabels(const QString fileName);

	void AddImage(const QString& label, cv::Mat& image);

	void Train();
	void Save(const QString& filePath);
	void Load(const QString& filePath);

	const QString* FindNameByLabel(int label);
};

#endif // DATABASE_H
