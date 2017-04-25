//	Author 			: Mostafa Bassem Heiba
// 	Description 		: Dart Board Game
//  	Date			: 2/5/2016

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

#define DART_BOARD_CIRCLES 9
#define OPENING_OPERATION 2
#define CLOSING_OPERATION 3
#define MAX_USER_SCORE 9

Mat Original_Image, Gray_Image, Thresholded_Image, Temp, Bullet_Detection_Image, Final_Image;

int thresh = 100;
int Score = 0;
int Bullet_Level_Count[9] = { 0,0,0,0,0,0,0,0,0 }; 

vector<Vec3f> circles;
float Dart_Contours[9][4];

int Bullet_Score();

int main(int argc, char** argv)
{
	

	
	Bullet_Score();
	waitKey(0);
	return 0;
}

/*********************************************************************
/Function Name : Bullet_Score										 /
/Descreption   : Counts Your Score in Dart Board Game				 /
/Student Nam   : Mostafa Bassem										 /
/Section       : Section 3											 /
*********************************************************************/
int Bullet_Score()
{
	/* Read Image */
	Original_Image = imread("Input Image.jpg", CV_LOAD_IMAGE_COLOR);
	
	/*Cloning for Further Previews */
	Final_Image = Original_Image.clone();
	Bullet_Detection_Image = Original_Image.clone();

	if (!Original_Image.data)
	{
		return -1;
	}

	/* Convert image to GrayScale */
	cvtColor(Original_Image, Gray_Image, CV_BGR2GRAY);

	/* Reduce noise to avoid false circle detection */
	GaussianBlur(Gray_Image, Gray_Image, Size(9, 9), 2, 2);

	/* Apply the Hough Transform to find the circles */
	for (int count = 0;count < DART_BOARD_CIRCLES;count++)
	{
		/*Use Hough Circle Transform for 1st 3 Circles only as radius doesn't increase linearly in them , then after these 3 circles radius start increas linearly with increase = 47 */
		/* We also don't have to compute Center many times , one time is enough as they are all concentric circles, each iteration we need to compute only radius*/

		/* 1st Circle Weight = 11*/
		if (count == 0)
		{
			HoughCircles(Gray_Image, circles, CV_HOUGH_GRADIENT, 1, Gray_Image.rows / 1, 40, 10, 0, 0);
			Dart_Contours[0][0] = circles[0][0];
			Dart_Contours[0][1] = circles[0][1];
			Dart_Contours[0][2] = circles[0][2];
			Dart_Contours[0][3] = MAX_USER_SCORE - count;

		}

		/* 2nd Circle Weight = 10*/
		if (count == 1)
		{
			HoughCircles(Gray_Image, circles, CV_HOUGH_GRADIENT, 1, Gray_Image.rows / 1, 40, 60, 0, 0);
			Dart_Contours[1][0] = Dart_Contours[0][0];
			Dart_Contours[1][1] = Dart_Contours[0][1];
			Dart_Contours[1][2] = circles[0][2];
			Dart_Contours[1][3] = MAX_USER_SCORE - count;
		}

		/* 3rd Circle Weight = 9 */
		if (count == 2)
		{
			HoughCircles(Gray_Image, circles, CV_HOUGH_GRADIENT, 1, Gray_Image.rows / 1, 40, 60, 40, 85);
			Dart_Contours[2][0] = Dart_Contours[0][0];
			Dart_Contours[2][1] = Dart_Contours[0][1];
			Dart_Contours[2][2] = circles[0][2];
			Dart_Contours[2][3] = MAX_USER_SCORE - count;
		}
		
		/* Rest Circles Have repeated Pattern (Old Radius + 47 ) */
		if (count>=3)
		{
			Dart_Contours[count][0] = Dart_Contours[0][0];
			Dart_Contours[count][1] = Dart_Contours[0][1];
			Dart_Contours[count][2] = Dart_Contours[count - 1][2]+47;
			Dart_Contours[count][3] = MAX_USER_SCORE - count;

		}
		
		
	}

	/* Draw the circles detected */
	for (int count = 0; count < DART_BOARD_CIRCLES; count++)
	{
		Point center(cvFloor(Dart_Contours[count][0]), cvFloor(Dart_Contours[count][1]));
		int radius = cvRound(Dart_Contours[count][2]);

		/* Circle Center , All Have Same Center */
		circle(Original_Image, center, 3, Scalar(0, 255, 0), -1, 8, 0);

		/* Circle Outline i chosed thicknes of 3 */
		circle(Original_Image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	/* Image of Detected Darts */
	imshow("Dart Board Detected", Original_Image);
	imwrite("Dart Board Detected.png", Original_Image);
	
	/* Now Filter Out Everyhting Except Bullets ( Found That Bullets Falls in this range (120->170) */
	inRange(Gray_Image, Scalar(120), Scalar(170), Thresholded_Image);

	/* Preview After Thresholding with this range */
	imshow("After Thresholding with bullet range", Thresholded_Image);
	imwrite("After Thresholding with bullet range.png", Thresholded_Image);

	/* Apply Morphological Operation on Thresholded Bullets , Opening followed by Closing to remove noise */
	Mat Structuring_Element = getStructuringElement(MORPH_ELLIPSE, cv::Size(9, 9));
	morphologyEx(Thresholded_Image, Temp, OPENING_OPERATION, Structuring_Element);
	morphologyEx(Temp, Temp, CLOSING_OPERATION, Structuring_Element);

	/* Preview After the two Morphological Operations */
	imshow("Result of Two Morphological Operations", Temp);
	imwrite("Result of Two Morphological Operations.png", Temp);
	
	/* Now Find Contours, This Contours represent Bullets Outlines */
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	/* Detect Edges Using Canny */
	Canny(Temp, Temp, thresh, thresh * 2, 3);

	/* Find contours */
	findContours(Temp, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/* Draw contours */
	Mat Temp_Drawing1 = Mat::zeros(Temp.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		drawContours(Temp_Drawing1, contours, i, color, 1, 8, hierarchy, 0, Point());


	}


	/* Now We'll Use minEnclosingCircle for each contour to get center and radius of minimum Circle Containing The Contour */
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i+=2)
	{
		minEnclosingCircle((Mat)contours[i], center[i], radius[i]);
	}

	/* Drawing Minimum Circles Containing Bullets */
	Mat Temp_Drawing2 = Mat::zeros(Temp.size(), CV_8UC3);

	/* I used Step of two as minEnclosingCircle Returns 2 smiliar circles for each contour */
	for (int i = 0; i< contours.size(); i+=2)
	{
		Scalar color = Scalar(255, 0, 0);
		circle(Final_Image, center[i], (int)radius[i], color, 2, 8, 0);
		circle(Bullet_Detection_Image, center[i], (int)radius[i], color, 2, 8, 0);
	}

	/* Preview Bullet Detected Only */
	imshow("Bullet Detected", Bullet_Detection_Image);
	imwrite("Bullet Detected.png", Bullet_Detection_Image);

	/* Accumlate Previous Dart Board Circles to The Result */
	for (int count = 0; count < DART_BOARD_CIRCLES; count++)
	{
		Point center(cvFloor(Dart_Contours[count][0]), cvFloor(Dart_Contours[count][1]));
		int radius = cvRound(Dart_Contours[count][2]);

		/* Circle Center , All Have Same Center */
		circle(Final_Image, center, 3, Scalar(0, 255, 0), -1, 8, 0);

		/* Circle Outline i chosed thicknes of 3 */
		circle(Final_Image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	imshow("Final Result", Final_Image);
	imwrite("Final Result.png", Final_Image);
	/* Now Calculate Total Score And Print Result in Console */
	for (int count = 0;count < contours.size(); count+=2)
	{
		double bullet_distance;
		int hit;
		Point Center(cvFloor(Dart_Contours[count][0]), cvFloor(Dart_Contours[count][1]));
		bullet_distance = norm(Center - (Point)center[count]);
		for (int inner_count = 0;inner_count < DART_BOARD_CIRCLES;inner_count++)
		{
			
			if (bullet_distance <= Dart_Contours[inner_count][2])
			{
				hit = Dart_Contours[inner_count][3];
				Bullet_Level_Count[inner_count] += 1;
				break;
			}
		}
		Score += hit;
	}
	for (int count = 0;count < MAX_USER_SCORE;count++)
	{
		cout << "Number of Bullets in Circle of Weight " << MAX_USER_SCORE - count << " is : " << Bullet_Level_Count[count] << endl;
	}
	cout << "Your Total Score is " << Score << " !" << endl;

	return 0;
}
