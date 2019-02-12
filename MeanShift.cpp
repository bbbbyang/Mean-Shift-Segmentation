//---------------- Head  File ---------------------------------------
#include "MeanShift.h"
#include <math.h>

//---------------- Name space ---------------------------------------
using namespace cv;
using namespace std;

//---------------- Definition ---------------------------------------
#define MS_MAX_NUM_CONVERGENCE_STEPS	5										// up to 10 steps are for convergence
#define MS_MEAN_SHIFT_TOL_COLOR			0.3										// minimum mean color shift change
#define MS_MEAN_SHIFT_TOL_SPATIAL		0.3										// minimum mean spatial shift change
const int dxdy[][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};	// Region Growing

// Constructor
Point5D::Point5D(){
	x = -1;
	y = -1;
}

// Destructor
Point5D::~Point5D(){
}

// Scale the OpenCV Lab color to Lab range
void Point5D::PointLab(){
	l = l * 100 / 255;
	a = a - 128;
	b = b - 128;
}

// Sclae the Lab color to OpenCV range that can be used to transform to RGB
void Point5D::PointRGB(){
	l = l * 255 / 100;
	a = a + 128;
	b = b + 128;
}

// Accumulate points
void Point5D::MSPoint5DAccum(Point5D Pt){
	x += Pt.x;
	y += Pt.y;
	l += Pt.l;
	a += Pt.a;
	b += Pt.b;
}

// Copy a point
void Point5D::MSPoint5DCopy(Point5D Pt){
	x = Pt.x;
	y = Pt.y;
	l = Pt.l;
	a = Pt.a;
	b = Pt.b;
}

// Compute color space distance between two points
float Point5D::MSPoint5DColorDistance(Point5D Pt){
	return sqrt((l - Pt.l) * (l - Pt.l) + (a - Pt.a) * (a - Pt.a) + (b - Pt.b) * (b - Pt.b));
}

// Compute spatial space distance between two points
float Point5D::MSPoint5DSpatialDistance(Point5D Pt){
	return sqrt((x - Pt.x) * (x - Pt.x) + (y - Pt.y) * (y - Pt.y));
}

// Scale point
void Point5D::MSPoint5DScale(float scale){
	x *= scale;
	y *= scale;
	l *= scale;
	a *= scale;
	b *= scale;
}

// Set point value
void Point5D::MSPOint5DSet(float px, float py, float pl, float pa, float pb){
	x = px;
	y = py;
	l = pl;
	a = pa;
	b = pb;
}

// Print 5D point
void Point5D::Print(){
	cout<<x<<" "<<y<<" "<<l<<" "<<a<<" "<<b<<endl;
}

// Constructor for spatial bandwidth and color bandwidth
MeanShift::MeanShift(float s, float r){
	hs = s;
	hr = r;
}

