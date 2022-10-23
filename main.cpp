
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/* constant value */
#define deg_rad 0.0175

/* <> = needed to config */
/* raw image */
const int raw_cen_x = 474;	// <> 
const int raw_cen_y = 291;	// <>
const int rad = 230;		// <>
const int di = rad * 2;
const string img_path = "./resource/raw.png";
const Rect crop(raw_cen_x - rad, raw_cen_y - rad, di, di);
Mat raw_img, crop_img;

/* dewarp image */
#define dw_hratio 1.f	// <>
#define dw_wratio 1.5f	// <>
const int dw_rows = rad * dw_hratio;
const int dw_cols = 360 * dw_wratio;
const float res_r = float(rad) / float(dw_rows);
const float res_c = 360.0f / float(dw_cols);
Mat dw_img(dw_rows, dw_cols, CV_8UC3);
Mat gray_img, thres_img;

/* video capture */
#define vdoFramerate 24.f	// <>
VideoCapture cap("./resource/vdo_raw.mp4");
VideoWriter video("./resource/vdo_out.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'),
					vdoFramerate, Size((int)dw_cols, (int)dw_rows));
int numFrame = 0;
unsigned long startTime, endTime;
int framerate, maxFramerate = 0, minFramerate = 100;
int sumFramerate;

/* ball tracking */
Mat diff_img[3];
vector<Vec3f> circles;
int numBall = 0;

int main()
{
	//raw_img = imread(img_path, IMREAD_COLOR);
	//cout << "\nStarting" << endl;
	cout << "\nBall coordinates" << endl;
	//cout << "\nBall discovered" << endl;
	//cout << "\nPrevious framerate" << endl;

	while (1)
	{
		startTime = clock();
		cap >> raw_img;
		if (raw_img.empty())// || numFrame >= 5)	
			break;

		//cout << "Frame:" << numFrame  << "\r   ";
		numFrame++;
		
		crop_img = raw_img(crop);
		
		/* dewarping procress */
		for (float t = 0; t < dw_img.cols; t += 1)
		{
			for (float r = 0; r < dw_img.rows; r += 1)
			{
				int x = r * res_r * cos(t * (double)res_c * deg_rad) + rad;
				int y = rad - r * (double)res_r * sin(t * (double)res_c * deg_rad);
				int col = dw_img.cols - 1 - t;
				int row = dw_img.rows - 1 - r;
				dw_img.at<Vec3b>(row, col) = crop_img.at<Vec3b>(y, x);
			}
		}

		#define circleDetect
		#ifdef circleDetect
		/* split red color channel */
		split(dw_img, diff_img);
		gray_img = diff_img[2];
		
		/* circle detect */
		HoughCircles(gray_img, circles, HOUGH_GRADIENT, 1, 10, 55, 10, 1, 10);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			int rad = 8;
			
			if (c[1] > 75 && c[1] < 150)
			{
				circle(dw_img, center, rad, Scalar(0, 0, 255), -1);
				numBall++;
				cout << "[" << c[0] << ", " << c[1] << "]  \r";
			}
			//endTime = clock();	
		}
		#endif
		
		//video.write(dw_img);
		//imwrite("./resource/img_out.jpg", dw_img);
		imshow("Crop Image", crop_img);
		imshow("Dewarping Image", dw_img);
		//imshow("Gray Image", gray_img);
		//imshow("Threshold Image", thres_img);

		waitKey(1);

		//endTime = clock();
		//framerate = 1000 / (endTime - startTime);
		//if (framerate > maxFramerate)	maxFramerate = framerate;
		//if (framerate < minFramerate)	minFramerate = framerate;
		//sumFramerate += framerate;
		//cout << framerate << "\r ";
		//cout << numBall << "/" << numFrame << "\r";
	}
	//cout << "\nMinimum Framerate: " << minFramerate << endl;
	//cout << "Maximum Framerate: " << maxFramerate << endl;
	//cout << "Average Framerate: " << sumFramerate / numFrame << endl;

	/* for first time settings */
	//#define firstTimeSetting
	#ifdef firstTimeSetting
		circle(raw_img, Point(raw_cen_x,raw_cen_y), rad, Scalar(0,0,255), 1);
		circle(raw_img, Point(raw_cen_x, raw_cen_y), 3, Scalar(0,0,255), -1);
		imwrite("./resource/img_out.jpg", raw_img);
		imshow("Image", raw_img);
		waitKey(0);
	#endif

	/* end of procress */
	cap.release();
	video.release();
	cout << "\n\nFinished" << endl;
	return 0;
}

