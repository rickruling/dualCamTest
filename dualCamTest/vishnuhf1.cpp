//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"

#include <vector>
#include <stdio.h>

#include<iostream>

using namespace cv;
using namespace std;

int showCamera(int cameraNum)
{
	int c = 0;
	char* window1 = "Camera1";
	cvNamedWindow(window1, CV_WINDOW_AUTOSIZE);
	CvCapture* capture1 = cvCaptureFromCAM(cameraNum);
	IplImage *img1;
	while (true){
		img1 = cvQueryFrame(capture1);
		cvShowImage(window1, img1);
		c = cvWaitKey(10);
		if (c == 27){
			break;
		}
	}
	cvReleaseCapture(&capture1);
	cvDestroyWindow(window1);
	return 0;
}

int showCameraMat(int cameraNum)
{

	char* source_window = "Camera";
	int c = 0;
	
	CvCapture* capture = cvCaptureFromCAM(cameraNum);

	if (!capture){
		cout << "Cannot open the cam" << endl;
		for (int i = 0; i < 999999; i++)
		{
			for (int j = 0; j < 200; j++);
		}
		return 0;
	}
	for (;;) {
		Mat img1 = cvQueryFrame(capture);
		imshow(source_window, img1);
		c = cvWaitKey(10);
		if (c == 27)
			break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow(source_window);
	return true;
}

IplImage* doPyrDown(IplImage* in) {
	// Best to make sure input image is divisible by two.    //  
	assert(in->width % 2 == 0 && in->height % 2 == 0);
	IplImage* out = cvCreateImage(cvSize(in->width / 2, in->height / 2), in->depth, in->nChannels);
	cvPyrDown(in, out);
	return(out);
}
/*
Mat doPyrDown(Mat in){
	//assert(in->width % 2 == 0 && in->height % 2 == 0);
	Mat out = cvCreateImage(cvSize(in.cols / 2, in.rows / 2), in.depth, in.step/in.cols);
	//cvPyrDown(in, out);
	return(out);
}*/

IplImage* doCanny(IplImage* in, double    lowThresh, double    highThresh, double    aperture) {
	if(in->nChannels != 1)
		return(0);
	//Canny only handles gray scale images
	//CVAPI(CvSize) cvGetSize(const CvArr* arr);
	//CV_INLINE  CvSize  cvSize( int width, int height )
	//IplImage* out = cvCreateImage(cvSize(cvGetSize(in), IPL_DEPTH_8U, 1));
	IplImage* out = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
	cvCanny(in, out, lowThresh, highThresh, aperture);
	return(out);
};

int showStereo(int cam1,int cam2)
{
	int c = 0;
	char* window1 = "Camera1";
	char* window2 = "Camera2";
	cvNamedWindow(window1, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(window2, CV_WINDOW_AUTOSIZE);
	CvCapture* capture1 = cvCaptureFromCAM(cam1);
	CvCapture* capture2 = cvCaptureFromCAM(cam2);
	IplImage *img1,*img2;
	//Mat img1, img2;
	while (true){
		img1 = cvQueryFrame(capture1);
		img2 = cvQueryFrame(capture2);
		cvShowImage(window1, img1);
		cvShowImage(window2, img2);
		c = cvWaitKey(10);
		if (c == 27){
			break;
		}
	}
	cvReleaseCapture(&capture1);
	cvDestroyWindow(window1);
	cvReleaseCapture(&capture2);
	cvDestroyWindow(window2);
	return 0;
}

IplImage* getGray(IplImage* img,int num=0){
	IplImage* img2;
	img2 = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);

	for (int i = 0; i < img->width; i++)
	{
		for (int j = 0; j < img->height; j++)
		{
			*(img2->imageData + i*img2->height + j)
				= *(img->imageData + i*img->height * 3 + j * 3 + num);
			int a = 1;
		}
	}
	return img2;
}

int test1(){
	int c = 0;
	cvNamedWindow("Camera", 0);
	//cvNamedWindow("red", 0);
	CvCapture* capture1 = cvCaptureFromCAM(0);
	IplImage *img1, *img2;
	while (true){
		img1 = cvQueryFrame(capture1);
		img2 = cvCreateImage(cvSize(img1->width, img1->height), img1->depth, 1);
		cvShowImage("Camera", img1);
		//cvCanny(getGray(img1, 1), img2, 1, 3);
		//cvShowImage("red", getGray(img1, 0));
		//cvShowImage("blue", getGray(img1, 1));
		//cvShowImage("green", getGray(img1, 2));
		//cvLaplace(getGray(img1, 2), img2);
		//cvSobel(getGray(img1, 1), img2, 1, 1);
		//cvDilate(getGray(img1, 1), img2);
		cvShowImage("window2", img2);
		c = cvWaitKey(10);
		if (c == 27){
			break;
		}
	}
	cvReleaseCapture(&capture1);
	cvDestroyWindow("window2");
	exit(0);
	return 0;
}


IplImage* convertToGray(IplImage* img){
	IplImage* img2;
	img2 = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);
	for (int i = 0; i < img->width; i++)
	{
		for (int j = 0; j < img->height; j++)
		{
			*(img2->imageData + i*img2->height + j)
				= (*(img->imageData + i*img->height * 3 + j * 3 + 0) +
					*(img->imageData + i*img->height * 3 + j * 3 + 1) +
					*(img->imageData + i*img->height * 3 + j * 3 + 2))/3;
				int a = 1;
		}
	}
	return img2;
}
/*		for (int i = 0; i < img->width; i++)
		{
			for (int j = 0; j < img->height; j++)
			{
				for (int k = 0; k < img->nChannels; k++)
				{
					char data, d1;
					data = *(img->imageData);
					d1 = *(img->imageData + i*img->width + j);
					//*(img2->imageData + j*img2->height + i+k) = *(img->imageData + j*img->height + i+k);
					*(img2->imageData + i*img2->height*3 + j * 3 + k) = *(img->imageData + i*img->height*3 + j * 3 + k);
					int a = 1;
				}
			}
		}*/
