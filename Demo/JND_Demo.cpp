// JND_Demo.cpp : 定义控制台应用程序的入口点。
//

//#define  _CRT_SECURE_NO_WARNINGS.


#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include"Lin_GetJnd.h"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include"Lin_disjnd.h"

using namespace std;
using namespace cv;

int main()
{
	Mat orgImage;
	//orgImage = imread("./Market.exr");
	orgImage = imread("./1.bmp");
	//cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", orgImage);
	Mat Limage;
	cvtColor(orgImage, Limage, CV_BGR2GRAY);//得到亮度通道的图像

	
	//Limage.convertTo(Limage, CV_64FC1, 1.0/255); //归一化到0~1
	//imshow("image_1", Limage);
	Mat ordJnd = GetOrdJND(Limage);
	Mat disJnd = GetDisJnd(Limage);
	Mat JndVal = ordJnd + disJnd - 0.3 * min(disJnd, ordJnd);
	JndVal.convertTo(JndVal, CV_64FC1,1.0/ 255.0);
	/*double min, max;
	minMaxIdx(JndVal, &min, &max);
	double nor= max - min;
	Mat JndMap=(JndVal-min) / nor * 255;*/
	imshow("jnd", JndVal);
	waitKey(0);
	return 0;
}

