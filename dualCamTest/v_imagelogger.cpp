#include "v_imagelogger.h"

#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <time.h>
#include <iomanip>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int Log::filecount = 0;
int window = 7;
int windowsHalf = 3;

struct RGB {
	uchar blue;
	uchar green;
	uchar red;
};

//RGB& rgb = image.ptr<RGB>(y)[x];
Vector<Log::CornerData> Log::getCornerData(cv::Mat img, vector<Point2f> corners){
	Vector<CornerData> CornersInfo;
	/*certified gives correct values*/
	try{
		for (int cornerpoint = 0; cornerpoint < corners.capacity(); cornerpoint++)
		{
			int neighbourSum = 0;
			int x = corners[cornerpoint].x;
			int y = corners[cornerpoint].y;
			uchar r = img.ptr<RGB>(y)[x].red;
			uchar g = img.ptr<RGB>(y)[x].green;
			uchar b = img.ptr<RGB>(y)[x].blue;
			RGB& rgb = img.ptr<RGB>(y)[x];
			for (int i = -1*windowsHalf; i <= windowsHalf; i++)
			{
				for (int j = -1*windowsHalf; j <= windowsHalf; j++)
				{/*
					neighbourSum += (int)(img.ptr<RGB>(y+i)[x+j].red + 
						img.ptr<RGB>(y+i)[x+j].blue + img.ptr<RGB>(y+i)[x+j].green);
						*/
					neighbourSum = (int)(pow((img.ptr<RGB>(y + i)[x + j].red - img.ptr<RGB>(y)[x].red),2) +
										pow((img.ptr<RGB>(y + i)[x + j].blue - img.ptr<RGB>(y)[x].blue),2) +
										pow((img.ptr<RGB>(y + i)[x + j].green - img.ptr<RGB>(y)[x].green),2));
				}
			}
			CornersInfo.push_back(CornerData{ x, y, b, g, r, neighbourSum });
		}
	}
	catch (exception& e)
	{
		cout << "\n\nException Message : il 1\n\n" << e.what() << '\n';
	}
	return CornersInfo;
}
Vector<Log::MatchedCorners> Log::getCornerPair(cv::Mat img1, vector<Point2f> corners1, cv::Mat img2, vector<Point2f> corners2){
	Vector<CornerData> Corner1, Corner2;
	Vector<MatchedCorners> match;
	Corner1 = getCornerData(img1, corners1);
	Corner2 = getCornerData(img2, corners2);
	write(Corner1);
	write(Corner2);
	int x=0, y=0;
	int difference = 0;
	char * val = (char *)malloc(10);
	//write(Corner1);
	try{
		for (int i = 0; i < Corner1.size(); i++)
		{
			int minimum = 9999;
			x = 0, y = 0;
			int found = 0;
			for (int j = 0; j < Corner2.size(); j++)
			{
				difference = Corner1[i].neighbourSum - Corner2[j].neighbourSum;
				if (abs(difference) < minimum && abs((Corner2[j].y - Corner1[i].y))<20)
				{
					found = 1;
					minimum = abs(difference);
					x = Corner2[j].x; y = Corner2[j].y;
				}
			}
			if (found = 1)
				match.push_back(MatchedCorners{ Corner1[i].x, Corner1[i].y, x, y, minimum });
		}
		//drawing matched corners in image corners
		for (int i = 0; i < match.size(); i++)
		{
			int y1 = match[i].y1;
			int x1 = match[i].x1;
			int y2 = match[i].y2;
			int x2 = match[i].x2;
			//char* val = "0";
			sprintf(val, "%d - %d", i,(int)round(sqrt(pow(x2-x1,2) + pow(y2-y1,2))));
			putText(img1, val, cvPoint(x1, y1), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 0), 1, CV_AA);
			putText(img2, val, cvPoint(x2, y2), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 0), 1, CV_AA);
		}
	}
	catch (exception& e)
	{
		cout << "\n\nException Message : il 2\n\n" << e.what() << '\n';
	}

	return match;
}

