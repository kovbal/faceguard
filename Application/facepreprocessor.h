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

#include <memory>

struct FaceClassifiers
{
    std::shared_ptr<cv::CascadeClassifier> face;
    std::shared_ptr<cv::CascadeClassifier> eye;
};

class FacePreprocessor final
{
    static const double MAX_ROTATE_ANGLE;
    const cv::Mat& input;
    cv::Mat normalized;
    cv::Mat rotated;
    cv::Mat result;
    cv::Rect face = cv::Rect(0, 0, 0, 0);
    bool gotEyes = false;

    FaceClassifiers classifiers;

    unsigned int GetMinSize() const;

    double GetRotation(cv::Rect left, cv::Rect right);
    void RotateFace();
    void ScaleFace();

    static cv::Rect LargestRect(const std::vector<cv::Rect>& rects);
 public:
    FacePreprocessor(FaceClassifiers classifiers, const cv::Mat& input) throw(std::invalid_argument);
    cv::Mat Preprocess() throw (NoFaceFoundException);
    double GetAccuracy() const;
};

#endif // FACEPREPROCESSOR_H
