#include<iostream>
#include <fstream>
#include"Lin_MemSet.h"
using namespace std;

double**  g_jnd;    //����һ��ȫ�ֱ������洢JND��ֵ
double ** BlockJnd; //��¼һ֡���JND��ƽ��ֵ
int cuWsize = 0;
int cuHsize = 0;
int cuWmargin = 0;
int cuHmargin = 0;
double *g_OneDBlcokJnd; //����һ���ռ���������ÿһ֡�ĵ�����ƽ��JND��ֵ
void   g_getBasicImformation(int& picHeight, int& picWidth)
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
}

void AllocateMen(int& HeightOrg, int&WidthOrg)
{
	//////////////////һ������д�Ŀ��ٿռ�Ĵ����//////////////////////////////////////////////////////

	//���ٿռ�洢ԭʼ��Jnd��ֵ
	g_jnd = new double*[HeightOrg];
	if (g_jnd == NULL)
	{
		cout << "Failed to sert mem for ALL g_jnd" << endl;
		exit(1);

	}
	for (int i = 0; i < HeightOrg; i++)
	{
		g_jnd[i] = new double[WidthOrg];
		if (g_jnd[i] == NULL)
		{
			cout << "Failed to sert mem for PART g_jnd" << endl;
			exit(1);
		}
	}
	//�õ�ͼ�����ж���LCU
	g_getBasicImformation(HeightOrg, WidthOrg);  
	//���ٿռ�洢ÿһ�����JND��ֵ
	BlockJnd = new double*[cuHsize]; 
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
	//���ڴ洢���յ�JND
	g_OneDBlcokJnd = new double[cuWsize*cuHsize]; 

}
void DestroyMen(int& HeightOrg, int&WidthOrg)
{
	////////////////////////////////////////////////////////////////////////////////////////
	//���´��������д���ͷ��ڴ��
	for (int i = 0; i < HeightOrg; i++)   //�ͷ��ڴ�
	{
		delete g_jnd[i];
		g_jnd[i] = NULL;
	}
	delete g_jnd;
	g_jnd = NULL;
	for (int j = 0; j < cuHsize; j++)  //�ͷſ��ٵĿռ�
	{
		delete[] BlockJnd[j];

	}
	delete BlockJnd;
	BlockJnd = NULL;
	delete[] g_OneDBlcokJnd;
	g_OneDBlcokJnd = NULL;
}