Log::Log(){

}
Log::Log(char *filename)
{
	this->filename = filename;
	file.open(filename);
	time_t now = time(0);
	tm* localtm = localtime(&now);
	file << "Program Started ";
	file << "[" << localtm->tm_mday << "/" << localtm->tm_mon + 1 << "/" << localtm->tm_year + 1900 << " - ";
	file << localtm->tm_hour << ":" << localtm->tm_min << ":" << localtm->tm_sec << "] ";
	file << "\n-------------------------------------------------------------------------";
}
Log::~Log()
{

	file << "\n-------------------------------------------------------------------------";
	file << "\nProgram terminated";
	time_t now = time(0);
	tm* localtm = localtime(&now);
	file << "[" << localtm->tm_mday << "/" << localtm->tm_mon + 1 << "/" << localtm->tm_year + 1900 << " - ";
	file << localtm->tm_hour << ":" << localtm->tm_min << ":" << localtm->tm_sec << "] ";
	file.close();
}

void Log::debug(char* s, ...){
	time_t now = time(0);
	tm* localtm = localtime(&now);
	file << "\n[" << localtm->tm_mday << "/" << localtm->tm_mon + 1 << "/" << localtm->tm_year + 1900 << " - ";
	file << localtm->tm_hour << ":" << localtm->tm_min << ":" << localtm->tm_sec << "] ";
	va_list args;
	va_start(args, s);
	const char* arg = s;
	while (strlen(args) > 0){
		file << arg;
		arg = va_arg(args, const char*);
	}
	va_end(args);
}
void Log::write(char* s){
	file << s;
}
void Log::info(char *s, ...){
	va_list args;
	va_start(args, s);
	const char* arg = s;
	file << "\n";
	while (strlen(args) > 0){
		file << arg;
		arg = va_arg(args, const char*);
	}
	va_end(args);
}

bool Log::write(vector<cv::Point2f> corners){
	for (int cornerpoint = 0; cornerpoint < corners.capacity(); cornerpoint++)
	{
		file << "\nPoint " << setw(2)<<  cornerpoint << "[ ";
		file << " x : " << setw(3) << corners[cornerpoint].x;
		file << " y : " << setw(3) << corners[cornerpoint].y << " ]";
	}
	return true;
}

bool Log::write(Vector<Log::MatchedCorners> match){
	cout << "\n Vector MAtched Corners capacity: " << match.size();
	try{
		for (int i = 0; i < match.size(); i++)
		{
			file << "\nPoint " << setw(2) << i << "[";
			file << " x1:" << setw(3) << match[i].x1
				<< " y1:" << setw(3) << match[i].y1
				<< " x2:" << setw(3) << match[i].x2
				<< " y2:" << setw(3) << match[i].y2
				<< " min:" << setw(3) << match[i].minimum 
				<< "dist:" << round(sqrt(pow(match[i].x1 - match[i].x2, 2) + pow(match[i].y1 - match[i].y2, 2))) << "]";
		}
	}
	catch (exception& e)
	{
		cout << "\n\nException Message Main1:\n\n " << e.what() << '\n';
	}
	return true;
}

bool Log::write(Vector<CornerData> Corner){
	cout << "\nLog::write Vector Corner Data size : " << Corner.size();
	try{
		for (int i = 0; i < Corner.size(); i++)
		{
			file << "\nPoint " << setw(2) << i << "[";
			file << " x:" << setw(3) << Corner[i].x
				<< " y:" << setw(3) << Corner[i].y
				<< " sum:" << setw(3) << Corner[i].neighbourSum<< " ]";
		}
	}
	catch (exception& e)
	{
		cout << "\n\nException Message il4:\n\n " << e.what() << '\n';
	}
	return true;
}

