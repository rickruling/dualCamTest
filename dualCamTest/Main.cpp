#include "vishnuhf1.h"
#include "v_imagelogger.h"
//#include "v_iptools.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iomanip>

#define delay for(int i = 0;i<20000;i++){for(int j=0;j<20000;j++);}

using namespace cv;
using namespace std;

// image variables
Mat src1, src2, src_gray1, src_gray2;
vector<Point2f> corners1, corners2;

char* source_window1 = "Camera 1";
char* source_window2 = "Camera 2";

//corner detection variables
int maxCorners = 25;
int maxTrackbar = 50;
double qualityLevel = 0.01;
double minDistance = 10;
int blockSize = 3;
bool useHarrisDetector = false;
double k = 0.04;
int c = 0;
//int cornerTest();
int SingleCamTest();
int DualCamTest();
int CornerLoop(char* windowName, Mat frame, vector<Point2f> points[2]);
int getVideo();
int main2(){
	//getVideo();
	//cornerTest();
	//getchar();
	return 0;
}

int SingleCamTest(){
	//initializing logger
	Log log("C:/Users/vishnu/Documents/Visual Studio 2013/Projects/file2.txt");
	CvCapture* capture1 = cvCaptureFromCAM(0);
	clock_t begin_time;
	namedWindow(source_window1, CV_WINDOW_AUTOSIZE);
	while (true) {
		begin_time = clock();
		src1 = cvQueryFrame(capture1);
		cvtColor(src1, src_gray1, CV_BGR2GRAY);
		goodFeaturesToTrack(src_gray1, corners1, maxCorners, qualityLevel, minDistance,
			Mat(), blockSize, useHarrisDetector, k);
		log.markPoints(src1, corners1);
		imshow(source_window1, src1);
		cout << "Time Taken : " << float(clock() - begin_time) << endl;
		c = cvWaitKey(1);
		if (c == 27)
			break;
	}
	cvReleaseCapture(&capture1);
	cvDestroyWindow(source_window1);
	getchar();
	return 0;
}
int DualCamTest(){
	//initializing logger
	Log log("C:/Users/vishnu/Documents/Visual Studio 2013/Projects/file2.txt");
	CvCapture* capture1 = cvCaptureFromCAM(0);
	CvCapture* capture2 = cvCaptureFromCAM(1);
	clock_t begin_time;
	namedWindow(source_window1, CV_WINDOW_AUTOSIZE);
	namedWindow(source_window2, CV_WINDOW_AUTOSIZE);
	while (true) {
		log.write("\nWhile loop Starts\n");
		begin_time = clock();
		src1 = cvQueryFrame(capture1);
		src2 = cvQueryFrame(capture2);
		cvtColor(src1, src_gray1, CV_BGR2GRAY);
		cvtColor(src2, src_gray2, CV_BGR2GRAY);
		goodFeaturesToTrack(src_gray1, corners1, maxCorners, qualityLevel, minDistance,
			Mat(), blockSize, useHarrisDetector, k);
		goodFeaturesToTrack(src_gray2, corners2, maxCorners, qualityLevel, minDistance,
			Mat(), blockSize, useHarrisDetector, k);
		Vector<Log::MatchedCorners> match = log.getCornerPair(src1,corners1,src2,corners2);
		log.write(match);
		imshow(source_window1, src1);
		imshow(source_window2, src2);
		//delay;
		cout << "Time Taken : " << float(clock() - begin_time) << endl;
		c = cvWaitKey(1);
		if (c == 27)
			break;
	}
	cvReleaseCapture(&capture1);
	cvDestroyWindow(source_window1);
	cvReleaseCapture(&capture2);
	cvDestroyWindow(source_window2);
	getchar();
	return 0;
}
/*
Point2f pt1;
bool addRemovePt1 = false;
Point2f pt2;
bool addRemovePt2 = false;
void onMouse(int event, int x, int y, int flags, void* param)
{
	char* a = (char *)param;
	if (event == CV_EVENT_LBUTTONDOWN){
		if (strcmp("cameraOne", a)){
			pt1 = Point2f((float)x, (float)y);
			addRemovePt1 = true;
			cout << "addremovept1 :" << addRemovePt1 << endl;
			cout << "events:" << event << "x:" << x << "y:" << y << "flags:"
				<< flags << " void:" << a << endl;
		}
		else{
			pt1 = Point2f((float)x, (float)y);
			addRemovePt1 = true;

		}
	}
}
//constants

TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
Size winSize(10, 10);
const int MAX_COUNT = 500;
bool needToInit = false;
bool nightMode = false;

//cv:: Mat gray, prevGray, image;
Mat gray1, prevGray1, image1;
int cornerTest()
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	Mat frame;
	
	Mat gray2, prevGray2, image2;
	vector<Point2f> points[2];
	//points[0].push_back(Point2f{ (float)100, (float)100 });
	namedWindow("Camera1", 1);
	setMouseCallback("Camera1", onMouse, "cameraOne");
	for (;;)	{
		cout << "addremovept:" << addRemovePt1 << endl;
		cap >> frame; 
		if (CornerLoop("Camera1", frame,points) > 0)
			break;
	}
	return 0;
}
Mat image, gray, prevGray;
int CornerLoop(char* windowName, Mat frame, vector<Point2f> points[2]){
	frame.copyTo(image);
	cvtColor(image, gray, CV_BGR2GRAY);
	if (!points[0].empty())
	{
		vector<uchar> status;
		vector<float> err;
		if (prevGray.empty())
			gray.copyTo(prevGray);
		calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,3, termcrit, 0);
		size_t i, k;
		for (i = k = 0; i < points[1].size(); i++){
			if (addRemovePt1){
				if (norm(pt1 - points[1][i]) <= 5){
					addRemovePt1 = false;
					continue;
				}
			}
			if (!status[i])
				continue;
			points[1][k++] = points[1][i];
			circle(image, points[1][i], 3, Scalar(0, 255, 0), -1, 8);
		}
		points[1].resize(k);
	}
	if (addRemovePt1 && points[1].size() < (size_t)MAX_COUNT)
	{
		cout << "addRemovePt";
		vector<Point2f> tmp;
		tmp.push_back(pt1);
		cornerSubPix(gray, tmp, winSize, cvSize(-1, -1), termcrit);
		points[1].push_back(tmp[0]);
		addRemovePt1 = false;
	}
	imshow(windowName, image);
	std::swap(points[1], points[0]);
	swap(prevGray, gray);
	return waitKey(1);
}

int getVideo(){
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;
	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame;
		cvtColor(frame, edges, COLOR_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0) break;
	}
	return 0;
}
*/