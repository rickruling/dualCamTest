#include "v_DataTypes.h"

#include <iostream>
#include <vector>
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;

void stereoTest(){
	CvCapture* capture = cvCaptureFromCAM(0);
	Mat img = cvQueryFrame(capture);
	namedWindow("output", CV_WINDOW_AUTOSIZE);
	//while (true)
	{
		img = cvQueryFrame(capture);
		imshow("output", img);
	}
}

void videoRecorder(){
	CvCapture* capture = 0;
	capture = cvCreateFileCapture(0);
	if (!capture){
		return;
	}    
	IplImage *bgr_frame = cvQueryFrame(capture);
	//Init the video read   
	double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	CvSize size = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	CvVideoWriter *writer = cvCreateVideoWriter("demo", CV_FOURCC('M', 'J', 'P', 'G'), fps, size);
	IplImage* logpolar_frame = cvCreateImage(size, IPL_DEPTH_8U, 3);
	while ((bgr_frame = cvQueryFrame(capture)) != NULL) {
		cvLogPolar(bgr_frame, logpolar_frame, cvPoint2D32f(bgr_frame->width / 2, bgr_frame->height / 2), 40, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS);
		cvWriteFrame(writer, logpolar_frame);
	}
	cvReleaseVideoWriter(&writer);
	cvReleaseImage(&logpolar_frame);
	cvReleaseCapture(&capture);
	return;

}
void vector_demo()
{
	std::vector<int> myvector(3, 100);
	std::vector<int>::iterator it;

	it = myvector.begin();
	it = myvector.insert(it, 200);

	myvector.insert(it, 2, 300);

	// "it" no longer valid, get a new one:
	it = myvector.begin();

	std::vector<int> anothervector(2, 400);
	myvector.insert(it + 2, anothervector.begin(), anothervector.end());

	int myarray[] = { 501, 502, 503 };
	myvector.insert(myvector.begin(), myarray, myarray + 3);

	std::cout << "myvector contains:";
	for (it = myvector.begin(); it<myvector.end(); it++)
		std::cout << ' ' << *it;
	std::cout << '\n';
}