bool Log::write(cv::Mat img, std::vector<cv::Point2f> corners){
	for (int cornerpoint = 0; cornerpoint < corners.capacity(); cornerpoint++)
	{
		int  x = corners[cornerpoint].x;
		int  y = corners[cornerpoint].y;
		file << "\nPoint " << setw(2) << cornerpoint << "[ ";
		file << " x : " << setw(3) << x << " y : " << setw(3) << y << " ]";
		int neighbourSum = 0;
		if (img.step / img.cols > 1)
		{
			RGB& rgb = img.ptr<RGB>(y)[x];
			file << " R :" << setw(3) << (int)rgb.red << " G :" << setw(3) << (int)rgb.green << " B:" << setw(3) << (int)rgb.blue;
			
			for (int i = -2; i < 3; i++)
			{
				for (int j = -2; j < 3; j++)
				{
					neighbourSum += (int)img.ptr<RGB>(y)[x].red;
				}
			}
		}
		else{

			file << " I :" << (int)img.ptr<uchar>(y)[x];
			for (int i = -1 * windowsHalf; i <= windowsHalf; i++)
			{
				for (int j = -1 * windowsHalf; j <= windowsHalf; j++)
				{
					neighbourSum += (int)img.ptr<uchar>(y+i)[x+j];
				}
			}
		}
		file << "sum "<<window<<"x"<<window<<" = " << neighbourSum;
	}
	return true;
}

bool Log::writeCorners(cv::Mat img, std::vector<cv::Point2f> corners){
	if (img.step / img.cols > 1)
	{
		for (int cornerpoint = 0; cornerpoint < corners.capacity(); cornerpoint++)
		{
			uchar neighbour[7][7];
			int neighbourSum = 0;
			int x = corners[cornerpoint].x;
			int y = corners[cornerpoint].y;

			for (int i = -1 * windowsHalf; i <= windowsHalf; i++)
			{
				uchar* ptr = (uchar*)(img.data + (y + i) * img.step);
				for (int j = -1 * windowsHalf; j <= windowsHalf; j++)
				{
					neighbour[i + windowsHalf][j + windowsHalf] = ptr[3 * (x + j)];
				}
			}

			for (int i = 0; i < window; i++)
			{
				for (int j = 0; j < window; j++)
				{
					cout << (int)neighbour[i][j] << " ";
					neighbourSum += (int)neighbour[i][j];
				}
				cout << endl;
			}
			cout << "x " << x << " y " << y << " I: " << setw(3)
				<< (int)neighbour[windowsHalf][windowsHalf] << " n sum : " << cornerpoint << " is " << neighbourSum << endl;
		}
	}
	else{

		for (int cornerpoint = 0; cornerpoint < corners.capacity(); cornerpoint++)
		{
			uchar neighbour[7][7];
			int neighbourSum = 0;
			int x = corners[cornerpoint].x;
			int y = corners[cornerpoint].y;

			for (int i = -1 * windowsHalf; i <= windowsHalf; i++)
			{
				uchar* ptr = (uchar*)(img.data + (y + i) * img.step);
				for (int j = -1 * windowsHalf; j <= windowsHalf; j++)
				{
					neighbour[i + windowsHalf][j + windowsHalf] = ptr[x + j];
				}
			}

			for (int i = 0; i < window; i++)
			{
				for (int j = 0; j < window; j++)
				{
					cout << (int)neighbour[i][j] << " ";
					neighbourSum += (int)neighbour[i][j];
				}
				cout << endl;
			}
			cout << "x " << x << " y " << y << " I: " << setw(3)
				<< (int)neighbour[windowsHalf][windowsHalf] << " n sum : " << cornerpoint << " is " << neighbourSum << endl;

		}
	}
	
	return true;
}

