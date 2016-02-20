#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>
#pragma once
using namespace std;
using namespace cv;

class Log
{
private:
	const char* filename;
	std::ofstream file;
	static int filecount;
	struct CornerData{
		int x;
		int y;
		uchar blue;
		uchar green;
		uchar red;
		int neighbourSum;
	};


public:
	struct MatchedCorners{
		int x1;
		int y1;
		int x2;
		int y2;
		int minimum;
	};
	Log();
	Log(char* name);
	~Log();
	void debug(char* string,...);
	bool write(cv::Mat img, int datatype = 0);
	bool writeImageAsText(cv::Mat img,char* filename,int datatype = 0);
	void info(char* text, ...);
	void write(char *s);
	bool write(std::vector<cv::Point2f> corners);
	bool write(cv::Mat img, std::vector<cv::Point2f> corners);
	bool write(Vector<Log::MatchedCorners> match); 
	bool write(Vector<CornerData> Corner);
	bool writeCorners(cv::Mat img, std::vector<cv::Point2f> corners);
	bool markPoints(cv::Mat img,std::vector<cv::Point2f>corners);
	cv::Mat createGrayImage();
	cv::Mat createColorImage();
	std::vector<cv::Point2f> createRandomPoints();
	void matchCorners(std::vector<cv::Point2f>corners1, std::vector<cv::Point2f>corners2);
	Vector<CornerData> getCornerData(cv::Mat img, vector<Point2f> corners);
	Vector<MatchedCorners> getCornerPair(cv::Mat img1, vector<Point2f> corners1, cv::Mat img2, vector<Point2f> corners2);
};

