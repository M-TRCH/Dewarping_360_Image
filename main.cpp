
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
const int raw_cen_x = 327;	// <> 
const int raw_cen_y = 250;	// <> 
const int rad = 230;		// <>
const int di = rad * 2;
const string img_path = "./resource/raw.png";
const Rect crop(raw_cen_x - rad, raw_cen_y - rad, di, di);
Mat raw_img, crop_img;

/* dewarp image */
const float dw_rows = rad * 1.0;	// <>
const float dw_cols = 360 * 1.75;	// <>
const float res_r = rad / dw_rows;
const float res_c = 360 / dw_cols;
Mat dw_img((int)dw_rows, (int)dw_cols, CV_8UC3);
Mat gray_img, thres_img;


/* video capture */
#define vdoFramerate 11.f
VideoCapture cap("./resource/vdo_raw.mp4");
VideoWriter video("./resource/vdo_out.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'),
					vdoFramerate, Size((int)dw_cols, (int)dw_rows));
int numFrame = 0;

/* ball detection */
Mat diff_img[3];
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

int main()
{
	//raw_img = imread(img_path, IMREAD_COLOR);
	cout << "\nStarting" << endl;

	while (1)
	{
		cap >> raw_img;
		if (raw_img.empty()) //  || numFrame >= 10)	
			break;

		//cout << "Frame:" << numFrame  << "\r";
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

		/* split red color channel */
		split(dw_img, diff_img);
		gray_img = diff_img[2];
		
		/* find red ball */
		//cvtColor(dw_img, gray_img, COLOR_BGR2GRAY);
		threshold(gray_img, thres_img, 130, 255, THRESH_BINARY);
		findContours(thres_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		//drawContours(dw_img, contours, -1, Scalar(0, 255, 0), 2);

		/* plot each point of contour */
		//cout << "contours-> " << contours.size() << endl;
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = 0; j < contours[i].size(); j++)
			{
				circle(dw_img, contours[i][j], 1, Scalar(0, 0, 255), -1);
			}
		}

		video.write(dw_img);
		imshow("Crop Image", crop_img);
		imshow("Dewarping Image", dw_img);
		// imshow("Gray Image", gray_img);
		imshow("Threshold Image", thres_img);

		char c = (char)waitKey(1);
		if (c == 'q')
			break;
	}

	/* for first time settings */
	//#define firstTimeSetting
	#ifdef firstTimeSetting
		circle(raw_img, Point(raw_cen_x,raw_cen_y), rad, Scalar(0,0,255), 1);
		circle(raw_img, Point(raw_cen_x, raw_cen_y), 3, Scalar(0,0,255), -1);
		imwrite("./resource/img_out.jpg", dw_img);
		imshow("Image", raw_img);
		waitKey(0);
	#endif

	/* end of procress */
	cap.release();
	video.release();
	cout << "\nFinished" << endl;
	return 0;
}