bool Log::write(cv::Mat img, int datatype){
	file << "\n------------------------------- Image Data Start --------------------------" << endl;
	file << " Rows: " << img.rows << endl;
	file << " Cols: " << img.cols << endl;
	file << " Depth: " << img.step/img.cols;
	if (img.step / img.cols > 1)
	{
		for (int y = 0; y < img.rows; y++)
		{
			file << "\nRow:" << setw(3) << y << " ";
			uchar* ptr = (uchar*)(img.data + y * img.step);
			for (int x = 0; x < img.cols; x++)
			{
				file << " "<<setw(3)<<x<<" ";
				if (datatype == 1){
					file << "[" << setw(3) << (int)ptr[3 * x] << "," << setw(3) << (int)ptr[3 * x + 1] << "," << setw(3) << (int)ptr[3 * x + 2] << "]";
				}
				else{
					file << "[" << setw(3) << ptr[3 * x] << "," << setw(3) << ptr[3 * x + 1] << "," << setw(3) << ptr[3 * x + 2] << "]";
				}
			}
		}
	}
	else{
		for (int i = 0; i < img.rows; i++)
		{
			file << "\nRow:" <<setw(3)<< i<<" ";
			for (int j = 0; j < img.cols; j++)
			{
					file << setw(3) << (int)img.ptr<uchar>(i)[j]<<" ";
			}
		}
	}
	file << "\n------------------------------- Image Data Ends --------------------------" << endl;
	return true;
}

bool Log::writeImageAsText(cv::Mat img, char* filename, int datatype){
	Log::filecount++;
	ofstream ofile(filename);
	file << "Image file ( " << filename << ") written";
	ofile << "------------------------------- Image Data Start --------------------------" << endl;
	for (int y = 0; y < img.rows; y++)
	{
		ofile << "----------------------------- " << "Row : " << y << " ----------------------------" << endl;
		uchar* ptr = (uchar*)(img.data + y * img.step);
		for (int x = 0; x < img.cols; x++)
		{
			ofile << "--Col : " << x << "  ";
			if (datatype == 1){
				ofile << "[" << (int)ptr[3 * x] << "," << (int)ptr[3 * x + 1] << "," << (int)ptr[3 * x + 2] << "]\t";
			}
			else{
				ofile << "[" << ptr[3 * x] << "," << ptr[3 * x + 1] << "," << ptr[3 * x + 2] << "]\t";
			}
		}
		ofile << "------------------------------- Image Data Ends --------------------------" << endl;
	}
	return true;
}


cv::Mat Log::createGrayImage(){
	Mat img;
	img = cvCreateImage(cvSize(512, 256), 8, 1);
	for (int i = 0; i < img.rows; i++)
	{
		uchar* ptr = (uchar*)(img.data + (i)* img.step);
		for (int j = 0; j < img.cols; j++)
		{
			ptr[j] = i + j;
		}
	}
	return img;
}


