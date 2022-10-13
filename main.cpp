
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
const int raw_cen_x = 489; // 453
const int raw_cen_y = 272; // 330
const int rad = 296;
const int di = rad * 2;
const string img_path = "./resource/raw.png";
const Rect crop(raw_cen_x - rad, raw_cen_y - rad, di, di);
Mat raw_img;

// new image
const int w_ratio = 2.5;
const int h_ratio = 1;
const int new_w = w_ratio * 360;
const int new_h = h_ratio * rad;

// video capture
VideoCapture cap("./resource/vdo_small.mp4");
int numFrame = 0;


int main()
{
	//raw_img = imread(img_path, IMREAD_COLOR);
	
	while (1)
	{
		cap >> raw_img;
		if (raw_img.empty() || numFrame >= 5)	
			break;

		numFrame++;

		/*
		Mat crop_img = raw_img(crop);
		Mat dw_img(rad, 360, CV_8UC3);
		
		for (float t = 0; t < dw_img.cols; t += 1)
		{
			for (float r = 0; r < dw_img.rows; r += 1)
			{
				int x = r * cos(t * deg_rad) + rad;
				int y = rad - r * sin(t * deg_rad);
				dw_img.at<Vec3b>((int)r, (int)t) = crop_img.at<Vec3b>(x, y);
			}
		}
		*/
	}

	//resize(dw_img, dw_img, Size(new_w, new_h), INTER_AREA);
	//imshow("Image", dw_img);

	imwrite("./resource/img_out.jpg", raw_img);
	imshow("Image", raw_img);
	waitKey(0);
	return 0;
}

