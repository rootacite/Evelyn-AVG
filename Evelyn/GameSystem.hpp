#pragma once

#include "ViewObject.h"
using namespace std;

HDC m_DC;
HWND mhwnd;
int nowFarm = 0;



LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI SpeedTheead(LPVOID lPvoid);

struct ElementItem {
	POINT position;
	//char path[255];
	Mat path;
	int ChangeTime = 1;
};

struct ButtonItem {
	UINT msg;
	POINT position;	
	Mat path1;
	Mat path2;
};

struct ViewItem {
	Mat BackGround;
	//char BackGround[255] = {0};
	char music[255] = {0};
	ButtonItem* buttons = NULL;
	int buttonCount = 0;
	ElementItem* elements = NULL;
	int elementCount = 0;

	int autoChange = 0;
	int BackGroundchangeTime = 1;
	char TextStr[125] = {0};

	char playonce[255] = {0};

	int tx = 0;
	int ty = 0;
};

ViewItem Allitems[1024];
//sizeof(ViewItem);

BOOL getViewformScript(int id,LPCSTR path,ViewItem*lpItem)
{
	ifstream scriptObj(path,ios::in);
	if (!scriptObj.is_open()) {
		return FALSE;
	}
	string buffer;
	char flagId[40];
	sprintf_s(flagId, "%s%d%c", "<id=", id, '>');
	while (getline(scriptObj, buffer)) {
		if (!strstr(buffer.c_str(), flagId)) continue;
		while (getline(scriptObj, buffer)) {
			const char* nPoint = NULL;
			if ((nPoint = strstr(buffer.c_str(), "<background>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0,12);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</background>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0,posofend);
				//strcpy_s(lpItem->BackGround, realData.c_str());
				lpItem->BackGround = imread(realData.c_str());
				continue;
			}
			if ((nPoint = strstr(buffer.c_str(), "<bkct>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 6);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</bkct>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);

				lpItem->BackGroundchangeTime = atoi(realData.c_str());
				continue;
			} 
			if ((nPoint = strstr(buffer.c_str(), "<playonce>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 10);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</playonce>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);

				strcpy_s(lpItem->playonce, realData.c_str());
				continue;
			}
			if ((nPoint = strstr(buffer.c_str(), "<auto>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 6);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</auto>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);
				//strcpy_s(lpItem->autoChange, realData.c_str());
				lpItem->autoChange = atoi(realData.c_str());
				continue;
			}
			if ((nPoint = strstr(buffer.c_str(), "<text>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 6);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</text>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);
				strcpy_s(lpItem->TextStr, realData.c_str());
				continue;
			}
			if ((nPoint = strstr(buffer.c_str(), "<tx>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 4);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</tx>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);
				lpItem->tx = atoi(realData.c_str());
			//	AfxMessageBox(realData.c_str());
				continue;
			}
			if ((nPoint = strstr(buffer.c_str(), "<ty>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 4);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</ty>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);
				lpItem->ty = atoi(realData.c_str());
				continue;
			}
			if ((nPoint = strstr(buffer.c_str(), "<music>")) != 0) {
				string thisPoint = nPoint;
				thisPoint.erase(0, 7);
				unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</music>") - (unsigned long long)thisPoint.c_str();
				string realData = thisPoint.substr(0, posofend);
				strcpy_s(lpItem->music, realData.c_str());
				continue;
			}

			if ((nPoint = strstr(buffer.c_str(), "<buttons=")) != 0) {
				string formLete = strstr(nPoint, "=");
				formLete.erase(0,1);
				formLete.erase(formLete.length() - 1, 1);

				lpItem->buttonCount = atoi(formLete.c_str());
				lpItem->buttons = new ButtonItem[lpItem->buttonCount];
				int thisCountOfb = 0;
				while (getline(scriptObj, buffer)) {
					if (strstr(buffer.c_str(), "</buttons>") != 0)break;
					if (strstr(buffer.c_str(), "<button>") != 0)
						while (getline(scriptObj, buffer)) {
							if (strstr(buffer.c_str(), "</button>") != 0) {
								thisCountOfb++;
								break;
							}
							if ((nPoint = strstr(buffer.c_str(), "<path1>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 7);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</path1>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								//strcpy_s(lpItem->buttons[thisCountOfb].path1, realData.c_str());
								lpItem->buttons[thisCountOfb].path1 = imread(realData.c_str(),-1);
								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<path2>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 7);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</path2>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								//strcpy_s(lpItem->buttons[thisCountOfb].path2, realData.c_str());
								lpItem->buttons[thisCountOfb].path2 = imread(realData.c_str(),-1);
							//	thisCountOfb++;
								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<event>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 7);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</event>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								lpItem->buttons[thisCountOfb].msg = atoi(realData.c_str()) + WM_USER + 100;
								//thisCountOfb++;
								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<px>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 4);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</px>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								lpItem->buttons[thisCountOfb].position.x = atoi(realData.c_str());
								//thisCountOfb++;
								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<py>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 4);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</py>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								lpItem->buttons[thisCountOfb].position.y = atoi(realData.c_str());
								//thisCountOfb++;
								continue;
							}
						}
				}
			}
			if ((nPoint = strstr(buffer.c_str(), "<elements=")) != 0) {
				string formLete = strstr(nPoint, "=");
				formLete.erase(0, 1);
				formLete.erase(formLete.length() - 1, 1);

				lpItem->elementCount = atoi(formLete.c_str());
				lpItem->elements = new ElementItem[lpItem->elementCount];
				int thisCountOfb = 0;
				while (getline(scriptObj, buffer)) {
					if (strstr(buffer.c_str(), "</elements>") != 0)break;
					if (strstr(buffer.c_str(), "<element>") != 0)
						while (getline(scriptObj, buffer)) {
							if (strstr(buffer.c_str(), "</element>") != 0) {
								thisCountOfb++;
								break;
							}
							if ((nPoint = strstr(buffer.c_str(), "<path>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 6);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</path>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								//strcpy_s(lpItem->elements[thisCountOfb].path, realData.c_str());
								lpItem->elements[thisCountOfb].path = imread(realData.c_str(),-1);

								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<elct>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 6);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</elct>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);

								lpItem->elements[thisCountOfb].ChangeTime = atoi(realData.c_str());
								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<px>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 4);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</px>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								lpItem->elements[thisCountOfb].position.x = atoi(realData.c_str());
								//thisCountOfb++;
								continue;
							}
							if ((nPoint = strstr(buffer.c_str(), "<py>")) != 0) {
								string thisPoint = nPoint;
								thisPoint.erase(0, 4);
								unsigned long long posofend = (unsigned long long)strstr(thisPoint.c_str(), "</py>") - (unsigned long long)thisPoint.c_str();
								string realData = thisPoint.substr(0, posofend);
								lpItem->elements[thisCountOfb].position.y = atoi(realData.c_str());
								//thisCountOfb++;
								continue;
							}
						}
				}
			}
			if (strstr(buffer.c_str(), "</id>"))
				break;
		}
		return TRUE;;
	}

	return FALSE;
}

class GameSystem
{
public:
	GameSystem(HWND hWnd, RECT rect);
	~GameSystem();
	friend LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam);
	friend DWORD WINAPI SpeedTheead(LPVOID lPvoid);

	BOOL ChangeMusic(LPCSTR path);
	BOOL ChangeBackGround(Mat path,int time);
	BOOL ClearElementAndButton();
	BOOL AddElement(Mat path,POINT p, int time);
	BOOL AddButton(Mat path1, Mat path2, POINT p, UINT eventId);
	BOOL PlayMusicOnce(LPCSTR path);

	void OrderView(ViewItem *items);
	void ButtonUpdata();

	int FindButtonInArea(POINT p) {
		for (int i = 0; i < thisview->eventCount; i++) {
			if (p.x == thisview->eventArea[i].left && p.y == thisview->eventArea[i].top)
				return i;
		}
		return -1;
	}

	Mat buttonIconon[100];
	Mat buttonIconoff[100];
	int buttonCount = 0;
	UINT buttonEvents[100];
	POINT buttonPoint[100];
	BOOL isInAreaOfButton[100];
	BOOL autoRedraw = TRUE;

	Mat elementsIcon[100];
	string m_text = "";

	int mtx;
	int mty;
	int elementsCount = 0;
	POINT elementsPoint[100];
private:
	ViewObject* thisview = NULL;   //主帧，将循环更新
};

DWORD WINAPI AutoChangeFlah(LPVOID lPvoid) {
	int* Seconds = (int*)lPvoid;

	Sleep(*Seconds);
	SendMessage(mhwnd, VIEW_EVENT_NEXT, MEVENT_CLICK, 0);
	delete Seconds;
	return 0;
}

void GameSystem::OrderView(ViewItem *items) {
	autoRedraw = FALSE;
	if (!items->BackGround.empty())
		if (items->BackGroundchangeTime == 1)
			this->ChangeBackGround(items->BackGround, items->BackGroundchangeTime);
		else {
		//	autoRedraw = FALSE;
			this->ChangeBackGround(items->BackGround, items->BackGroundchangeTime);
		//	autoRedraw = TRUE;
		}
	else
        this->ChangeBackGround(thisview->m_Bkground, items->BackGroundchangeTime);
	if (this->buttonCount > 0 || this->elementsCount > 0)
		this->ClearElementAndButton();

	thisview->redraw(m_DC);
	if (items->music[0] != 0)
		this->ChangeMusic(items->music);
	thisview->redraw(m_DC);
	for (int i = 0; i < items->elementCount; i++) {
		this->AddElement(items->elements[i].path, items->elements[i].position, items->elements[i].ChangeTime);
		thisview->redraw(m_DC);
	}
	for (int i = 0; i < items->buttonCount; i++) {

		this->AddButton(items->buttons[i].path1, items->buttons[i].path2, items->buttons[i].position, items->buttons[i].msg);
		thisview->redraw(m_DC);
	}
	

	if (items->autoChange > 0){
		int* FlagNow = new int;
		*FlagNow = items->autoChange;
		HANDLE hChangeAuto = CreateThread(NULL, NULL, AutoChangeFlah, FlagNow, 0, 0);
		CloseHandle(hChangeAuto);
	}
;

	if (items->TextStr[0]!=0) {
		//thisview->Text2Img(items->TextStr, items->tx, items->ty, 255, 255, 255);
		mtx = items->tx;
		mty = items->ty;
		m_text = items->TextStr;
		thisview->Text2Img(m_text, mtx, mty, 255, 255, 255);
	}
	else {
		m_text = "";
	}

	if (items->playonce[0] != 0) {
		//thisview->Text2Img(items->TextStr, items->tx, items->ty, 255, 255, 255);
		this->PlayMusicOnce(items->playonce);
	}
	thisview->Mat2CImage(&thisview->View, thisview->m_Image);
	autoRedraw = TRUE;
}

GameSystem::GameSystem(HWND hWnd,RECT rect)
{
	thisview = new ViewObject(rect, hWnd);
	POINT p;
	p.x = 200;
	p.y = 200;


	//AfxMessageBox("正在加载资源。。");
	CImage img;
	img.Load("res/CG/-01.png");
	img.Draw(GetDC(hWnd),rect);
	Sleep(2000);
	for (int i = 0; i < 1024;i++) {
		getViewformScript(i, "script.htm", &Allitems[i]);
	}

	//AfxMessageBox("资源已经全部加载完毕。");
	m_DC=GetDC(hWnd);
	this->OrderView(&Allitems[0]);

		
	
	//this->ButtonUpdata();
}

GameSystem::~GameSystem()
{
	AfxMessageBox("error");
}

BOOL GameSystem::ChangeMusic(LPCSTR path) {

	this->ButtonUpdata();
	thisview->clear();
	thisview->setBkground(thisview->m_Bkground,thisview->m_Bkground,1.0);
	if (m_text != "")
		thisview->Text2Img(m_text.c_str(),mtx,mty, 255, 255, 255);
	for (int i = 0; i < elementsCount; i++)
		thisview->addElement(elementsIcon[i], elementsPoint[i], NULL,1);
	for (int i = 0; i < buttonCount; i++)
		thisview->addElement(isInAreaOfButton[i] ?buttonIconon[i] : buttonIconoff[i], buttonPoint[i], buttonEvents[i],1);
	thisview->setBkmusic(path);
	thisview->complete(m_DC);



	return 0;
}

BOOL GameSystem::ChangeBackGround(Mat path,int time)
{

    this->ButtonUpdata();

	Mat LoadNow;
	path.copyTo(LoadNow);

	Mat SaveLast ;
	thisview->View.copyTo(SaveLast);

	if(time==1)
		thisview->setBkground(SaveLast,LoadNow,1.0);
	else
		for (int i = 0; i <= time; i++) {
			SaveLast.copyTo(thisview->View);
			thisview->setBkground(SaveLast,LoadNow,( 1.0/time)*i);
			thisview->redraw(m_DC);
			if (time != 1)
			thisview->Mat2CImage(&thisview->View, thisview->m_Image);
			Sleep(20);
		//	AfxMessageBox("jjjjjj");
		}
	thisview->clear();
	//Sleep(45);
	//if (m_text != "")
	//	thisview->Text2Img(m_text.c_str(), mtx, mty, 255, 255, 255);
	//for (int i = 0; i < elementsCount; i++)
	//	thisview->addElement(elementsIcon[i].c_str(), elementsPoint[i], NULL);
	//for (int i = 0; i < buttonCount; i++)
	//	thisview->addElement(isInAreaOfButton[i] ? buttonIconon[i].c_str() : buttonIconoff[i].c_str(), buttonPoint[i], buttonEvents[i]);
	thisview->complete(m_DC);

	
	return 0;
}

inline BOOL GameSystem::ClearElementAndButton()
{

	thisview->clear();
	thisview->setBkground(thisview->m_Bkground,thisview->m_Bkground,1.0);
	thisview->complete(m_DC);

	buttonCount = 0;
	m_text = "";
	elementsCount = 0;
	return 0;
}

inline BOOL GameSystem::AddElement(Mat path, POINT p,int time)
{
	Mat SaveView;
	thisview->View.copyTo(SaveView);

	Mat LoadNow;
	path.copyTo(LoadNow);
	for (int i = 0; i <= time; i++) {
		//thisview->View= SaveView;
		//memcpy(&SaveView,&thisview->View,sizeof(Mat));
		SaveView.copyTo(thisview->View);
		//imshow("showview", SaveView);
		//imshow("view", thisview->View);
		thisview->addElement(LoadNow, p, NULL, (1.0 / time)*i);
		if(time!=1)
		thisview->Mat2CImage(&thisview->View, thisview->m_Image);
		thisview->redraw(m_DC);
		Sleep(20);

	}
	 path.copyTo(elementsIcon[elementsCount]);
	elementsPoint[elementsCount] = p;
	elementsCount++;
	
	return 0;
}

inline BOOL GameSystem::AddButton(Mat path1, Mat path2, POINT p,UINT eventId)
{

	//thisview->addElement(path1, p, eventId);
	path1.copyTo(buttonIconon[buttonCount]);
	path2.copyTo(buttonIconoff[buttonCount]);
	buttonEvents[buttonCount] = eventId;
	buttonPoint[buttonCount] = p;
	buttonCount++;
	this->ButtonUpdata();

	
	thisview->clear();

	thisview->setBkground(thisview->m_Bkground,thisview->m_Bkground,1.0);
	if (m_text != "")
		thisview->Text2Img(m_text.c_str(), mtx, mty, 255, 255, 255);
	for (int i = 0; i < elementsCount; i++)
		thisview->addElement(elementsIcon[i], elementsPoint[i], NULL,1);
	for (int i = 0; i < buttonCount; i++)
		thisview->addElement(isInAreaOfButton[i] ? buttonIconon[i] : buttonIconoff[i], buttonPoint[i], buttonEvents[i],1);
	thisview->complete(m_DC);
	return 0;
}

inline BOOL GameSystem::PlayMusicOnce(LPCSTR path)
{
	PlaySound(path, NULL, SND_FILENAME | SND_ASYNC);
	return 0;
}

inline void GameSystem::ButtonUpdata()
{
	for (int i = 0; i < this->buttonCount; i++) {
		isInAreaOfButton[i] = thisview->isInArea[FindButtonInArea(this->buttonPoint[i])];
	}

/*	thisview->clear();
	thisview->setBkground(thisview->m_Bkground.c_str());
	for (int i = 0; i < elementsCount; i++)
		thisview->addElement(elementsIcon[i].c_str(), elementsPoint[i], NULL);
	for (int i = 0; i < buttonCount; i++)
		thisview->addElement(isInAreaOfButton[i]? buttonIconon[i].c_str(): buttonIconoff[i].c_str(), buttonPoint[i], buttonEvents[i]);
	thisview->complete(m_DC);*/
}

GameSystem* sysObject = NULL;

LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam) {
	POINT pm;
	GetCursorPos(&pm);
	ScreenToClient(mhwnd, &pm);

	switch (wParam) {
	case WM_MOUSEMOVE:
		for (int i = 0; i < sysObject->thisview->eventCount; i++)
			if (PointInRect(pm, sysObject->thisview->eventArea[i])) {
				
				if (!sysObject->thisview->isInArea[i])
				{
					sysObject->thisview->isInArea[i] = TRUE;
					sysObject->ButtonUpdata();


					sysObject->thisview->clear();
					sysObject->thisview->setBkground(sysObject->thisview->m_Bkground,sysObject->thisview->m_Bkground, 1.0);
					if (sysObject->m_text != "")
						sysObject->thisview->Text2Img(sysObject->m_text.c_str(),sysObject->mtx, sysObject->mty, 255, 255, 255);
					for (int i = 0; i < sysObject->elementsCount; i++)
						sysObject->thisview->addElement(sysObject->elementsIcon[i], sysObject->elementsPoint[i], NULL,1);
					for (int i = 0; i < sysObject->buttonCount; i++)
						sysObject->thisview->addElement(sysObject->isInAreaOfButton[i] ? sysObject->buttonIconon[i] : sysObject->buttonIconoff[i], sysObject->buttonPoint[i], sysObject->buttonEvents[i],1);
					sysObject->thisview->complete(m_DC);
					sysObject->thisview->Mat2CImage(&sysObject->thisview->View, sysObject->thisview->m_Image);
					SendMessage(sysObject->thisview->m_hWnd, sysObject->thisview->msgList[i], MEVENT_MOVEIN, NULL);
				
				}
				
			}
			else {
			
				if (sysObject->thisview->isInArea[i]) {
					sysObject->thisview->isInArea[i] = FALSE;
					sysObject->ButtonUpdata();
					sysObject->thisview->clear();
					sysObject->thisview->setBkground(sysObject->thisview->m_Bkground,sysObject->thisview->m_Bkground, 1.0);
					if (sysObject->m_text != "")
						sysObject->thisview->Text2Img(sysObject->m_text.c_str(), sysObject->mtx, sysObject->mty, 255, 255, 255);
					for (int i = 0; i < sysObject->elementsCount; i++)
						sysObject->thisview->addElement(sysObject->elementsIcon[i], sysObject->elementsPoint[i], NULL,1);
					for (int i = 0; i < sysObject->buttonCount; i++)
						sysObject->thisview->addElement(sysObject->isInAreaOfButton[i] ? sysObject->buttonIconon[i] : sysObject->buttonIconoff[i], sysObject->buttonPoint[i], sysObject->buttonEvents[i],1);
					sysObject->thisview->complete(m_DC);
                    sysObject->thisview->Mat2CImage(&sysObject->thisview->View, sysObject->thisview->m_Image);
					SendMessage(sysObject->thisview->m_hWnd, sysObject->thisview->msgList[i], MEVENT_MOVEOUT, NULL);
					
				}
					
			
			}
		break;
	case WM_LBUTTONDOWN:
		for (int i = 0; i < sysObject->thisview->eventCount; i++)
			if (PointInRect(pm, sysObject->thisview->eventArea[i])) {
					SendMessage(sysObject->thisview->m_hWnd, sysObject->thisview->msgList[i], MEVENT_CLICK, NULL);
			}
		break;
	default:
		break;
	}

	return CallNextHookEx(sysObject->thisview->hMouse, nCode, wParam, lParam);

}

DWORD WINAPI SpeedTheead(LPVOID lPvoid) {
	HDC dc=GetDC(sysObject->thisview->m_hWnd);
	while (1) {
		//thisview->clear();
		//thisview->setBkground("timg.jpg");
		//thisview->addElement("sue.png", p, VIEW_EVENT_TEST);
		if(sysObject->autoRedraw)sysObject->thisview->redraw(dc);

		//ioi++;
		Sleep(45);
	}
	return 0;
}