
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// constant value
#define deg_rad 0.0175

// raw image
const int raw_cen_x = 487; // 453
const int raw_cen_y = 272; // 330
const int rad = 250; // 296
const int di = rad * 2;
const string img_path = "./resource/raw.png";
const Rect crop(raw_cen_x - rad, raw_cen_y - rad, di, di);
Mat raw_img, crop_img;

// out image
const int w_ratio = 2.5;
const int h_ratio = 1;
const int out_w = w_ratio * 360;
const int out_h = h_ratio * rad;

// dewarp image
const float dw_rows = rad * 1.0;
const float dw_cols = 360 * 1.5;
const float res_r = rad / dw_rows;
const float res_c = 360 / dw_cols;
Mat dw_img((int)dw_rows, (int)dw_cols, CV_8UC3);
Mat gray_img, thres_img;


// video capture
VideoCapture cap("./resource/vdo_small.mp4");
VideoWriter video("./resource/vdo_out.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'),
					29.75, Size((int)dw_cols, (int)dw_rows));
int numFrame = 0;


int main()
{
	//raw_img = imread(img_path, IMREAD_COLOR);
	cout << "\nStarting" << endl;

	while (1)
	{
		cap >> raw_img;
		if (raw_img.empty()) //|| numFrame >= 300)	
			break;

		//cout << "Frame:" << numFrame  << "\r";
		numFrame++;
	
		crop_img = raw_img(crop);
		
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
		//cvtColor(dw_img, gray_img, COLOR_BGR2GRAY);
		//threshold(gray_img, thres_img, 120, 255, THRESH_BINARY);

		video.write(dw_img);
		imshow("Crop Image", crop_img);
		imshow("Dewarping Image", dw_img);

		char c = (char)waitKey(1);
		if (c == 'q')
			break;

	}

	//resize(dw_img, dw_img, Size(out_w, out_h), INTER_AREA);
	
	//circle(raw_img, Point(raw_cen_x,raw_cen_y), rad, Scalar(0,0,255), 1);
	//imwrite("./resource/img_out.jpg", dw_img);
	
	//imshow("Image", dw_img);
	//waitKey(0);

	cap.release();
	video.release();
	cout << "\nFinished" << endl;

	return 0;
}

