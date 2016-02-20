#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<iostream>

using namespace cv;
using namespace std;

//constants

int getVideo();
Point2f pt,pt2;
bool addRemovePt = false, addRemovePt2 = false;
char cornerTester(Mat &frame, Mat &image, Mat &gray, Mat &prevGray, vector<Point2f> points[2], vector<Point2f> points2[2],
	char *windowName, Point2f &pt, bool &addRemovePt);
void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		char *c = (char *)param;
		cout << "event:" << c << endl;
		if (!strcmp(c, "2")){
			pt2 = Point2f((float)x, (float)y);
			addRemovePt2 = true;
		}
		else{
			pt = Point2f((float)x, (float)y);
			addRemovePt = true;
		}
	}
}

int main(){
	//getVideo();
	/*
	std::ofstream ofile;
	ofile.open("new.txt");
	file << "Program Started ";*/
	VideoCapture cap,cap2;
	char * windowName = "window1";
	char * windowName2 = "window2";
	namedWindow(windowName, 1);
	namedWindow(windowName2, 1);
	setMouseCallback(windowName, onMouse, "1");
	setMouseCallback(windowName2, onMouse, "2");

	Mat gray, prevGray, image;
	vector<Point2f> points[2];
	Mat gray2, prevGray2, image2;
	vector<Point2f> points2[2];
	cap.open(0);
	cap2.open(1);
	if (!cap.isOpened() ||! cap2.isOpened()){
		cout << "Could not initialize capturing...\n";
		return 0;
	}
	Mat frame,frame2;
	while (true){
		cap >> frame;
		if (cornerTester(frame, image, gray, prevGray, points, points2, windowName,pt,addRemovePt) == ' ')
			break;
		cap2 >> frame2;
		if (cornerTester(frame2, image2, gray2, prevGray2, points2, points, windowName2, pt2, addRemovePt2) == ' ')
			break;
	}
	return 0;
}

char cornerTester(Mat &frame, Mat &image, Mat &gray, Mat &prevGray, vector<Point2f> points[2], vector<Point2f> points2[2],
	char *windowName, Point2f &pt, bool &addRemovePt)
{
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
	Size winSize(10, 10);
	const int MAX_COUNT = 500;

		frame.copyTo(image);
		cvtColor(image, gray, CV_BGR2GRAY);
		char * val = (char *)malloc(25);
		if (!points[0].empty()){
			vector<uchar> status;
			vector<float> err;
			if (prevGray.empty())
				gray.copyTo(prevGray);
			calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
				3, termcrit, 0);
			size_t i, k;
			for (i = k = 0; i < points[1].size(); i++){
				if (addRemovePt){
					if (norm(pt - points[1][i]) <= 5){
						addRemovePt = false;
						continue;
					}
				}
				if (!status[i])
					continue;
				points[1][k++] = points[1][i];
				circle(image, points[1][i], 3, Scalar(0, 255, 0), -1, 8);
				//sprintf(val, "%.2f,%.2f", (points[1][i].x), (points[1][i].y));
				
				if (points[1].size()>i && points2[1].size()>i){
					sprintf(val, "x-%.1f,d-%.2f", (points[1][i].x - points2[1][i].x), (200) / (points[1][i].x - points2[1][i].x));
				}
				putText(image, val, points[1][i], FONT_HERSHEY_COMPLEX_SMALL, 0.6, cvScalar(0, 0, 0), 1, CV_AA);
			}
			points[1].resize(k);
		}
		if (addRemovePt && points[1].size() < (size_t)MAX_COUNT){
			vector<Point2f> tmp;
			tmp.push_back(pt);
			cornerSubPix(gray, tmp, winSize, cvSize(-1, -1), termcrit);
			points[1].push_back(tmp[0]);
			addRemovePt = false;
		}
		imshow(windowName, image);
		char c = waitKey(10);
		std::swap(points[1], points[0]);
		swap(prevGray, gray);
	return c;
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

void log(vector<Point2f> points[2]){
	cout << "\n Writing Points : " ;
	try{
		int i,x,y;
		for (i = 0; i < points[1].size(); i++){
			x = points[1][i].x;
			y = points[1][i].y;
		}
	}
	catch (exception& e)
	{
		cout << "\n\nException Message il4:\n\n " << e.what() << '\n';
	}
}