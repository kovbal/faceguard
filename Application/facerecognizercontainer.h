#ifndef FACERECOGNIZERCONTAINER_H
#define FACERECOGNIZERCONTAINER_H

#include <opencv2/opencv.hpp>

#include <memory>

class FaceRecognizerContainer
{
	static std::shared_ptr<FaceRecognizerContainer> faceRecognizerContainer;

	cv::Ptr<cv::FaceRecognizer> currentFaceRecognizer;

	FaceRecognizerContainer();

public:
	static std::shared_ptr<FaceRecognizerContainer>& Instance();

	cv::Ptr<cv::FaceRecognizer>& CurrentFaceRecognizer();

	void SetEigenFaceRecognizer(int num_components = 0, double threshold = DBL_MAX);
	void SetFisherFaceRecognizer(int num_components = 0, double threshold = DBL_MAX);
	void SetLBPHFaceRecognizer(int radius = 1, int neighbors = 8, int grid_x = 8, int grid_y = 8, double threshold = DBL_MAX);
};

#endif // FACERECOGNIZERCONTAINER_H
