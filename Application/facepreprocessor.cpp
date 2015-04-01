#include "facepreprocessor.h"

#include <vector>
#include <algorithm>
#include <limits>

#include <opencv2/imgproc/imgproc.hpp>

static const double PI = std::atan(1.0) * 4;

using namespace cv;

FacePreprocessor::FacePreprocessor(FaceClassifiers classifiers, const Mat& input)  throw(std::invalid_argument)
    : input(input),
      result(Mat(input.rows, input.cols, CV_8UC1)),
      classifiers(classifiers)
{
    if (classifiers.face.get() == nullptr)
        throw std::invalid_argument("face classifier");
    else if (classifiers.face->empty())
        throw std::invalid_argument("face classifier");
    else if (classifiers.eye.get() == nullptr)
        throw std::invalid_argument("eye classifier");
    else if (classifiers.eye->empty())
        throw std::invalid_argument("eye classifier");
}

unsigned int FacePreprocessor::GetMinSize() const
{
    int minDim = std::min(input.rows, input.cols);
    return static_cast<unsigned int>(minDim / 5.0);
}

Rect FacePreprocessor::LargestRect(const std::vector<Rect>& rects)
{
    if(rects.empty())
        return Rect();
    Rect largest = *std::max_element(rects.begin(), rects.end(), [](const Rect& a, const Rect& b)
    {
        return a.area() < b.area();
    }
    );
    return largest;
}

const double FacePreprocessor::MAX_ROTATE_ANGLE = 20.0;

double FacePreprocessor::GetRotation(cv::Rect left, cv::Rect right)
{
    Point direction;
    direction.x = right.x - left.x;
    direction.y = right.y - left.y;
    double angle = std::atan2(direction.y, direction.x) * 180.0 / PI;
    if (std::abs(angle) > MAX_ROTATE_ANGLE)
        angle = 0.0;
    return angle;
}

void FacePreprocessor::RotateFace()
{
    int maxSize = static_cast<int>(std::ceil(std::max(result.rows, result.cols) / 6.0));
    int minSize = std::max(static_cast<int>(std::floor(std::min(result.rows, result.cols) / 20.0)), 5);
    Mat tmp;
    equalizeHist(result, tmp);
    std::vector<Rect> eyes;
    classifiers.eye->detectMultiScale(tmp, eyes, 1.1, 6, 0, Size(minSize, minSize), Size(maxSize, maxSize));
    if(eyes.size() < 2)
        return;

    std::sort(eyes.begin(), eyes.end(), [](const Rect& a, const Rect& b){ return a.area() > b.area(); });

    std::sort(eyes.begin(), eyes.begin() + 2, [](const Rect& a, const Rect& b) { return a.x < b.x; });

    double angle = GetRotation(eyes[0], eyes[1]);
    if (std::abs(angle) < std::numeric_limits<double>::epsilon() * 3)
        return;

    int size = std::max(result.cols, result.rows);
    int fullSize = std::max(normalized.cols, normalized.rows);
    Point2f pt(size/2.0f + face.x, size/2.0f + face.y);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(normalized, result, r, Size(fullSize, fullSize));

    result = result(face);
}

Mat FacePreprocessor::Preprocess() throw (NoFaceFoundException)
{
    if(input.type() == CV_8UC3)
        cvtColor(input, result, CV_BGR2GRAY);
    else
        result = input;
    equalizeHist(result, normalized);

    result = normalized;

    std::vector<Rect> faces;
    Size minSize(GetMinSize(), GetMinSize());
    classifiers.face->detectMultiScale(result, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, minSize);

    if (!faces.empty())
    {
        face = LargestRect(faces);

        result = result(face);
    }
    else
    {
        throw NoFaceFoundException();
    }

    RotateFace();

    return result;
}
