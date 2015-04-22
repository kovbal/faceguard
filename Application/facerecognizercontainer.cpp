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
