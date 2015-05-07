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
#include <vector>

struct FaceClassifiers
{
    std::shared_ptr<cv::CascadeClassifier> face;
    std::shared_ptr<cv::CascadeClassifier> eye;
    std::shared_ptr<cv::CascadeClassifier> eyePair;
    std::shared_ptr<cv::CascadeClassifier> eyeLeft;
    std::shared_ptr<cv::CascadeClassifier> eyeRight;
};

class FacePreprocessor final
{
	bool markFoundFeatures = true;

    static const double MAX_ROTATE_ANGLE;
    const cv::Mat& input;
    cv::Mat normalized;
    cv::Mat rotated;
    cv::Mat result;
    cv::Rect face = cv::Rect(0, 0, 0, 0);
    bool gotEyes = false;

    FaceClassifiers classifiers;

    unsigned int GetMinSize() const;

    static double GetRotation(cv::Rect left, cv::Rect right);

    std::vector<cv::Rect> GetEyes();
    std::vector<cv::Rect> GetEyesAlternateMethod();

    void RotateFace();
    void ScaleFace();

    static cv::Rect LargestRect(const std::vector<cv::Rect>& rects);
 public:
	FacePreprocessor(FaceClassifiers classifiers, const cv::Mat& input, bool markFoundFeatures) throw(std::invalid_argument);
    cv::Mat Preprocess() throw (NoFaceFoundException);

	bool GetMarkFoundFeatures() const;
    double GetAccuracy() const;
    bool IsFaceFound() const;
    bool AreEyesFound() const;
    bool IsRotated() const;
};

#endif // FACEPREPROCESSOR_H
