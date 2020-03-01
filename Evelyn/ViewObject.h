#pragma once

#include "framework.h"

#include <string>

BOOL PointInRect(POINT p, RECT rect);
using namespace cv;
using namespace std;



#define MEVENT_CLICK 0
#define MEVENT_MOVEIN 1
#define MEVENT_MOVEOUT 2


// 定义变量

class ViewObject
{
public:
	ViewObject(RECT area, HWND wnd);
	~ViewObject();

	friend LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam);
	BOOL setBkground(Mat path,Mat viewBuf, double Tranl);
	Mat m_Bkground;
 	BOOL setBkmusic(string path);
	BOOL Text2Img(string text, int x, int y, int r, int g, int b);
	string m_Bkmusic = "";
	BOOL addElement(Mat elementIni, POINT area, UINT msg,double Tranl);
	BOOL complete(HDC m_DC);
	BOOL redraw(HDC dc);
	int Mat2CImage(Mat* mat, CImage& img);

	CImage m_Image;
	void clear();


	Mat View;
	HHOOK hMouse = NULL;
	RECT ShowArea;
	HWND m_hWnd;

	RECT eventArea[100];
	BOOL isInArea[100];
	int eventCount = 0;
	string TextPath = "";
	UINT msgList[100];
	LPSTR musicPath = NULL;
private:
	void  FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	void DrawMatToDC(HDC MemDC, const Mat& tmpMat);

	void BitMatToWnd(HWND hWnd, cv::Mat img, RECT Roii);
};