bool Log::markPoints(cv::Mat img, std::vector<cv::Point2f>corners){
	file<<"\nLog::markPoints\n";
	RNG rng(12345);
	int r = 4;
	char * val = (char *)malloc(15);
	try {
		if (img.step / img.cols > 1)
		{
			vector<CornerData> cornerData1;
			for (int i = 0; i < corners.size(); i++)
			{
				int y = corners[i].y;
				int x = corners[i].x;
				int neighbourSum = 0;
				RGB * rgb = (RGB *)malloc(3 * sizeof(RGB *));
				for (int i = -1 * windowsHalf; i <=windowsHalf; i++)
				{
					for (int j = -1 * windowsHalf; j <= windowsHalf; j++)
					{
						if ((y + i) >= 0 && (y + i) < img.cols && (x + j) >= 0 && (x + j) < img.rows)
							*rgb= img.ptr<RGB>(y + i)[x + j];
						neighbourSum+= rgb->blue + rgb->green + rgb->red;
					}
				}
				sprintf(val, "%d-[%d]", i,neighbourSum);
				putText(img, val, corners[i], FONT_HERSHEY_COMPLEX_SMALL, 0.4, cvScalar(0, 0, 0), 1, CV_AA);
				//writing points in corner1
				cornerData1.push_back(CornerData{ x, y, rgb->blue, rgb->green, rgb->red, neighbourSum });
			}
			file << "\nsize of vector : " << cornerData1.size();
			
			for (int cornerpoint = 0; cornerpoint < cornerData1.size(); cornerpoint++)
			{
				file << "\nPoint " << setw(2) << cornerpoint << "[ ";
				file << " x : " << setw(3) << cornerData1[cornerpoint].x;
				file << " y : " << setw(3) << cornerData1[cornerpoint].y;
				file << " b : " << setw(3) << (int)cornerData1[cornerpoint].blue;
				file << " g : " << setw(3) << (int)cornerData1[cornerpoint].green;
				file << " r : " << setw(3) << (int)cornerData1[cornerpoint].red;
				file << " sum : " << setw(6) << cornerData1[cornerpoint].neighbourSum;
				file << " ]";
			}
			cornerData1.erase(cornerData1.begin(),cornerData1.end());
		}
		else{
			for (int i = 0; i < corners.size(); i++)
			{
				int y = corners[i].y;
				int x = corners[i].x;
				int neighbourSum = 0;
				for (int i = -1 * windowsHalf; i <= windowsHalf; i++)
				{
					for (int j = -1 * windowsHalf; j <= windowsHalf; j++)
					{
						if ((y + i) >= 0 && (y + i) < img.cols && (x + j) >= 0 && (x + j) < img.rows)
							neighbourSum += (int)img.ptr<uchar>(y + i)[x + j];
					}
				}
				sprintf(val, "[%d]", neighbourSum);
				putText(img, val, corners[i], FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(255), 1, CV_AA);
				
			}
		}
	}
		catch (exception& e)
		{
			cout <<"\n\nException Message : il3\n\n"<<e.what() << '\n';
			file << "\nException Message : " << e.what();
			file << "Val : " << val<<endl;
		}
		free(val);
		file << "\nLog::markPoints ends::\n";
	return true;
}


cv::Mat Log::createColorImage(){
	Mat img;
	img = cvCreateImage(cvSize(512, 256), 8, 3);
	for (int i = 0; i < img.rows; i++)
	{
		uchar* ptr = (uchar*)(img.data + (i)* img.step);
		for (int j = 0; j < img.cols; j++)
		{
			RGB rgb = {i%255,j%255,i+j};
			img.ptr<RGB>(i)[j] = rgb;
		}
	}
	return img;
}

std::vector<Point2f> Log::createRandomPoints(){
	vector<Point2f>  points;
	points.push_back(Point2f(200, 50));
	points.push_back(Point2f(400, 50));
	return points;
}


void Log::matchCorners(std::vector<cv::Point2f>corners1, std::vector<cv::Point2f>corners2){

}
/*
void Log::info(char *s, ...)
{
	va_list args;
	va_start(args, s);
	try{
		for (const char* arg = s; arg != NULL; arg = va_arg(args, const char*)){
			cout << arg;
		}
	}
	catch (exception& e)
	{
		cout << e.what() << '\n';
	}
	catch (...) {
		cout << "default exception";
	}
	va_end(args);
}*/


/*
va_list vl;
va_start(vl,n);
for (i=0;i<n;i++)
{
val=va_arg(vl,double);
printf (" [%.2f]",val);
}
va_end(vl);
printf ("\n");

template<typename First, typename ... Strings>
void Output(First arg, const Strings&... rest) {
	std::cout << arg << " ";
	Output(rest...);
}
va_list args;
va_start (args, firstArg);
for (const char* arg = firstArg; arg != NULL; arg = va_arg(arg, const char*)) {
// do something with arg
}

va_end(args);

void Capitalize(string* s, ...)
{
va_list ap;

va_start(ap, s);

while (s)
{
string::size_type i = 0;

while ((*s)[i] != '\0')
{
(*s)[i] = toupper((*s)[i]);
i++;
}

s = va_arg(ap, string*);
}

va_end(ap);
}
*/
