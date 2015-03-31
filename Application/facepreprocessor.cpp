#include "facepreprocessor.h"

#include <vector>
#include <algorithm>

#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

FacePreprocessor::FacePreprocessor(std::shared_ptr<cv::CascadeClassifier> faceClassifier, const Mat& input)  throw(std::invalid_argument)
    : input(input),
      result(Mat(input.rows, input.cols, CV_8UC1)),
      faceClassifier(faceClassifier)
{
    if (faceClassifier.get() == nullptr)
        throw std::invalid_argument("faceClassifier");
    else if (faceClassifier->empty())
        throw std::invalid_argument("faceClassifier");
}

unsigned int FacePreprocessor::GetMinSize() const
{
    int minDim = std::min(input.rows, input.cols);
    return static_cast<unsigned int>(minDim / 5.0);
}

Mat FacePreprocessor::Preprocess() throw (NoFaceFoundException)
{
    cvtColor(input, result, CV_BGR2GRAY);
    Mat normalized(result.rows, result.cols, CV_8UC1);
    equalizeHist(result, normalized);

    result = normalized;

    std::vector<Rect> faces;
    Size minSize(GetMinSize(), GetMinSize());
    faceClassifier->detectMultiScale(result, faces, 1.1, 3, 0, minSize);

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
