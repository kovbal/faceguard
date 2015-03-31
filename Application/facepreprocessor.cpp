#include "facepreprocessor.h"

#include <vector>
#include <algorithm>

#include <QString>
#include <QCoreApplication>
#include <QDir>

#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

FacePreprocessor::FacePreprocessor(const Mat& input)
    : input(input),
      result(Mat(input.rows, input.cols, CV_8UC1))
{
    const QString pathToFaceClassifier = QCoreApplication::applicationDirPath() + QDir::separator() + "haarcascade_frontalface_default.xml";
    faceClassifier.load(pathToFaceClassifier.toStdString());
}

Mat FacePreprocessor::Preprocess() throw (NoFaceFoundException)
{
    cvtColor(input, result, CV_BGR2GRAY);
    Mat normalized(result.rows, result.cols, CV_8UC1);
    normalize(result, normalized, 0.0, 255.0, NORM_MINMAX);

    result = normalized;

    std::vector<Rect> faces;
    faceClassifier.detectMultiScale(result, faces);

    if (!faces.empty())
    {

        Rect largestFace = *std::max_element(faces.begin(), faces.end(), [](const Rect& a, const Rect& b)
        {
            return a.area() < b.area();
        }
        );

        result = result(largestFace);
    }
    else
    {
        throw NoFaceFoundException();
    }

    return result;
}
