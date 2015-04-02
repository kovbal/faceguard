#include "facepreprocessor.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <cstdint>

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

#include <QDebug>

double FacePreprocessor::GetRotation(cv::Rect left, cv::Rect right)
{
    Point direction;
    direction.x = right.x - left.x;
    direction.y = right.y - left.y;
    double angle = std::atan2(direction.y, direction.x) * 180.0 / PI;
    qDebug () << angle;
    if (std::abs(angle) > MAX_ROTATE_ANGLE)
        angle = 0.0;
    return angle;
}

std::vector<Rect> FacePreprocessor::GetEyesAlternateMethod()
{
    static std::vector<Rect> empty;
    std::vector<Rect> eyes;
    classifiers.eyePair->detectMultiScale(result, eyes, 1.1, 4, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(eyes.empty())
        return empty;
    qDebug() << "eye pair was found";
    rectangle(result, eyes.front(), 1);

    std::vector<Rect> lefts;
    std::vector<Rect> rights;
    Rect cRect = eyes.front();
    cRect.width /= 2;
    classifiers.eyeLeft->detectMultiScale(result(cRect), lefts, 1.1, 4, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(lefts.empty())
    {
        qDebug () << "left wasnt found";
        return empty;
    }
    cRect.x += cRect.width;
    classifiers.eyeLeft->detectMultiScale(result(cRect), rights, 1.1, 4, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(rights.empty())
    {
        qDebug () << "right wasnt found";
        return empty;
    }
    rights.front().x += cRect.width;

    gotEyes = true;

    lefts.front().x += eyes.front().x;
    lefts.front().y += eyes.front().y;
    rights.front().x += eyes.front().x;
    rights.front().y += eyes.front().y;
    rectangle(result, lefts.front(), 1);
    rectangle(result, rights.front(), 1);

    eyes.clear();
    eyes.push_back(lefts.front());
    eyes.push_back(rights.front());
    return eyes;
}

std::vector<Rect> FacePreprocessor::GetEyes()
{
    int maxSize = static_cast<int>(std::ceil(std::max(result.rows, result.cols) / 6.0));
    int minSize = std::max(static_cast<int>(std::floor(std::min(result.rows, result.cols) / 20.0)), 5);
    Rect upperHalfOfFace(0, 0, face.width, static_cast<int>(face.height / 1.8));
    std::vector<Rect> eyes;
    std::vector<Rect> eyePairR;
    //classifiers.eyePair->detectMultiScale(result(upperHalfOfFace), eyePairR, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(eyePairR.empty())
    {
        classifiers.eye->detectMultiScale(result(upperHalfOfFace), eyes, 1.1, 6, 0, Size(minSize, minSize), Size(maxSize, maxSize));
    }
    else
    {
        classifiers.eye->detectMultiScale(result(eyePairR.front()), eyes, 1.1, 6, 0, Size(minSize, minSize), Size(maxSize, maxSize));
        rectangle(result, eyePairR.front(), 255);
    }
    for (auto eye : eyes)
    {
        if(!eyePairR.empty())
        {
            eye.x += eyePairR.front().x;
            eye.y += eyePairR.front().y;
        }
        rectangle(result, eye, 1);
    }
    if(eyes.size() < 2)
    {
        qDebug() << "only" << eyes.size() << "eyes were found";
        eyes.clear();
        return eyes;
    }
    gotEyes = true;

    if(eyes.size() > 2)
    {
        // finding the pair with lowest angle
        std::vector<std::pair<uint16_t, uint16_t>> helper;
        helper.reserve(eyes.size() * eyes.size() - eyes.size());
        for(auto i = eyes.begin(); i != eyes.end(); i++)
        {
            for(auto j = eyes.begin(); j != eyes.end(); j++)
            {
                if (i != j)
                    helper.push_back(std::make_pair(std::distance(eyes.begin(), i), std::distance(eyes.begin(), j)));
            }
        }

        std::sort(helper.begin(), helper.end(), [&eyes](const std::pair<uint16_t, uint16_t>& a, const std::pair<uint16_t, uint16_t>& b)
        {
            double angleA = std::abs(GetRotation(eyes[a.first], eyes[a.second]));
            double angleB = std::abs(GetRotation(eyes[b.first], eyes[b.second]));
            if (angleA == 0.0)
                return false;
            else if (angleB == 0.0)
                return true;
            else
                return angleA < angleB;
        });

        assert(helper.front().first != helper.front().second);

        std::swap(eyes[0], eyes[helper.front().first]);
        std::swap(eyes[1], eyes[helper.front().second]);
    }

    std::sort(eyes.begin(), eyes.begin() + 2, [](const Rect& a, const Rect& b) { return a.x < b.x; });
    eyes.resize(2);
    return eyes;
}

void FacePreprocessor::RotateFace()
{
    std::vector<Rect> eyes = GetEyes();
    if(eyes.size() < 2)
    {
        return;
    }

    double angle = GetRotation(eyes[0], eyes[1]);
    qDebug() << "rotation angle: " << angle;
    if (std::abs(angle) < std::numeric_limits<double>::epsilon() * 3)
        return;

    int size = std::max(result.cols, result.rows);
    int fullSize = std::max(normalized.cols, normalized.rows);
    Point2f pt(size/2.0f + face.x, size/2.0f + face.y);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);

    warpAffine(normalized, rotated, r, Size(fullSize, fullSize), CV_INTER_LANCZOS4);

    result = rotated(face);
}

void FacePreprocessor::ScaleFace()
{
    Rect aspected = face;
    if(face.width < face.height)
    {
        aspected.width = face.height;
    }
    else
    {
        aspected.height = face.width;
    }

    Mat scaled = result.clone();

    copyMakeBorder(scaled, scaled, 0, aspected.height, 0, aspected.width, BORDER_REPLICATE);

    aspected.x = 0;
    aspected.y = 0;

    result = scaled(aspected);
    resize(result, result, Size(512, 512), 0.0, 0.0, CV_INTER_LANCZOS4);
}

double FacePreprocessor::GetAccuracy() const
{
    double accuracy = 0.0;
    if(face.width > 0 && face.height > 0)
        accuracy += 0.5;
    if(gotEyes)
        accuracy += 0.5;
    return accuracy;
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

    static uint64_t cntr = 0;
    static uint64_t cntr2 = 0;

    cntr2++;

    if(!gotEyes)
        qDebug() << "no eyes were found on the face -- this is odd :(" << ++cntr << "/" << cntr2;
    else
        qDebug() << "is rotated? " << !rotated.empty() << (cntr2 - cntr) << "/" << cntr2;

    ScaleFace();

    return result;
}
