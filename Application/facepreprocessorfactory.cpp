#include "facepreprocessorfactory.h"

#include <QString>
#include <QDir>
#include <QCoreApplication>

#include <vector>
#include <memory>

#include <opencv2/objdetect/objdetect.hpp>

FacePreprocessorFactory::FacePreprocessorFactory()
{
    classifiers.face = std::make_shared<cv::CascadeClassifier>();
    classifiers.eye = std::make_shared<cv::CascadeClassifier>();
    classifiers.eyePair = std::make_shared<cv::CascadeClassifier>();
    classifiers.eyeLeft = std::make_shared<cv::CascadeClassifier>();
    classifiers.eyeRight = std::make_shared<cv::CascadeClassifier>();
    static std::vector<QString> classifierPaths
    {
        "haarcascade_frontalface_default.xml",
        "haarcascade_eye.xml",
        "haarcascade_mcs_eyepair_small.xml",
        "haarcascade_mcs_lefteye.xml",
        "haarcascade_mcs_righteye.xml"
    };
    for(auto path : classifierPaths)
    {
        path = QCoreApplication::applicationDirPath() + QDir::separator() + path;
    }
    classifiers.face->load(classifierPaths[0].toStdString());
    classifiers.eye->load(classifierPaths[1].toStdString());
    classifiers.eyePair->load(classifierPaths[2].toStdString());
    classifiers.eyeLeft->load(classifierPaths[3].toStdString());
    classifiers.eyeRight->load(classifierPaths[4].toStdString());
}

FacePreprocessor FacePreprocessorFactory::GetPreprocessor(const cv::Mat& image)
{
    FacePreprocessor toRet(classifiers, image);
    return toRet;
}
