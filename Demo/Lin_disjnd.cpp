#include "cv.h"
//#include "highgui.h"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include"Lin_disjnd.h"
//#include<iostream>
//#include <fstream>
#define  r 3
using namespace cv;
using namespace std;

Mat GetDisJnd(Mat orgImg)
{
	if (orgImg.type() != CV_64FC1)
		orgImg.convertTo(orgImg, CV_64FC1, 1.0);
	Mat preImg = GetPreImg(orgImg);
	if (preImg.type() != CV_64FC1)
		preImg.convertTo(preImg, CV_64FC1, 1.0);
	Mat freEng =abs( orgImg - preImg);
	return freEng;
	Mat burEng;
	GaussianBlur(freEng, burEng, Size(2 * r + 1, 2 * r + 1), (double)(2 * r + 1) / 3);
//	GaussianBlur(freEng, burEng, Size(3, 3), 1.5);            //这里是自己设置的参数，上面的参数感觉不靠谱
	Mat Img_Min = min(freEng,burEng);
	Mat var = GetVar(orgImg);
	Mat DisJnd = freEng;
	int height = freEng.rows;
	int width = freEng.cols;
	for (int h = 0; h < height; ++h)
	{
		double* vardata = var.ptr<double>(h);
		double* Img_Mindata = Img_Min.ptr<double>(h);
		double* DisJnddata = DisJnd.ptr<double>(h);
		for (int w = 0; w < width; ++w)
		{
			if (vardata[w] < 10)
				DisJnddata[w] = Img_Mindata[w];
		}
			
	}
	Mat sEdg = GetEdge(orgImg, 1);   //Matlab中传入的是0.7
	DisJnd = DisJnd.mul(sEdg);
	return DisJnd;
}
Mat GetEdge(Mat orgImg, int th)
{
	if (orgImg.type()!=CV_8UC1)
		orgImg.convertTo(orgImg, CV_8UC1, 1);
	Mat edgeMap;
	Canny(orgImg, edgeMap, 200, 200*3, 3);  //这里的参数是自己设置的
    edgeMap.convertTo(edgeMap, CV_64FC1, 1.0);
	Mat superEdge = 1 - 0.8*edgeMap;
	Mat resEdge;
	GaussianBlur(superEdge, resEdge, Size(5, 5), 0.8);
	return resEdge;
}
/*Mat ConvertToMat(Frame *inYuv)
{
int Height = inYuv->m_height[0];
int Width = inYuv->m_width[0];
Mat Img(Height,Width,CV_8UC1);
uchar *data = Img.data;
for (int i = 0; i < inYuv->m_compSize[0]; i++)                      //下面就是颜色空间的转换吧，inp->m_compSize[0]存的应该是像素的个数
*(data+i)=(uchar)inYuv->m_floatComp[0][i];

return Img;
}
*/
