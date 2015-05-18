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

#include "facerecognizercontainer.h"

std::shared_ptr<FaceRecognizerContainer> FaceRecognizerContainer::faceRecognizerContainer(new FaceRecognizerContainer);

FaceRecognizerContainer::FaceRecognizerContainer()
{
}

std::shared_ptr<FaceRecognizerContainer>& FaceRecognizerContainer::Instance()
{
    return faceRecognizerContainer;
}

cv::Ptr<cv::FaceRecognizer>& FaceRecognizerContainer::CurrentFaceRecognizer()
{
    return currentFaceRecognizer;
}

void FaceRecognizerContainer::SetEigenFaceRecognizer(int num_components, double threshold)
{
    currentFaceRecognizer = cv::createEigenFaceRecognizer(num_components, threshold);
}

void FaceRecognizerContainer::SetFisherFaceRecognizer(int num_components, double threshold)
{
    currentFaceRecognizer = cv::createFisherFaceRecognizer(num_components, threshold);
}

void FaceRecognizerContainer::SetLBPHFaceRecognizer(int radius, int neighbors, int grid_x, int grid_y, double threshold)
{
    currentFaceRecognizer = cv::createLBPHFaceRecognizer(radius, neighbors, grid_x, grid_y, threshold);
}
