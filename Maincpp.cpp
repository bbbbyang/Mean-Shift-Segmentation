//--------------------------------------------------------------------
//				Bingyang Liu  2015/07/14
//				Meanshift Algorithm
//				OpenCV 3.0.0
//--------------------------------------------------------------------

//---------------- Head  File ---------------------------------------
#include <iostream>
#include "MeanShift.h"

int main(){
	// Load image
	Mat Img = imread("mandril_color.tif");
	resize(Img, Img, Size(256, 256), 0, 0, 1);
	// Show that image
	namedWindow("The Original Picture");
	imshow("The Original Picture", Img);

	// Convert color from RGB to Lab
	cvtColor(Img, Img, CV_RGB2Lab);

	// Initilize Mean Shift with spatial bandwith and color bandwith
	MeanShift MSProc(8, 16);
	// Filtering Process
	MSProc.MSFiltering(Img);
	// Segmentation Process include Filtering Process (Region Growing)
//	MSProc.MSSegmentation(Img);
	
	// Print the bandwith
	cout<<"the Spatial Bandwith is "<<MSProc.hs<<endl;
	cout<<"the Color Bandwith is "<<MSProc.hr<<endl;

	// Convert color from Lab to RGB
	cvtColor(Img, Img, CV_Lab2RGB);

	// Show the result image
	namedWindow("MS Picture");
	imshow("MS Picture", Img);

	waitKey();
	return 1;
}