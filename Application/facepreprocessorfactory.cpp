#include "facepreprocessorfactory.h"

#include <QString>
#include <QDir>
#include <QCoreApplication>


FacePreprocessorFactory::FacePreprocessorFactory()
{
    classifiers.face = std::make_shared<cv::CascadeClassifier>();
    classifiers.eye = std::make_shared<cv::CascadeClassifier>();
    const QString pathToFaceClassifier = QCoreApplication::applicationDirPath() + QDir::separator() + "haarcascade_frontalface_default.xml";
    const QString pathToEyeClassifier = QCoreApplication::applicationDirPath() + QDir::separator() + "haarcascade_eye.xml";
    classifiers.face->load(pathToFaceClassifier.toStdString());
    classifiers.eye->load(pathToEyeClassifier.toStdString());
}

FacePreprocessor FacePreprocessorFactory::GetPreprocessor(const cv::Mat& image)
{
    FacePreprocessor toRet(classifiers, image);
    return toRet;
}