// Mean Shift Filtering
void MeanShift::MSFiltering(Mat& Img){
	int ROWS = Img.rows;			// Get row number
	int COLS = Img.cols;			// Get column number
	split(Img, IMGChannels);		// Split Lab color

	Point5D PtCur;					// Current point
	Point5D PtPrev;					// Previous point
	Point5D PtSum;					// Sum vector of the shift vector
	Point5D Pt;
	int Left;						// Left boundary
	int Right;						// Right boundary
	int Top;						// Top boundary
	int Bottom;						// Bottom boundary
	int NumPts;						// number of points in a hypersphere
	int step;
	
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			Left = (j - hs) > 0 ? (j - hs) : 0;						// Get Left boundary of the filter
			Right = (j + hs) < COLS ? (j + hs) : COLS;				// Get Right boundary of the filter
			Top = (i - hs) > 0 ? (i - hs) : 0;						// Get Top boundary of the filter
			Bottom = (i + hs) < ROWS ? (i + hs) : ROWS;				// Get Bottom boundary of the filter
			// Set current point and scale it to Lab color range
			PtCur.MSPOint5DSet(i, j, (float)IMGChannels[0].at<uchar>(i, j), (float)IMGChannels[1].at<uchar>(i, j), (float)IMGChannels[2].at<uchar>(i, j));
			PtCur.PointLab();
			step = 0;				// count the times
			do{
				PtPrev.MSPoint5DCopy(PtCur);						// Set the original point and previous one
				PtSum.MSPOint5DSet(0, 0, 0, 0, 0);					// Initial Sum vector
				NumPts = 0;											// Count number of points that satisfy the bandwidths
				for(int hx = Top; hx < Bottom; hx++){
					for(int hy = Left; hy < Right; hy++){
						// Set point in the spatial bandwidth
						Pt.MSPOint5DSet(hx, hy, (float)IMGChannels[0].at<uchar>(hx, hy), (float)IMGChannels[1].at<uchar>(hx, hy), (float)IMGChannels[2].at<uchar>(hx, hy));
						Pt.PointLab();

						// Check it satisfied color bandwidth or not
						if(Pt.MSPoint5DColorDistance(PtCur) < hr){
							PtSum.MSPoint5DAccum(Pt);				// Accumulate the point to Sum vector
							NumPts++;								// Count
						}
					}
				}
				PtSum.MSPoint5DScale(1.0 / NumPts);					// Scale Sum vector to average vector
				PtCur.MSPoint5DCopy(PtSum);							// Get new origin point
				step++;												// One time end
			// filter iteration to end
			}while((PtCur.MSPoint5DColorDistance(PtPrev) > MS_MEAN_SHIFT_TOL_COLOR) && (PtCur.MSPoint5DSpatialDistance(PtPrev) > MS_MEAN_SHIFT_TOL_SPATIAL) && (step < MS_MAX_NUM_CONVERGENCE_STEPS));
			
			// Scale the color
			PtCur.PointRGB();
			// Copy the result to image
			Img.at<Vec3b>(i, j) = Vec3b(PtCur.l, PtCur.a, PtCur.b);
		}
	}
}

void MeanShift::MSSegmentation(Mat& Img){

//---------------- Mean Shift Filtering -----------------------------
	// Same as MSFiltering function
	int ROWS = Img.rows;
	int COLS = Img.cols;
	split(Img, IMGChannels);

	Point5D PtCur;
	Point5D PtPrev;
	Point5D PtSum;
	Point5D Pt;
	int Left;
	int Right;
	int Top;
	int Bottom;
	int NumPts;					// number of points in a hypersphere
	int step;
	
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			Left = (j - hs) > 0 ? (j - hs) : 0;
			Right = (j + hs) < COLS ? (j + hs) : COLS;
			Top = (i - hs) > 0 ? (i - hs) : 0;
			Bottom = (i + hs) < ROWS ? (i + hs) : ROWS;
			PtCur.MSPOint5DSet(i, j, (float)IMGChannels[0].at<uchar>(i, j), (float)IMGChannels[1].at<uchar>(i, j), (float)IMGChannels[2].at<uchar>(i, j));
			PtCur.PointLab();
			step = 0;
			do{
				PtPrev.MSPoint5DCopy(PtCur);
				PtSum.MSPOint5DSet(0, 0, 0, 0, 0);
				NumPts = 0;
				for(int hx = Top; hx < Bottom; hx++){
					for(int hy = Left; hy < Right; hy++){
						
						Pt.MSPOint5DSet(hx, hy, (float)IMGChannels[0].at<uchar>(hx, hy), (float)IMGChannels[1].at<uchar>(hx, hy), (float)IMGChannels[2].at<uchar>(hx, hy));
						Pt.PointLab();

						if(Pt.MSPoint5DColorDistance(PtCur) < hr){
							PtSum.MSPoint5DAccum(Pt);
							NumPts++;
						}
					}
				}
				PtSum.MSPoint5DScale(1.0 / NumPts);
				PtCur.MSPoint5DCopy(PtSum);
				step++;
			}while((PtCur.MSPoint5DColorDistance(PtPrev) > MS_MEAN_SHIFT_TOL_COLOR) && (PtCur.MSPoint5DSpatialDistance(PtPrev) > MS_MEAN_SHIFT_TOL_SPATIAL) && (step < MS_MAX_NUM_CONVERGENCE_STEPS));
			
			PtCur.PointRGB();
			Img.at<Vec3b>(i, j) = Vec3b(PtCur.l, PtCur.a, PtCur.b);
		}
	}
