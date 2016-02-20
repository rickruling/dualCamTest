#pragma once

#include <opencv2/highgui/highgui.hpp>

int showCameraMat(int cameraNum);
int showCamera(int cameraNum);
int showVideo();
int showStereo(int cam1, int cam2);
int compareStereo(IplImage *object, IplImage * image);
IplImage* doPyrDown(IplImage* in);
//Mat doPyrDown(Mat in);
IplImage* doCanny(IplImage* in, double    lowThresh, double    highThresh, double    aperture);
IplImage* convertToGray(IplImage* in);
IplImage* getGray(IplImage* img, int num = 0);
int designs();