#pragma once

#include "ViewObject.h"

LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam);



BOOL ViewObject::redraw(HDC dc)
{
	if (View.empty())
		return 0;
	//BitMatToWnd(this->m_hWnd,View,this->ShowArea);

	m_Image.Draw(dc,ShowArea);
	return 0;
}

int ViewObject::Mat2CImage(Mat* mat, CImage& img)
{
	if (!mat || mat->empty())
		return -1;
	int nBPP = mat->channels() * 8;

	if (!img.IsNull())
		img.Destroy();

	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols * mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)
		psrc += mat->step;
		pdst += imgPitch;
	}
	return 0;
}

void overlayImage(const cv::Mat& background, const cv::Mat& foreground,
	cv::Mat& output, cv::Point2i location)
{
	background.copyTo(output);


	// start at the row indicated by location, or at row 0 if location.y is negative.
	for (int y = std::max(location.y, 0); y < background.rows; ++y)
	{
		int fY = y - location.y; // because of the translation

		// we are done of we have processed all rows of the foreground image.
		if (fY >= foreground.rows)
			break;

		// start at the column indicated by location,

		// or at column 0 if location.x is negative.
		for (int x = std::max(location.x, 0); x < background.cols; ++x)
		{
			int fX = x - location.x; // because of the translation.

			// we are done with this row if the column is outside of the foreground image.
			if (fX >= foreground.cols)
				break;

			// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
			double opacity =
				((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

				/ 255.;


			// and now combine the background and foreground pixel, using the opacity,

			// but only if opacity > 0.
			for (int c = 0; opacity > 0 && c < output.channels(); ++c)
			{
				unsigned char foregroundPx =
					foreground.data[fY * foreground.step + fX * foreground.channels() + c];
				unsigned char backgroundPx =
					background.data[y * background.step + x * background.channels() + c];
				output.data[y * output.step + output.channels() * x + c] =
					backgroundPx * (1. - opacity) + foregroundPx * opacity;
			}
		}
	}
}


ViewObject::ViewObject(RECT area, HWND wnd)
{
	ShowArea = area;
	this->m_hWnd = wnd;
}
ViewObject::~ViewObject()
{
	if (hMouse != NULL) {
		UnhookWindowsHookEx(hMouse);
	}
}

BOOL PointInRect(POINT p, RECT rect) {
	if ((p.x > rect.left && p.x < rect.right) && (p.y > rect.top && p.y < rect.bottom))
		return TRUE;
	else
		return FALSE;
}
BOOL ViewObject::addElement(Mat elementIni, POINT area, UINT msg, double Tranl) {
	
//	Mat elementIni = imread(path.c_str(),-1);
	if (elementIni.empty() || View.empty()) {
		return FALSE;
	}

	//Mat imageROI = View(Rect(area.x, area.y, elementIni.cols, elementIni.rows));
	Mat Buffer;
	overlayImage(View, elementIni, Buffer, Point2i(area.x, area.y));
	//Buffer.copyTo(View);
	addWeighted(View,1- Tranl, Buffer, Tranl, 0.0, View);
	if (msg != NULL) {
		RECT onceRect;
		onceRect.left = area.x;
		onceRect.top = area.y;
		onceRect.right = area.x + elementIni.cols;
		onceRect.bottom = area.y + elementIni.rows;

		eventArea[eventCount] = onceRect;
		msgList[eventCount] = msg;

		//POINT pMou;
		//GetCursorPos(&pMou);
		//isInArea[eventCount] = PointInRect(pMou, eventArea[eventCount]);
		eventCount++;

	}
	//Mat2CImage(&View,m_Image);
	return TRUE;
}


BOOL ViewObject::setBkground(Mat path,Mat viewBuf, double Tranl)
{
	//	MessageBox(0,path,"",0);
//	Mat viewBuf = imread(path);
	Mat lastBuf;

	View.copyTo(lastBuf);
	Mat Buffer3;
	Mat Buffer2;
	Mat Buffer1;
	if (viewBuf.empty()) {
		return FALSE;
	}

	
	if (!lastBuf.empty()) {
		resize(viewBuf, Buffer2, Size(ShowArea.right - ShowArea.left, ShowArea.bottom - ShowArea.top));
		resize(lastBuf, Buffer1, Size(ShowArea.right - ShowArea.left, ShowArea.bottom - ShowArea.top));
		addWeighted(Buffer2, Tranl, Buffer1, 1.0-Tranl, 0.0, Buffer3);
		Buffer3.copyTo(this->View);
	}
	else {
		resize(viewBuf, Buffer2, Size(ShowArea.right - ShowArea.left, ShowArea.bottom - ShowArea.top));
			addWeighted(Buffer2, Tranl, NULL, 1.0 - Tranl, 0.0, this->View);
			
	}
	//this->View = Buffer2;
	if (this->View.empty()) {
		return FALSE;
	}
	viewBuf.copyTo(this->m_Bkground);
	//Mat2CImage(&View, m_Image);
	return TRUE;
}

BOOL ViewObject::complete(HDC m_DC) {
	this->redraw(m_DC);
	if (hMouse == NULL) {
		hMouse = SetWindowsHookEx(WH_MOUSE, MouseEventProc, NULL, GetCurrentThreadId());
	}
	if (musicPath != NULL) {
		PlaySound(NULL, NULL, SND_FILENAME);
		PlaySound(musicPath, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	//Mat2CImage(&View, m_Image);
	return 0;
}

void ViewObject::clear() {
	if (hMouse != NULL) {
		UnhookWindowsHookEx(hMouse);
	}
	eventCount = 0;
	musicPath = NULL;

	hMouse = NULL;
}
void ViewObject::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}
void ViewObject::DrawMatToDC(HDC MemDC, const Mat& tmpMat)
{
	uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	int bmp_w = tmpMat.cols, bmp_h = tmpMat.rows;
	FillBitmapInfo(bmi, bmp_w, bmp_h, tmpMat.channels() * 8, -1);
	int from_x = MIN(MAX(0, 0), bmp_w - 1);
	int from_y = MIN(MAX(0, 0), bmp_h - 1);
	int sw = MAX(MIN(bmp_w - 0, tmpMat.cols), 0);
	int sh = MAX(MIN(bmp_h - 0, tmpMat.rows), 0);
	SetDIBitsToDevice(
		MemDC, 0, 0, sw, sh, from_x, from_y, from_y, sh,
		tmpMat.data + from_y * tmpMat.channels(),
		bmi, DIB_RGB_COLORS);
}
void ViewObject::BitMatToWnd(HWND hWnd, cv::Mat img, RECT Roi)
{
	if (img.empty())
		return;
	CDC* pDC = CDC::FromHandle(GetDC(hWnd));
	CDC MemDC;//首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	Gdiplus::Bitmap bitmap(img.cols, img.rows, img.cols * img.channels(), PixelFormat24bppRGB, (BYTE*)img.data);//根据Mat矩阵创建一个GDI+中的Bitmap位图

	//随后建立与屏幕显示兼容的内存显示设备
	MemDC.CreateCompatibleDC(pDC);
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小，可以用窗口的大小
	MemBitmap.CreateCompatibleBitmap(pDC, Roi.right - Roi.left, Roi.bottom - Roi.top);

	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap* pOldBit = MemDC.SelectObject(&MemBitmap);

	//先用背景色将位图清除干净，可以用自己应该用的颜色
	MemDC.FillSolidRect(0, 0, Roi.right - Roi.left, Roi.bottom - Roi.top, RGB(255, 255, 255));

	//绘图
	Gdiplus::Graphics g(MemDC.m_hDC);
	Gdiplus::Image* ii = &bitmap;
	g.DrawImage(ii, Gdiplus::Rect(0, 0, Roi.right- Roi.left, Roi.bottom- Roi.top));
	g.ReleaseHDC(MemDC.m_hDC);

	//将内存中的图拷贝到屏幕上进行显示
	pDC->BitBlt(0, 0, Roi.right - Roi.left, Roi.bottom - Roi.top, &MemDC, 0, 0, SRCCOPY);
	//绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(hWnd, pDC->m_hDC);
}
BOOL ViewObject::setBkmusic( string path) {

	if (musicPath != NULL)
		delete[] musicPath;
	musicPath = new char[path.length()+1];
	strcpy(musicPath , path.c_str());
	this->m_Bkmusic = path.c_str();

//	Mat2CImage(&View, m_Image);
	return 0;
}

BOOL ViewObject::Text2Img( string text, int x,int y,int r, int g, int b)
{	
	int font_face = cv::FONT_HERSHEY_SIMPLEX; 	
	double font_scale = 0.8;
	int thickness = 1;	
	int baseline;	//获取文本框的长宽	
	cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline); 	//将文本框居中绘制	
	cv::Mat image = View(Rect(x,y, text_size.width, text_size.height));	
	cv::Point origin; 
	origin.x = image.cols / 2 - text_size.width / 2;
	origin.y = image.rows / 2 + text_size.height / 2;	
	//cv::putText(image, text, origin, font_face, font_scale, cv::Scalar(r, g, b), thickness, 8, 0); 
	//putTextZH(image, text.c_str(), origin, cv::Scalar(r, g, b), 30);
	//Mat2CImage(&View, m_Image);
	return 0;
}