//--------------------------------------------------------------------

//----------------------- Segmentation ------------------------------
	int RegionNumber = 0;			// Reigon number
	int label = -1;					// Label number
	float *Mode = new float [ROWS * COLS * 3];					// Store the Lab color of each region
	int *MemberModeCount = new int [ROWS * COLS];				// Store the number of each region
	memset(MemberModeCount, 0, ROWS * COLS * sizeof(int));		// Initialize the MemberModeCount
	split(Img, IMGChannels);
	// Label for each point
	int **Labels = new int *[ROWS];
	for(int i = 0; i < ROWS; i++)
		Labels[i] = new int [COLS];

	// Initialization
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			Labels[i][j] = -1;
		}
	}

	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS;j ++){
			// If the point is not being labeled
			if(Labels[i][j] < 0){
				Labels[i][j] = ++label;		// Give it a new label number
				// Get the point
				PtCur.MSPOint5DSet(i, j, (float)IMGChannels[0].at<uchar>(i, j), (float)IMGChannels[1].at<uchar>(i, j), (float)IMGChannels[2].at<uchar>(i, j));
				PtCur.PointLab();

				// Store each value of Lab
				Mode[label * 3 + 0] = PtCur.l;
				Mode[label * 3 + 1] = PtCur.a;
				Mode[label * 3 + 2] = PtCur.b;
				
				// Region Growing 8 Neighbours
				vector<Point5D> NeighbourPoints;
				NeighbourPoints.push_back(PtCur);
				while(!NeighbourPoints.empty()){
					Pt = NeighbourPoints.back();
					NeighbourPoints.pop_back();

					// Get 8 neighbours
					for(int k = 0; k < 8; k++){
						int hx = Pt.x + dxdy[k][0];
						int hy = Pt.y + dxdy[k][1];
						if((hx >= 0) && (hy >= 0) && (hx < ROWS) && (hy < COLS) && (Labels[hx][hy] < 0)){
							Point5D P;
							P.MSPOint5DSet(hx, hy, (float)IMGChannels[0].at<uchar>(hx, hy), (float)IMGChannels[1].at<uchar>(hx, hy), (float)IMGChannels[2].at<uchar>(hx, hy));
							P.PointLab();

							// Check the color
							if(PtCur.MSPoint5DColorDistance(P) < hr){
								// Satisfied the color bandwidth
								Labels[hx][hy] = label;				// Give the same label					
								NeighbourPoints.push_back(P);		// Push it into stack
								MemberModeCount[label]++;			// This region number plus one
								// Sum all color in same region
								Mode[label * 3 + 0] += P.l;
								Mode[label * 3 + 1] += P.a;
								Mode[label * 3 + 2] += P.b;
							}
						}
					}
				}
				MemberModeCount[label]++;							// Count the point itself
				Mode[label * 3 + 0] /= MemberModeCount[label];		// Get average color
				Mode[label * 3 + 1] /= MemberModeCount[label];
				Mode[label * 3 + 2] /= MemberModeCount[label];
			}
		}
	}
	RegionNumber = label + 1;										// Get region number
	
	// Get result image from Mode array
	for(int i = 0; i < ROWS; i++){
		for(int j = 0; j < COLS; j++){
			label = Labels[i][j];
			float l = Mode[label * 3 + 0];
			float a = Mode[label * 3 + 1];
			float b = Mode[label * 3 + 2];
			Point5D Pixel;
			Pixel.MSPOint5DSet(i, j, l, a, b);
			Pixel.PointRGB();
//			Pixel.Print();
			Img.at<Vec3b>(i, j) = Vec3b(Pixel.l, Pixel.a, Pixel.b);
		}
	}
//--------------------------------------------------------------------

//	for(int i = 0; i < ROWS; i++){
//		for(int j = 0; j < COLS - 1; j++){
//			if(Labels[i][j] != Labels[i][j + 1])
//				Img.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
//		}
//	}

//--------------- Delete Memory Applied Before -----------------------
	delete[] Mode;
	delete[] MemberModeCount;
	
	for(int i = 0; i < ROWS; i++)
		delete[] Labels[i];
	delete[] Labels;
}