int showVideo(){

	VideoCapture cap(0); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video cam" << endl;
		for (int i = 0; i < 999999; i++)
		{
			for (int j = 0; j < 999; j++);
		}
		return -1;
	}

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	while (1)
	{
		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
		}

		imshow("MyVideo", frame); //show the frame in "MyVideo" window

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}


/*
void open(VideoCapture &camera)
{
camera.open(0);
if (!camera.isOpened())
{
std::cout << "Camera did not open";
exit(1);
}
camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
}

int main() {
VideoCapture camera;
namedWindow("CamerOP", CV_WINDOW_AUTOSIZE);
bool b = 0;
open(camera);
int c = 0;
while (true)
{
// Grab the next camera frame.
Mat cframe;
b = camera.read(cframe);
if (!b)
{
std::cerr << "ERROR: Couldn't grab a camera frame." << std::endl;
exit(1);
}
imshow("CamerOP", cframe);
c = cvWaitKey(10);
if (c == 27)
break;
}
return 1;
}*/



static void help()
{
	printf(
		"This program demonstrated the use of the SURF Detector and Descriptor using\n"
		"either FLANN (fast approx nearst neighbor classification) or brute force matching\n"
		"on planar objects.\n"
		"Usage:\n"
		"./find_obj <object_filename> <scene_filename>, default is box.png  and box_in_scene.png\n\n");
	return;
}

// define whether to use approximate nearest-neighbor search
#define USE_FLANN

#ifdef USE_FLANN
static void
flannFindPairs(const CvSeq*, const CvSeq* objectDescriptors,
const CvSeq*, const CvSeq* imageDescriptors, vector<int>& ptpairs)
{
	int length = (int)(objectDescriptors->elem_size / sizeof(float));

	cv::Mat m_object(objectDescriptors->total, length, CV_32F);
	cv::Mat m_image(imageDescriptors->total, length, CV_32F);


	// copy descriptors
	CvSeqReader obj_reader;
	float* obj_ptr = m_object.ptr<float>(0);
	cvStartReadSeq(objectDescriptors, &obj_reader);
	for (int i = 0; i < objectDescriptors->total; i++)
	{
		const float* descriptor = (const float*)obj_reader.ptr;
		CV_NEXT_SEQ_ELEM(obj_reader.seq->elem_size, obj_reader);
		memcpy(obj_ptr, descriptor, length*sizeof(float));
		obj_ptr += length;
	}
	CvSeqReader img_reader;
	float* img_ptr = m_image.ptr<float>(0);
	cvStartReadSeq(imageDescriptors, &img_reader);
	for (int i = 0; i < imageDescriptors->total; i++)
	{
		const float* descriptor = (const float*)img_reader.ptr;
		CV_NEXT_SEQ_ELEM(img_reader.seq->elem_size, img_reader);
		memcpy(img_ptr, descriptor, length*sizeof(float));
		img_ptr += length;
	}

	// find nearest neighbors using FLANN
	cv::Mat m_indices(objectDescriptors->total, 2, CV_32S);
	cv::Mat m_dists(objectDescriptors->total, 2, CV_32F);
	cv::flann::Index flann_index(m_image, cv::flann::KDTreeIndexParams(4));  // using 4 randomized kdtrees
	flann_index.knnSearch(m_object, m_indices, m_dists, 2, cv::flann::SearchParams(64)); // maximum number of leafs checked

	int* indices_ptr = m_indices.ptr<int>(0);
	float* dists_ptr = m_dists.ptr<float>(0);
	for (int i = 0; i<m_indices.rows; ++i) {
		if (dists_ptr[2 * i]<0.6*dists_ptr[2 * i + 1]) {
			ptpairs.push_back(i);
			ptpairs.push_back(indices_ptr[2 * i]);
		}
	}
}
#else

