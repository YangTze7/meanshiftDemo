#include "opencv2/opencv.hpp"
#include <iostream>
#include"time.h"
#include <omp.h>
using namespace cv;
using namespace std;



static void help(char** argv)
{
	cout << "\nDemonstrate mean-shift based color segmentation in spatial pyramid.\n"
		<< "Call:\n   " << argv[0] << " image\n"
		<< "This program allows you to set the spatial and color radius\n"
		<< "of the mean shift window as well as the number of pyramid reduction levels explored\n"
		<< endl;
}

//This colors the segmentations
static void floodFillPostprocess(Mat& img, const Scalar& colorDiff = Scalar::all(1))
{
	CV_Assert(!img.empty());
	RNG rng = theRNG();
	Mat mask(img.rows + 2, img.cols + 2, CV_8UC1, Scalar::all(0));
	#pragma omp parallel for
	for (int y = 0; y < img.rows; y++)
	{

		for (int x = 0; x < img.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(img, mask, Point(x, y), newVal, 0, colorDiff, colorDiff);
			}
		}
	}

}

string winName = "meanshift";
int spatialRad, colorRad, maxPyrLevel;
Mat img, res;

static void meanShiftSegmentation(int, void*)
{
	cout << "spatialRad=" << spatialRad << "; "
		<< "colorRad=" << colorRad << "; "
		<< "maxPyrLevel=" << maxPyrLevel << endl;
	pyrMeanShiftFiltering(img, res, spatialRad, colorRad, maxPyrLevel);
	floodFillPostprocess(res, Scalar::all(2));
	imshow(winName, res);
	imwrite("meanshift.tif", res);
	img.release();
	res.release();
}


int main(int argc, char** argv)
{
	int a = clock();
	//if (argc != 2)
	//{
	//	help(argv);
	//	return -1;
	//}
	char* man = "D:\\pca_temp\\pca1.tif";
	string fimg = "ER0363.tif";//"G:/Pic/2012060619243397.png";"D:/gdalData/meanshift/ER0363.tif"
	//IplImage *pSrcImage = cvLoadImage(man, CV_LOAD_IMAGE_ANYDEPTH);
	//IplImage *pGrayImage_8U_2 = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
	//cvConvertScale(pSrcImage, pGrayImage_8U_2); //64F×ª8U
	//cvShowImage("1", pGrayImage_8U_2);

	//cvWaitKey();
	Mat img1 = imread(man);

	cv::Mat MatTemp2;
	img1.convertTo(MatTemp2, CV_8U);
	imshow("ee", MatTemp2);
	img = imread(fimg);
	if (img.empty())
		return -1;
	//640-by-480  it works well to 	set spatialRadiusequal = 2 and colorRadiusequal = 40
	// max_level, which describes how many levels of scale pyramid you want 
	//used for segmentation.A max_levelof 2 or 3 works well for a 640 - by - 480 color image
	spatialRad = 10;
	colorRad = 10;
	maxPyrLevel = 1;

	namedWindow(winName, WINDOW_GUI_NORMAL);

	/*createTrackbar("spatialRad", winName, &spatialRad, 80, meanShiftSegmentation);
	createTrackbar("colorRad", winName, &colorRad, 60, meanShiftSegmentation);
	createTrackbar("maxPyrLevel", winName, &maxPyrLevel, 5, meanShiftSegmentation);*/

	meanShiftSegmentation(0, 0);
	int b = clock();
	/*int minute = (b - a) / 1000 / 60;
	int second = (b - a) / 1000 % 60;*/
	cout << "total time: " << (b - a)<<"ºÁÃë"<<endl;
	
	//waitKey(6000);
	waitKey();
	
	return 0;
}
