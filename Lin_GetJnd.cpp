#include "cv.h"
#include"Lin_GetJnd.h"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include"Lin_disjnd.h"
#include<iostream>
#include <fstream>
#define  r 3
using namespace cv;
using namespace std;
double FramJnd = 0;
double**  g_jnd;    //设置一个全局变量来存储JND的值
double ** BlockJnd; //记录一帧块的JND的平均值
int cuWsize = 0;
int cuHsize = 0;
int cuWmargin = 0;
int cuHmargin = 0;
double *g_OneDBlcokJnd; //开辟一个空间用来储存每一帧的的区域平均JND的值
void JndPro(Mat orgImg)
{
	orgImg.convertTo(orgImg, CV_64FC1, 1.0);
	int height = orgImg.rows;
	int width = orgImg.cols;
	Mat ordJnd = GetOrdJND(orgImg);
	Mat disJnd = GetDisJnd(orgImg);
	Mat JndVal = ordJnd + disJnd - 0.3 * min(disJnd, ordJnd);
	//开辟空间存储Jnd的值
	g_jnd = new double*[height];
	if (g_jnd == NULL)
	{
		cout << "Failed to sert mem for ALL g_jnd" << endl;
		exit(1);

	}
	for (int i = 0; i < height; i++)
	{
		g_jnd[i] = new double[width];
		if (g_jnd[i] == NULL)
		{
			cout << "Failed to sert mem for PART g_jnd" << endl;
			exit(1);
		}
	}
	//开辟空间代码段结束
	for (int h = 0; h< height; ++h)
	{
		double* Jnddata = JndVal.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			g_jnd[h][w] = Jnddata[w];
	}
	g_getBasicImformation(width, height);
	calcWeight(width, height);
	return;
}
void   g_getBasicImformation(int picWidth, int picHeight)
{
	if (picWidth % 64 == 0)
	{
		cuWsize = picWidth / 64;
		cuWmargin = 64;
	}
	else
	{
		cuWsize = picWidth / 64 + 1;
		cuWmargin = picWidth % 64;
	}
	if (picHeight % 64 == 0)
	{
		cuHsize = picHeight / 64;
		cuHmargin = 64;
	}
	else
	{
		cuHsize = picHeight / 64 + 1;
		cuHmargin = picHeight % 64;
	}
	g_OneDBlcokJnd = new double[cuWsize*cuHsize];
}
//根据JND的值计算权重
void calcWeight(int picWidth, int picHeight)
{
	BlockJnd = new double*[cuHsize];  //开辟空间存储每一个块的JND的值
	if (BlockJnd == NULL)
	{
		cout << "Failed to get JND menory" << endl;
		exit(1);
	}
	for (int j = 0; j < cuHsize; j++)
	{
		BlockJnd[j] = new double[cuWsize];
		if (BlockJnd[j] == NULL)
		{
			cout << "Failed to get JND menory" << endl;
			exit(1);
		}
		memset(BlockJnd[j], 0, cuWsize*sizeof(double));
	}
	for (int i = 0; i < cuHsize - 1; i++)
	for (int j = 0; j < cuWsize - 1; j++)
	{
		for (int ii = 0; ii < 64; ii++)
		for (int jj = 0; jj < 64; jj++)
			BlockJnd[i][j] += g_jnd[ i * 64 + ii][j * 64 + jj];
		BlockJnd[i][j] = BlockJnd[i][j] / pow(64, 2.0);
	}
	//end of for 
	//下面的代码开始处理边缘块

	//处理行边缘
	for (int i = 0; i < cuHsize - 1; i++)
	{
		for (int ii = 0; ii < 64; ii++)
		for (int jj = 0; jj < cuWmargin; jj++)
			BlockJnd[i][cuWsize - 1] += g_jnd[ i * 64 + ii][(cuWsize - 1) * 64 + jj];
		BlockJnd[i][cuWsize - 1] = BlockJnd[i][cuWsize - 1] / (cuWmargin * 64);
	}

	//处理列边缘
	for (int j = 0; j < cuWsize - 1; j++)
	{
		for (int ii = 0; ii < cuHmargin; ii++)
		for (int jj = 0; jj < 64; jj++)
			BlockJnd[cuHsize - 1][j] += g_jnd[ + (cuHsize - 1) * 64 + ii][j * 64 + jj];
		BlockJnd[cuHsize - 1][j] = BlockJnd[cuHsize - 1][j] / (cuHmargin * 64);
	}
	//处理右下角最后一个块
	for (int ii = 0; ii < cuHmargin; ii++)
	{
		for (int jj = 0; jj < cuWmargin; jj++)
			BlockJnd[cuHsize - 1][cuWsize - 1] += g_jnd[ + (cuHsize - 1) * 64 + ii][(cuWsize - 1) * 64 + jj];
		BlockJnd[cuHsize - 1][cuWsize - 1] = BlockJnd[cuHsize - 1][cuWsize - 1] / (cuHmargin * cuWmargin);
	}
	for (int i = 0; i < picHeight; i++)
	for (int j = 0; j < picWidth; j++)
	{
		FramJnd += g_jnd[i][j];
	}
	FramJnd = FramJnd / (picHeight*picWidth);
	double *flag = g_OneDBlcokJnd;
	for (int i = 0; i < cuHsize; i++)
	for (int j = 0; j < cuWsize; j++)
	{
		BlockJnd[i][j] = 0.7 + 0.6 / (1 + exp(-4 * (BlockJnd[i][j] - FramJnd) / FramJnd));
		*flag = BlockJnd[i][j];
		flag++;

	}
	for (int j = 0; j < cuHsize; j++)  //释放开辟的空间
	{
		delete[] BlockJnd[j];

	}
	delete[] BlockJnd;
}
