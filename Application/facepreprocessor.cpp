#include "facepreprocessor.h"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

FacePreprocessor::FacePreprocessor(const Mat& input)
    : input(input),
      result(Mat(input.rows, input.cols, CV_8UC1))
{

}

Mat FacePreprocessor::Preprocess() throw (NoFaceFoundException)
{
    cvtColor(input, result, CV_BGR2GRAY);
    Mat normalized(result.rows, result.cols, CV_8UC1);
    normalize(result, normalized, 0.0, 255.0, NORM_MINMAX);

    return result;
}
