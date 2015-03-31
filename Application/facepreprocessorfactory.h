#ifndef FACEPREPROCESSORFACTORY_H
#define FACEPREPROCESSORFACTORY_H

#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "facepreprocessor.h"

class FacePreprocessorFactory final
{
    std::shared_ptr<cv::CascadeClassifier> faceClassifier;
public:
    FacePreprocessorFactory();

    FacePreprocessor GetPreprocessor(const cv::Mat& image);
};

#endif // FACEPREPROCESSORFACTORY_H