#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}

//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

//using namespace cv;

//int main(int argc, char *argv[])
//{
////	char *image_name = argv[1];

////	Mat image;
////	image = imread(image_name, 1);

////	if (argc != 2 || !image.data)
////	{
////		printf("No image data \n");
////		return -1;
////	}

////	Mat gray_image;
////	cvtColor(image, gray_image, CV_BGR2GRAY);

////	imwrite("Gray_Image.jpg", gray_image);

////	namedWindow(image_name, CV_WINDOW_AUTOSIZE);
////	namedWindow("Gray image", CV_WINDOW_AUTOSIZE);

////	imshow(image_name, image);
////	imshow("Gray image", gray_image);

//    VideoCapture cap(0);

//	if (!cap.isOpened())
//	{
//		return 1;
//	}

//	Mat edges;
//	namedWindow("edges", 1);

//	while (true)
//	{
//		Mat frame;
//		cap >> frame;
//		cvtColor(frame, edges, CV_BGR2GRAY);
//		GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
//		Canny(edges, edges, 0, 30, 3);
//		imshow("edges", edges);

//		if (waitKey(30) >= 0)
//		{
//			break;
//		}
//	}

//	return 0;
//}
