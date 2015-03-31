#include "facepreprocessorfactory.h"

#include <QString>
#include <QDir>
#include <QCoreApplication>


FacePreprocessorFactory::FacePreprocessorFactory()
    : faceClassifier(new cv::CascadeClassifier())
{
    const QString pathToFaceClassifier = QCoreApplication::applicationDirPath() + QDir::separator() + "haarcascade_frontalface_default.xml";
    faceClassifier->load(pathToFaceClassifier.toStdString());
}

FacePreprocessor FacePreprocessorFactory::GetPreprocessor(const cv::Mat& image)
{
    FacePreprocessor toRet(faceClassifier, image);
    return toRet;
}
