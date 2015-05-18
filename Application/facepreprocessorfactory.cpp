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

#include "facepreprocessorfactory.h"

#include <QString>
#include <QDir>
#include <QCoreApplication>

#include <vector>
#include <memory>

#include <opencv2/objdetect/objdetect.hpp>

FacePreprocessorFactory::FacePreprocessorFactory()
{
    static bool initiated = false;

    if (!initiated)
    {
        initiated = true;

        classifiers.face = std::make_shared<cv::CascadeClassifier>();
        classifiers.eye = std::make_shared<cv::CascadeClassifier>();
        classifiers.eyePair = std::make_shared<cv::CascadeClassifier>();
        classifiers.eyeLeft = std::make_shared<cv::CascadeClassifier>();
        classifiers.eyeRight = std::make_shared<cv::CascadeClassifier>();
        static std::vector<QString> classifierPaths
        {
            "haarcascade_frontalface_default.xml",
            "haarcascade_eye.xml",
            "haarcascade_mcs_eyepair_small.xml",
            "haarcascade_mcs_lefteye.xml",
            "haarcascade_mcs_righteye.xml"
        };
        for (auto& path : classifierPaths)
        {
            path = QCoreApplication::applicationDirPath() + QDir::separator() + path;
        }
        classifiers.face->load(classifierPaths[0].toStdString());
        classifiers.eye->load(classifierPaths[1].toStdString());
        classifiers.eyePair->load(classifierPaths[2].toStdString());
        classifiers.eyeLeft->load(classifierPaths[3].toStdString());
        classifiers.eyeRight->load(classifierPaths[4].toStdString());
    }
}

FacePreprocessor FacePreprocessorFactory::GetPreprocessor(const cv::Mat& image, bool markFoundFeatures)
{
    FacePreprocessor toRet(classifiers, image, markFoundFeatures);
    return toRet;
}
