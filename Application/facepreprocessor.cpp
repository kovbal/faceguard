/*M///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015, Balázs Kovács, Gergő Róth
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the University of Pannonia nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//M*/

#include "facepreprocessor.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <cstdint>
#include <cmath>

#include <opencv2/imgproc/imgproc.hpp>

#include <QDebug>

static const double PI = std::atan(1.0) * 4;

using namespace cv;

FacePreprocessor::FacePreprocessor(FaceClassifiers classifiers, const Mat& input, bool markFoundFeatures)  throw(std::invalid_argument)
	: markFoundFeatures(markFoundFeatures),
	  input(input),
      result(Mat(input.rows, input.cols, CV_8UC1)),
	  classifiers(classifiers)
{
    if (classifiers.face.get() == nullptr)
        throw std::invalid_argument("face classifier nullptr");
    else if (classifiers.face->empty())
        throw std::invalid_argument("face classifier");
    else if (classifiers.eye.get() == nullptr)
        throw std::invalid_argument("eye classifier nullptr");
    else if (classifiers.eye->empty())
        throw std::invalid_argument("eye classifier");
    else if (classifiers.eyePair.get() == nullptr)
        throw std::invalid_argument("eyepair classifier nullptr");
    else if (classifiers.eyePair->empty())
        throw std::invalid_argument("eyepair classifier");
    else if (classifiers.eyeLeft.get() == nullptr)
        throw std::invalid_argument("left eye classifier nullptr");
    else if (classifiers.eyeLeft->empty())
        throw std::invalid_argument("left eye classifier");
    else if (classifiers.eyeRight.get() == nullptr)
        throw std::invalid_argument("right eye classifier nullptr");
    else if (classifiers.eyeRight->empty())
        throw std::invalid_argument("right eye classifier");
    else if (input.empty())
        throw std::invalid_argument("input");
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

const double FacePreprocessor::MAX_ROTATE_ANGLE = 25.0;

double FacePreprocessor::GetRotation(cv::Rect left, cv::Rect right)
{
    Point direction;
    direction.x = right.x - left.x;
    direction.y = right.y - left.y;
    double angle = std::atan2(direction.y, direction.x) * 180.0 / PI;
    return angle;
}

std::vector<Rect> FacePreprocessor::GetEyesAlternateMethod()
{
    static std::vector<Rect> empty;
    std::vector<Rect> eyes;
    eyes.reserve(2);
    classifiers.eyePair->detectMultiScale(result, eyes, 1.1, 4, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(eyes.empty())
        return empty;
//    qDebug() << "eye pair was found";
	if (markFoundFeatures)
        rectangle(result, eyes.front(), 1);

    std::vector<Rect> lefts;
    std::vector<Rect> rights;
    Rect cRect = eyes.front();
    cRect.width /= 2;
    classifiers.eyeLeft->detectMultiScale(result(cRect), lefts, 1.1, 4, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(lefts.empty())
    {
//        qDebug () << "left wasnt found";
        return empty;
    }
    cRect.x += cRect.width;
    classifiers.eyeLeft->detectMultiScale(result(cRect), rights, 1.1, 4, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(rights.empty())
    {
//        qDebug () << "right wasnt found";
        return empty;
    }
    rights.front().x += cRect.width;

    gotEyes = true;

    lefts.front().x += eyes.front().x;
    lefts.front().y += eyes.front().y;
    rights.front().x += eyes.front().x;
    rights.front().y += eyes.front().y;
	if (markFoundFeatures)
    {
        rectangle(result, lefts.front(), 1);
        rectangle(result, rights.front(), 1);
    }

    eyes.clear();
    eyes.push_back(lefts.front());
    eyes.push_back(rights.front());
    return eyes;
}

std::vector<Rect> FacePreprocessor::GetEyes()
{
    const Rect upperHalfOfFace(face.width / 12, face.height / 5, face.width - (face.width / 6), static_cast<int>(face.height / 2.2));
    const int maxSize = static_cast<int>(std::ceil(std::max(upperHalfOfFace.width, upperHalfOfFace.height) / 5.0));
    const int minSize = std::max(static_cast<int>(std::floor(std::min(upperHalfOfFace.width, upperHalfOfFace.height) / 20.0)), 3);
    std::vector<Rect> eyes;
    std::vector<Rect> eyePairR;
    //classifiers.eyePair->detectMultiScale(result(upperHalfOfFace), eyePairR, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT);
    if(eyePairR.empty())
    {
        classifiers.eye->detectMultiScale(result(upperHalfOfFace), eyes, 1.05, 6, 0, Size(minSize, minSize), Size(maxSize, maxSize));
    }
    else
    {
        classifiers.eye->detectMultiScale(result(eyePairR.front()), eyes, 1.1, 6, 0, Size(minSize, minSize), Size(maxSize, maxSize));
		if (markFoundFeatures)
            rectangle(result, eyePairR.front(), 255);
    }
    for (auto eye : eyes)
    {
        if(!eyePairR.empty())
        {
            eye.x += eyePairR.front().x;
            eye.y += eyePairR.front().y;
        }
        else
        {
            eye.x += upperHalfOfFace.x;
            eye.y += upperHalfOfFace.y;
        }
		if (markFoundFeatures)
            rectangle(result, eye, 1);
    }

    // removing intersecting objects
    for(uint8_t i = 0; i < eyes.size(); i++)
    {
        for(uint8_t j = 0; j < eyes.size(); j++)
        {
            if (i != j)
            {
                Rect intersection = eyes[i] & eyes[j];
                if (intersection.area() > 0)
                {
                    eyes.erase(eyes.begin() + j);
                    if (i > j)
                        i--;
                    j--;
                }
            }
        }
    }

    if(eyes.size() < 2)
    {
//        qDebug() << "only" << eyes.size() << "eyes were found";
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
    angle = std::round(angle * 10.0) / 10.0;
//    qDebug() << "rotation angle: " << angle;
    if (std::abs(angle) < std::numeric_limits<double>::epsilon() * 3 || std::abs(angle) > MAX_ROTATE_ANGLE)
        return;

    Point2f ptOnFullFrame(face.width / 2.0f + face.x, face.height / 2.0f + face.y);
    //Mat r = getRotationMatrix2D(ptOnFullFrame, angle, 1.0);
    RotatedRect rr(ptOnFullFrame, Size2f(face.width, face.height), angle);
    Rect boundingBox = rr.boundingRect();
    // create border
    {
        int top = -std::min(0, boundingBox.y);
        int bottom = std::max(0, std::max(0, boundingBox.y) + boundingBox.height - normalized.rows);
        int left = -std::min(0, boundingBox.x);
        int right = std::max(0, std::max(0, boundingBox.x) + boundingBox.width - normalized.cols);
//        qDebug () << top << bottom << left << right;
        copyMakeBorder(normalized, normalized, top, bottom, left, right, BORDER_REPLICATE);
        boundingBox.x += left;
        boundingBox.y += top;
    }
    //copyMakeBorder(normalized, normalized, boundingBox.height, boundingBox.height, boundingBox.width, boundingBox.width, BORDER_REPLICATE);

    //warpAffine(normalized, rotated, r, Size(fullSize, fullSize), CV_INTER_LANCZOS4);
    rotated = normalized(boundingBox);

    Mat rotationMatrix = getRotationMatrix2D(Point2f(boundingBox.width / 2.0f, boundingBox.height / 2.0f), angle, 1.0);
    warpAffine(rotated, rotated, rotationMatrix, boundingBox.size(), CV_INTER_LANCZOS4);

    Rect faceTmp = face;
    faceTmp.x = (boundingBox.width - face.width) / 2;
    faceTmp.y = (boundingBox.height - face.height) / 2;
    result = rotated(faceTmp);
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
	resize(result, result, Size(384, 384), 0.0, 0.0, CV_INTER_LANCZOS4);
}

bool FacePreprocessor::IsFaceFound() const
{
    return face.width > 0 && face.height > 0;
}

bool FacePreprocessor::AreEyesFound() const
{
    return gotEyes;
}

bool FacePreprocessor::IsRotated() const
{
    return !rotated.empty();
}

double FacePreprocessor::GetAccuracy() const
{
    double accuracy = 0.0;
    if(IsFaceFound())
        accuracy += 0.5;
    if(AreEyesFound())
        accuracy += 0.25;
    if(IsRotated())
        accuracy += 0.25;
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
    classifiers.face->detectMultiScale(result, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_CANNY_PRUNING, minSize);

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

    ScaleFace();

	return result;

}

bool FacePreprocessor::GetMarkFoundFeatures() const
{
	return markFoundFeatures;
}