static double
compareSURFDescriptors(const float* d1, const float* d2, double best, int length)
{
	double total_cost = 0;
	assert(length % 4 == 0);
	for (int i = 0; i < length; i += 4)
	{
		double t0 = d1[i] - d2[i];
		double t1 = d1[i + 1] - d2[i + 1];
		double t2 = d1[i + 2] - d2[i + 2];
		double t3 = d1[i + 3] - d2[i + 3];
		total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
		if (total_cost > best)
			break;
	}
	return total_cost;
}

static int
naiveNearestNeighbor(const float* vec, int laplacian,
const CvSeq* model_keypoints,
const CvSeq* model_descriptors)
{
	int length = (int)(model_descriptors->elem_size / sizeof(float));
	int i, neighbor = -1;
	double d, dist1 = 1e6, dist2 = 1e6;
	CvSeqReader reader, kreader;
	cvStartReadSeq(model_keypoints, &kreader, 0);
	cvStartReadSeq(model_descriptors, &reader, 0);

	for (i = 0; i < model_descriptors->total; i++)
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* mvec = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM(kreader.seq->elem_size, kreader);
		CV_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
		if (laplacian != kp->laplacian)
			continue;
		d = compareSURFDescriptors(vec, mvec, dist2, length);
		if (d < dist1)
		{
			dist2 = dist1;
			dist1 = d;
			neighbor = i;
		}
		else if (d < dist2)
			dist2 = d;
	}
	if (dist1 < 0.6*dist2)
		return neighbor;
	return -1;
}

static void
findPairs(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs)
{
	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq(objectKeypoints, &kreader);
	cvStartReadSeq(objectDescriptors, &reader);
	ptpairs.clear();

	for (i = 0; i < objectDescriptors->total; i++)
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		const float* descriptor = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM(kreader.seq->elem_size, kreader);
		CV_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
		int nearest_neighbor = naiveNearestNeighbor(descriptor, kp->laplacian, imageKeypoints, imageDescriptors);
		if (nearest_neighbor >= 0)
		{
			ptpairs.push_back(i);
			ptpairs.push_back(nearest_neighbor);
		}
	}
}
#endif

/* a rough implementation for object location */
static int
locatePlanarObject(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
const CvPoint src_corners[4], CvPoint dst_corners[4])
{
	double h[9];
	CvMat _h = cvMat(3, 3, CV_64F, h);
	vector<int> ptpairs;
	vector<CvPoint2D32f> pt1, pt2;
	CvMat _pt1, _pt2;
	int i, n;

#ifdef USE_FLANN
	flannFindPairs(objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs);
#else
	findPairs(objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs);
#endif

	n = (int)(ptpairs.size() / 2);
	if (n < 4)
		return 0;

	pt1.resize(n);
	pt2.resize(n);
	for (i = 0; i < n; i++)
	{
		pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs[i * 2]))->pt;
		pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints, ptpairs[i * 2 + 1]))->pt;
	}

	_pt1 = cvMat(1, n, CV_32FC2, &pt1[0]);
	_pt2 = cvMat(1, n, CV_32FC2, &pt2[0]);
	if (!cvFindHomography(&_pt1, &_pt2, &_h, CV_RANSAC, 5))
		return 0;

	for (i = 0; i < 4; i++)
	{
		double x = src_corners[i].x, y = src_corners[i].y;
		double Z = 1. / (h[6] * x + h[7] * y + h[8]);
		double X = (h[0] * x + h[1] * y + h[2])*Z;
		double Y = (h[3] * x + h[4] * y + h[5])*Z;
		dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
	}

	return 1;
}
/*
int main2(int argc, char** argv)
{
const char* object_filename = argc == 3 ? argv[1] : "box.png";
const char* scene_filename = argc == 3 ? argv[2] : "box_in_scene.png";

cv::initModule_nonfree();
help();

IplImage* object = cvLoadImage(object_filename, CV_LOAD_IMAGE_GRAYSCALE);
IplImage* image = cvLoadImage(scene_filename, CV_LOAD_IMAGE_GRAYSCALE); */

