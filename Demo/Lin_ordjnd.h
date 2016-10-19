#ifndef _ORDJND_H
#define _ORDJND_H
#include "cv.h"
#include "highgui.h"
using namespace cv;
//Mat Convet2Doule(const Mat IputImg);
Mat GetOrdJND(Mat img);
Mat GetLaJnd(Mat img);
Mat GetCmJnd( Mat img);
Mat GetBgLum32( Mat img);
Mat GetAdjLum( Mat JndBgLum);
Mat GetBgJnd();
Mat GetLogDif( Mat img);
Mat GetLumDif(Mat img);
//Mat Filter2(char*B[],Mat img);
#endif // !