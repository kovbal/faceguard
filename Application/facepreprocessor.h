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
    bool markFoundFeatures;

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
