#ifndef FACEPREPROCESSOR_H
#define FACEPREPROCESSOR_H

#include <stdexcept>

class NoFaceFoundException : std::logic_error
{
public:
    NoFaceFoundException() : std::logic_error("No face can be found on the given image.") {}
};


#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

class FacePreprocessor final
{
    const cv::Mat& input;
    cv::Mat result;

    cv::CascadeClassifier faceClassifier;
public:
    FacePreprocessor(const cv::Mat& input);
    cv::Mat Preprocess() throw (NoFaceFoundException);
};

#endif // FACEPREPROCESSOR_H
