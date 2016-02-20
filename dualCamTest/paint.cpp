#include "opencv\cv.h"
#include <opencv2/highgui/highgui.hpp>

void my_mouse_callback(int event, int x, int y, int flags, void* param);
CvRect box;
CvPoint p1,p2;
bool drawing_box = false;
int g_switch_value = 0;

IplImage* image;
void draw_points(IplImage* img, CvRect rect, int ch){
	if (ch == 3)
	{
		cvLine(img, cvPoint(box.x, box.y), cvPoint(box.x + box.width, box.y + box.height),
			cvScalar(0x00, 0x00, 0xff), /*thickness*/1, /*connectivity*/8);
	}
	else{
		cvLine(img, cvPoint(box.x, box.y), cvPoint(box.x + box.width, box.y + box.height),
			cvScalar(0x00, 0x00, 0xff), /*thickness*/3, /*connectivity*/4);
	}
}

void clear_image(int position)
{
	if (position == 0) {
		printf("nothing to do");
	}
	else {
		for (int y = 0; y<image->height; y++) {
			uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);
			for (int x = 0; x<image->width; x++) {
				ptr[3 * x] = 255;
				ptr[3 * x + 1] = 255;
				ptr[3 * x + 2] = 255;
			}
		}
		cvSetTrackbarPos("Clear", "Paint",0);
		printf("clearing...");
	}
}
void draw_box(IplImage* img, CvRect rect) {
	switch (cvGetTrackbarPos("Object", "Paint"))
	{
	case 0: std::cout << "Drawing recatangle";
		cvRectangle(img, cvPoint(box.x, box.y), cvPoint(box.x + box.width, box.y + box.height), cvScalar(0x00, 0x00, 0xff));
		break;
	case 1:std::cout << "Drawing circle";
		cvCircle(img, cvPoint(box.x, box.y), sqrt(pow(box.width, 2) + pow(box.height, 2)),
			cvScalar(0xff, 0x00, 0x00), /*thickness*/1, /*connectivity*/8);
		break;
	case 2:std::cout << "Drawing Line";
		cvLine(img, cvPoint(box.x, box.y), cvPoint(box.x + box.width, box.y + box.height),
			cvScalar(0x00, 0x00, 0xff), /*thickness*/1, /*connectivity*/8);
		break;
	case 3:
		break;
		//default:std::cout << "Drawing recatangle"; 
		//	cvRectangle(img, cvPoint(box.x, box.y), cvPoint(box.x + box.width, box.y + box.height), cvScalar(0xff, 0x00, 0x00));
	}
}

void switch_callback(int position) {
	if (position == 0) {
		printf("switch_off_function()");
	}
	else {
		printf("switch_on_function()");
	}
}
int mainp1(int argc, char* argv[]) {

	box = cvRect(-1, -1, 0, 0);
	image = cvCreateImage(cvSize(1360, 660), IPL_DEPTH_8U, 3);
	//cvZero(image);
	//set the colour of background white
	for (int y = 0; y<image->height; y++) {
		uchar* ptr = (uchar*)(image->imageData + y * image->widthStep);
		for (int x = 0; x<image->width; x++) {
			ptr[3 * x] = 255;
			ptr[3 * x + 1] = 255;
			ptr[3 * x + 2] = 255;
		}
	}
	IplImage* temp = cvCloneImage(image);
	cvNamedWindow("Paint");
	cvCreateTrackbar("Object", "Paint", &g_switch_value, 5, switch_callback);
	cvCreateTrackbar("Clear", "Paint", &g_switch_value, 1, clear_image);
	cvSetMouseCallback("Paint", my_mouse_callback, (void*)image);

	while (1) {
		cvCopyImage(image, temp);
		if (drawing_box)
			draw_box(temp, box);
		cvShowImage("Paint", temp);
		if (cvWaitKey(1) == 27) break;
	}
	cvReleaseImage(&image);
	cvReleaseImage(&temp);
	cvDestroyWindow("Paint");
	return 0;
}

void my_mouse_callback(int event, int x, int y, int flags, void* param) {
	IplImage* image = (IplImage*)param;
	std::cout << "pos: (" << x << "," << y << ")"
		"track" << cvGetTrackbarPos("Object", "Paint") << "flag : " << flags << std::endl;
	int choice = cvGetTrackbarPos("Object", "Paint");
	switch (event)
	{
	case CV_EVENT_MOUSEMOVE:
	{  if (drawing_box){
		box.width = x - box.x;
		box.height = y - box.y;
		if (choice == 3)
		{
			draw_points(image, box, 3);
		}if (choice == 4)
		{
			box.x = x;
			box.y = y;
			draw_points(image, box, 4);
		}
		
	}
	}
		break;
	case CV_EVENT_LBUTTONDOWN:
	{
		drawing_box = true;
		box = cvRect(x, y, 0, 0);
		if (choice == 3)
		{
			draw_points(image, box, 3);
		}if (choice == 4)
		{
			box.x = x;
			box.y = y;
			draw_points(image, box, 4);
		}
	}
		break;
	case CV_EVENT_LBUTTONUP: {
		drawing_box = false;
		if (box.width<0) {
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height<0)
		{
			box.y += box.height;
			box.height *= -1;
		}
		draw_box(image, box);
	}
		break;
	}
}