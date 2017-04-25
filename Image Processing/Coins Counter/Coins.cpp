//	Author 			: Mostafa Bassem Heiba
// 	Description 		: Coins Counter
//  	Date			: 26/3/2016
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

#define ELLIPSE_MORPH_ELEMENT 2
#define MORPH_ELEMENT_SIZE_LARGE_COINS 25
#define MORPH_ELEMENT_SIZE_SMALL_COINS 10
#define OPENING_OPERATION 2

/*Original_Image is the source Image , Image_Clone is a copy as not to modify Original Image, RegionFilled Image for Manipulations*/
Mat Original_Image, Image_Clone, RegionFilled_Image;
/* Large Coins to Store RegionFilled Large Coins , Boundary Large Coins for Storing Boundaries...etc */
Mat Large_Coins, Small_Coins, Boundary_Large_Coins, Boundary_Small_Coins, Difference;

int Large_Small_Coins_Count();

int main()
{
	Large_Small_Coins_Count();
	waitKey(0);
	return 0;
}

/*********************************************************************
/Function Name : Large_Small_Coins_Count							 /
/Descreption   : Counts Number of Large , Small Coins and Mark Them  /
/Student Nam   : Mostafa Bassem										 /
/Section       : Section 3											 /
*********************************************************************/
int Large_Small_Coins_Count()
{
	/* Read Image in GreyScale */
	Original_Image = cv::imread("coins.png", CV_LOAD_IMAGE_GRAYSCALE);

	/* Image Clone , Do Operations on a Copy */
	Mat Image_Clone = Original_Image.clone();

	/* Image Clone Thresholding */
	threshold(Image_Clone, Image_Clone, 0, 255, THRESH_OTSU | THRESH_BINARY);

	if (Image_Clone.empty())
		return -1;

	/* Preview and Save Image After Thresholding Operation */
	imshow("Thresholding", Image_Clone);
	imwrite("Thresholding.png", Image_Clone);

	/* Region Filling Part, Cross shaped structuring element */
	RegionFilled_Image = Mat::zeros(Image_Clone.size(), CV_8U);
	RegionFilled_Image.at<uchar>(75, 75) = 1;

	Mat prev;
	Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

	do
	{
		RegionFilled_Image.copyTo(prev);
		cv::dilate(RegionFilled_Image, RegionFilled_Image, kernel);
		RegionFilled_Image &= (1 - Image_Clone);
	} while (cv::countNonZero(RegionFilled_Image - prev) > 0);

	normalize(Image_Clone, Image_Clone, 0, 255, cv::NORM_MINMAX);
	normalize(RegionFilled_Image, RegionFilled_Image, 0, 255, cv::NORM_MINMAX);

	/* Invert Destination Image */
	RegionFilled_Image = 255 - RegionFilled_Image;

	/* Preview and Save Image After Region Filling */
	imshow("Region Filled Image", RegionFilled_Image);
	imwrite("RegionFilled_Image.png", RegionFilled_Image);

	/* Opening Operation to get Large Coins Only */
	Mat Structuring_Element = getStructuringElement(ELLIPSE_MORPH_ELEMENT, Size(2 * MORPH_ELEMENT_SIZE_LARGE_COINS + 1, 2 * MORPH_ELEMENT_SIZE_LARGE_COINS + 1), Point(MORPH_ELEMENT_SIZE_LARGE_COINS, MORPH_ELEMENT_SIZE_LARGE_COINS));
	morphologyEx(RegionFilled_Image, Large_Coins, OPENING_OPERATION, Structuring_Element);

	/* Now Find Total Number of Large Coins by Finding Image Countours size , i.e Curves Constructing Shapes */
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/* This Operation Modifies Image , Use Temp Copy */
	Mat Temp = Large_Coins.clone();
	findContours(Temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	cout << "Number of Large Coins: " << contours.size() << endl;

	/* Preview and Save Image of Large Coins Only*/
	imshow("Large Coins Only", Large_Coins);
	imwrite("Large_Coins.png", Large_Coins);

	/* Erode Large Coins Picture , Then Boundary = Original - Eroded ,Gives Outlines */
	kernel = cv::Mat::ones(3, 3, CV_8U);
	Mat eroded;
	erode(Large_Coins, eroded, kernel);
	Boundary_Large_Coins = Large_Coins - eroded;

	/* Dialation Here Just to Bold The Boundary and Appear Thicker */
	dilate(Boundary_Large_Coins, Boundary_Large_Coins, kernel);

	/* Preview and Save Boundary of Large Coins */
	imshow("Boundary Of Large Coins", Boundary_Large_Coins);
	imwrite("Boundary_Large_Coins.png", Boundary_Large_Coins);

	/* Now Get Difference = RegionFilled Image - Image Containg Large Coins , i.e Small Coins Only now */
	Difference = RegionFilled_Image - Large_Coins;

	/* Opening Operation for Enhancement to Get Small Coins Only */
	Structuring_Element = getStructuringElement(ELLIPSE_MORPH_ELEMENT, Size(2 * MORPH_ELEMENT_SIZE_SMALL_COINS + 1, 2 * MORPH_ELEMENT_SIZE_SMALL_COINS + 1), Point(MORPH_ELEMENT_SIZE_SMALL_COINS, MORPH_ELEMENT_SIZE_SMALL_COINS));
	morphologyEx(Difference, Small_Coins, OPENING_OPERATION, Structuring_Element);

	/* Now Find Total Number of Large Coins by Finding Image Countours size , i.e Curves Constructing Shapes */
	Temp = Small_Coins.clone();
	findContours(Temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	cout << "Number of Small Coins: " << contours.size() << endl;

	/* Preview and Save Image of Small Coins Only */
	imshow("Small Coins", Small_Coins);
	imwrite("Small_Coins.png", Small_Coins);

	/* Erode Small Coins Picture , Then Boundary = Original - Eroded ,Gives Outlines */
	erode(Small_Coins, eroded, kernel);
	Boundary_Small_Coins = Small_Coins - eroded;

	/* Dialation Here Just to Bold The Boundary and Appear Thicker */
	dilate(Boundary_Small_Coins, Boundary_Small_Coins, kernel);

	/* Preview and Save Boundary of Small Coins */
	imshow("Boundary for Small Coins", Boundary_Small_Coins);
	imwrite("Boudnary_Small_Coins.png", Boundary_Small_Coins);

	/* Create 8bit Color Image */
	Mat Image_RGB(Original_Image.size(), CV_8UC3);

	/* Convert Grayscale To Color Image */
	cvtColor(Original_Image, Image_RGB, CV_GRAY2RGB);

	/* Loop Through Original Image , Replace Boundaries of Large Coins' Pixels in Original Image With Red Color , Small Coins with Blue Color */
	for (int j = 0;j<Original_Image.rows;j++)
	{
		for (int i = 0;i<Original_Image.cols;i++)
		{
			if (Boundary_Small_Coins.at<uchar>(j, i) > 180)
			{
				Image_RGB.at<Vec3b>(Point(i, j))[0] = 255;

			}
			if (Boundary_Large_Coins.at<uchar>(j, i) > 180)
			{
				Image_RGB.at<Vec3b>(Point(i, j))[2] = 255;
			}

		}
	}

	/* Preview and Save Final Image With Large Coins Detected By Red Boundary , Small Ones With Blue Boundary */
	imshow("Final Image", Image_RGB);
	imwrite("Final_Image.png", Image_RGB);
}