int compareStereo(IplImage *object, IplImage * image){

	if (!object || !image)
	{
		fprintf(stderr, "Can not load images");
		exit(-1);
	}

	CvMemStorage* storage = cvCreateMemStorage(0);

	cvNamedWindow("Object", 1);
	cvNamedWindow("Object Correspond", 1);

	static CvScalar colors[] =
	{
		{ { 0, 0, 255 } },
		{ { 0, 128, 255 } },
		{ { 0, 255, 255 } },
		{ { 0, 255, 0 } },
		{ { 255, 128, 0 } },
		{ { 255, 255, 0 } },
		{ { 255, 0, 0 } },
		{ { 255, 0, 255 } },
		{ { 255, 255, 255 } }
	};

	IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
	cvCvtColor(object, object_color, CV_GRAY2BGR);

	CvSeq* objectKeypoints = 0, *objectDescriptors = 0;
	CvSeq* imageKeypoints = 0, *imageDescriptors = 0;
	int i;
	CvSURFParams params = cvSURFParams(500, 1);

	double tt = (double)cvGetTickCount();
	cvExtractSURF(object, 0, &objectKeypoints, &objectDescriptors, storage, params);
	printf("Object Descriptors: %d\n", objectDescriptors->total);

	cvExtractSURF(image, 0, &imageKeypoints, &imageDescriptors, storage, params);
	printf("Image Descriptors: %d\n", imageDescriptors->total);
	tt = (double)cvGetTickCount() - tt;

	printf("Extraction time = %gms\n", tt / (cvGetTickFrequency()*1000.));

	CvPoint src_corners[4] = { { 0, 0 }, { object->width, 0 }, { object->width, object->height }, { 0, object->height } };
	CvPoint dst_corners[4];
	IplImage* correspond = cvCreateImage(cvSize(image->width, object->height + image->height), 8, 1);
	cvSetImageROI(correspond, cvRect(0, 0, object->width, object->height));
	cvCopy(object, correspond);
	cvSetImageROI(correspond, cvRect(0, object->height, correspond->width, correspond->height));
	cvCopy(image, correspond);
	cvResetImageROI(correspond);

#ifdef USE_FLANN
	printf("Using approximate nearest neighbor search\n");
#endif

	if (locatePlanarObject(objectKeypoints, objectDescriptors, imageKeypoints,
		imageDescriptors, src_corners, dst_corners))
	{
		for (i = 0; i < 4; i++)
		{
			CvPoint r1 = dst_corners[i % 4];
			CvPoint r2 = dst_corners[(i + 1) % 4];
			cvLine(correspond, cvPoint(r1.x, r1.y + object->height),
				cvPoint(r2.x, r2.y + object->height), colors[8]);
		}
	}
	vector<int> ptpairs;
#ifdef USE_FLANN
	flannFindPairs(objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs);
#else
	findPairs(objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs);
#endif
	for (i = 0; i < (int)ptpairs.size(); i += 2)
	{
		CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, ptpairs[i]);
		CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem(imageKeypoints, ptpairs[i + 1]);
		cvLine(correspond, cvPointFrom32f(r1->pt),
			cvPoint(cvRound(r2->pt.x), cvRound(r2->pt.y + object->height)), colors[8]);
	}

	cvShowImage("Object Correspond", correspond);
	for (i = 0; i < objectKeypoints->total; i++)
	{
		CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem(objectKeypoints, i);
		CvPoint center;
		int radius;
		center.x = cvRound(r->pt.x);
		center.y = cvRound(r->pt.y);
		radius = cvRound(r->size*1.2 / 9. * 2);
		cvCircle(object_color, center, radius, colors[0], 1, 8, 0);
	}
	cvShowImage("Object", object_color);

	cvWaitKey(0);

	cvDestroyWindow("Object");
	cvDestroyWindow("Object Correspond");

	return 0;
}

int designs(){
	cvNamedWindow("window", 1);
	IplImage *img;
	img = cvCreateImage(cvSize(640, 480), 8, 3);
	for (int i = 0; i < img->width; i++)
	{
		for (int j = 0; j < img->height; j++)
		{
			*(img->imageData + i*img->height * 3 + j * 3 + 0)
				= i % 100 + 50;
			*(img->imageData + i*img->height * 3 + j * 3 + 1)
				= 0;
			*(img->imageData + i*img->height * 3 + j * 3 + 2)
				= 0;
		}
	}
	cvShowImage("window", img);
	cvWaitKey(0);
	return 0;

}