#include "facepreprocessor.h"

using namespace cv;

FacePreprocessor::FacePreprocessor(const Mat& input)
    : input(input)
{

}

Mat FacePreprocessor::Preprocess() throw (NoFaceFoundException)
{
    result = input;
    return result;
}
