#include"Lin_ordjnd.h"
#include"cv.h"
#define MinLum 32
Mat GetBgLum32( Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	assert(img.channels() == 1);    //此代码只处理灰度图像
	Mat kern = (Mat_<double>(5, 5) << 1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32,
		1.0 / 32, 2.0 / 32, 2.0 / 32, 2.0 / 32, 1.0 / 32,
		1.0 / 32, 2.0 / 32, 0.0 / 32, 2.0 / 32, 1.0 / 32,
		1.0 / 32, 2.0 / 32, 2.0 / 32, 2.0 / 32, 1.0 / 32,
		1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32);
	Mat filImg;
	filter2D(img, filImg, img.depth(), kern);
	int height = filImg.rows;
	int width = filImg.cols;
	for (int h = 0; h < height; ++h)
	{
		double* orgdata = filImg.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			orgdata[w] = cvFloor(orgdata[w]);
			
	}
	return filImg;
}
Mat GetAdjLum( Mat img)
{
	int height = img.rows;
	int width = img.cols;
//	Mat adjImg(height, width,CV_64FC1);
	Mat adjImg = img;
	for (int h = 0; h < height; ++h)
	{
		double* orgdata = img.ptr<double>(h);
		double* adjdata = adjImg.ptr<double>(h);
		for (int w = 0; w < width; ++w)
		{
			if (orgdata[w] <= 127)
				adjdata[w] = round(MinLum + orgdata[w] * (double)(127 - MinLum) / 127);
		}
		   
	}
	return adjImg;
}
Mat GetBgJnd()
{
	Mat BgJnd(256, 1, CV_64FC1);
	double T0 = 17.0;
	double gamma = 3.0 / 128;
	for (int lum = 0; lum < 256; ++lum)
	{
		if (lum<=127)
			BgJnd.at<double>(lum, 0) = T0 * (1 - sqrt((double)lum / 127));
		else
			BgJnd.at<double>(lum, 0) = gamma * ((double)lum - 127) + 3;
	}

	return BgJnd;
}
Mat GetLumDif( Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	assert(img.channels() == 1);    //此代码只处理灰度图像
	Mat kern1 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0,
		1.0 , 3.0 , 8.0 , 3.0 , 1.0,
		0, 0, 0, 0, 0,
		-1.0 , -3.0 , -8.0 , -3.0 , -1.0,
		0, 0, 0, 0, 0);
	Mat kern2 = (Mat_<double>(5, 5) << 0, 0, 1.0 , 0, 0,
		0, 8.0 , 3.0 , 0, 0,
		1.0 , 3.0 , 0, -3.0 , -1.0 ,
		0, 0, -3.0 , -8.0 , 0,
		0, 0, -1.0 , 0, 0);
	Mat kern3 = (Mat_<double>(5, 5) << 0, 0, 1.0 , 0, 0,
		0, 0, 3.0 , 8.0 , 0,
		-1.0 , -3.0 , 0, 3.0 , 1.0 ,
		0, -8.0 , -3.0 , 0, 0,
		0, 0, -1.0 , 0, 0);
	Mat kern4 = (Mat_<double>(5, 5) << 0, 1.0 , 0, -1.0 , 0,
		0, 3.0 , 0, -3.0  , 0,
		0, 8.0 , 0, -8.0 , 0,
		0, 3.0 , 0, -3.0 , 0,
		0, 1.0 , 0, -1.0 , 0);
	int height = img.rows;
	int width = img.cols;
	Mat Grad1 , Grad2 , Grad3 , Grad4 ;
	filter2D(img, Grad1, img.depth(), kern1);
	filter2D(img, Grad2, img.depth(), kern2);
	filter2D(img, Grad3, img.depth(), kern3);
	filter2D(img, Grad4, img.depth(), kern4);
	Grad1 = Grad1 / 16;
	Grad2 = Grad2 / 16;
	Grad3 = Grad3 / 16;
	Grad4 = Grad4 / 16;

	Mat LumDif =Grad1;
	for (int h = 0; h <height; ++h)
	{  
		double* g1 = Grad1.ptr<double>(h);
		double* g2 = Grad2.ptr<double>(h);
		double* g3 = Grad3.ptr<double>(h);
		double* g4 = Grad4.ptr<double>(h);
		double* res = LumDif.ptr<double>(h);	
		for (int w = 0; w < width; ++w)
		{
			double max = 0;
			if (abs(g1[w])>max)                        
				max = abs(g1[w]);
			if (abs(g2[w])>max)
				max = abs(g2[w]);
			if (abs(g3[w])>max)
				max = abs(g3[w]);
			if (abs(g4[w])>max)
				max = abs(g4[w]);
			res[w] = max;

		}
	}
	return LumDif;
}
Mat  GetLogDif( Mat img)
{
	Mat LogDif = img;
	int height = img.rows;
	int width = img.cols;
	int Th = 80;
	for (int h = 0; h < height; ++h)
	{
		const double* orgdata = img.ptr<double>(h);
		double* logdata = LogDif.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			logdata[w] = Th*log10(1 + (float)orgdata[w] / Th) / log10(4);;
	}
	return LogDif;
}
Mat GetLaJnd( Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	Mat JndBgLum = GetBgLum32(img);
	Mat AdjJndLum = GetAdjLum(JndBgLum);
	Mat BgJnd = GetBgJnd();
	Mat LaJnd(img.rows,img.cols,CV_64FC1);
	int height = img.rows;
	int width = img.cols;
	for (int h = 0; h < height; ++h)
	{
		double* jndgdata = LaJnd.ptr<double>(h);
		double* lumdata = AdjJndLum.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			jndgdata[w] = BgJnd.at<double>(lumdata[w], 0);			
	}
	return LaJnd;
}
Mat GetCmJnd( Mat img)
{
	Mat LumDif = GetLumDif(img);
	Mat logDiff = GetLogDif(LumDif);
	Mat BgLum = GetBgLum32(img);
	float landa = 0.5;
	Mat alpha = 0.0001*BgLum + 0.115;
	Mat belta = landa - 0.01*BgLum;
	LumDif = LumDif.mul(alpha);
	Mat JndCM = abs(LumDif + belta);
	return JndCM;
}
Mat GetOrdJND(Mat img)
{
	
	Mat JndLa = GetLaJnd(img);
	Mat JndCm = GetCmJnd(img);
	Mat FinalJnd = JndLa + JndCm - 0.3 * min(JndLa, JndCm);
	return FinalJnd;
}
