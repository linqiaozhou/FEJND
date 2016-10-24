#include "opencv2/opencv.hpp"
#include"Lin_GetJnd.h"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include"Lin_disjnd.h"
using namespace cv;
extern double**  g_jnd;    //����һ��ȫ�ֱ������洢JND��ֵ
extern double ** BlockJnd; //��¼һ֡���JND��ƽ��ֵ
extern int cuWsize ;
extern int cuHsize ;
extern int cuWmargin ;
extern int cuHmargin ;
extern double *g_OneDBlcokJnd; //����һ���ռ���������ÿһ֡�ĵ����� ƽ��JND��ֵ
void JndPro(Mat orgImg)
{
	int height = orgImg.rows;
	int width = orgImg.cols;
	orgImg.convertTo(orgImg, CV_64FC1, 1.0);
	Mat preImg = GetPreImg(orgImg);
	Mat FreEng = abs(orgImg - preImg);

	Mat ordJnd = GetOrdJND(preImg);
	Mat disJnd = GetDisJnd(orgImg, FreEng);
	Mat JndVal = ordJnd + disJnd - 0.3 * min(disJnd, ordJnd);
	//JndVal = ordJnd;
	/*double min0, max0;
	minMaxLoc(disJnd, &min0, &max0);
	disJnd.convertTo(disJnd, CV_64FC1, 1.0 / max0);
	imshow("DisJnd", disJnd);
	waitKey(0);


	double min1, max1;
	minMaxLoc(JndVal, &min1, &max1);
	JndVal.convertTo(JndVal, CV_64FC1, 1.0 / max1 );
	imshow("FinalJnd", JndVal);
	waitKey(0);*/
	


	for (int h = 0; h< height; ++h)
	{
		double* Jnddata = JndVal.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			g_jnd[h][w] = Jnddata[w];
	}
	calcWeight(width, height);
	return;
}

//����JND��ֵ����Ȩ��
void calcWeight(int picWidth, int picHeight)
{
	double FramJnd = 0;
	for (int i = 0; i < cuHsize - 1; i++)
	for (int j = 0; j < cuWsize - 1; j++)
	{
		for (int ii = 0; ii < 64; ii++)
		for (int jj = 0; jj < 64; jj++)
			BlockJnd[i][j] += g_jnd[ i * 64 + ii][j * 64 + jj];
		BlockJnd[i][j] = BlockJnd[i][j] / pow(64, 2.0);
	}
	//end of for 
	//����Ĵ��뿪ʼ�����Ե��

	//�����б�Ե
	for (int i = 0; i < cuHsize - 1; i++)
	{
		for (int ii = 0; ii < 64; ii++)
		for (int jj = 0; jj < cuWmargin; jj++)
			BlockJnd[i][cuWsize - 1] += g_jnd[ i * 64 + ii][(cuWsize - 1) * 64 + jj];
		BlockJnd[i][cuWsize - 1] = BlockJnd[i][cuWsize - 1] / (cuWmargin * 64);
	}

	//�����б�Ե
	for (int j = 0; j < cuWsize - 1; j++)
	{
		for (int ii = 0; ii < cuHmargin; ii++)
		for (int jj = 0; jj < 64; jj++)
			BlockJnd[cuHsize - 1][j] += g_jnd[ + (cuHsize - 1) * 64 + ii][j * 64 + jj];
		BlockJnd[cuHsize - 1][j] = BlockJnd[cuHsize - 1][j] / (cuHmargin * 64);
	}
	//�������½����һ����
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
}
