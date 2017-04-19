//---------------- Head  File ---------------------------------------
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <iostream>

//---------------- Name space ---------------------------------------
using namespace cv;
using namespace std;

// 5-Dimensional Point
class Point5D{
	public:
		float x;			// Spatial value
		float y;			// Spatial value
		float l;			// Lab value
		float a;			// Lab value
		float b;			// Lab value
	public:
		Point5D();													// Constructor
		~Point5D();													// Destructor
		void PointLab();											// Scale the OpenCV Lab color to Lab range
		void PointRGB();											// Sclae the Lab color to OpenCV range that can be used to transform to RGB
		void MSPoint5DAccum(Point5D);								// Accumulate points
		void MSPoint5DCopy(Point5D);								// Copy a point
		float MSPoint5DColorDistance(Point5D);						// Compute color space distance between two points
		float MSPoint5DSpatialDistance(Point5D);					// Compute spatial space distance between two points
		void MSPoint5DScale(float);									// Scale point
		void MSPOint5DSet(float, float, float, float, float);		// Set point value
		void Print();												// Print 5D point
};

class MeanShift{
	public:
		float hs;				// spatial radius
		float hr;				// color radius
		vector<Mat> IMGChannels;
	public:
		MeanShift(float, float);									// Constructor for spatial bandwidth and color bandwidth
		void MSFiltering(Mat&);										// Mean Shift Filtering
		void MSSegmentation(Mat&);									// Mean Shift Segmentation
};