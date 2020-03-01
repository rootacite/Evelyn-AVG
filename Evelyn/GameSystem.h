#pragma once
#include "Evelyn.h"

HDC m_DC;
HWND mhwnd;
ViewObject* thisview = NULL;   //主帧，将循环更新

LRESULT CALLBACK MouseEventProc(int nCode, WPARAM wParam, LPARAM lParam) {
	POINT pm;
	GetCursorPos(&pm);
	ScreenToClient(mhwnd, &pm);

	for (int i = 0; i < thisview->eventCount; i++) {
		if (PointInRect(pm, thisview->eventArea[i])) {
			if (!thisview->isInArea[i]) {
				SendMessage(thisview->m_hWnd, thisview->msgList[i], MEVENT_MOVEIN, NULL);
			}
			thisview->isInArea[i] = TRUE;
			switch (wParam)
			{
			case WM_LBUTTONUP:
				SendMessage(thisview->m_hWnd, thisview->msgList[i], MEVENT_CLICK, NULL);
				//MessageBox(0,"","",0);
				break;
			default:
				break;
			}

		}
		else {
			if (thisview->isInArea[i]) {
				SendMessage(thisview->m_hWnd, thisview->msgList[i], MEVENT_MOVEOUT, NULL);
			}
			thisview->isInArea[i] = false;
		}
	}
	return CallNextHookEx(thisview->hMouse, nCode, wParam, lParam);
}

DWORD WINAPI SpeedTheead(LPVOID lPvoid) {
	POINT  p;
	while (1) {
		//thisview->clear();
		//thisview->setBkground("timg.jpg");
		//thisview->addElement("sue.png", p, VIEW_EVENT_TEST);
		thisview->redraw(m_DC);

		//ioi++;
		Sleep(5);
	}
	return 0;
}

class GameSystem
{
public:
	GameSystem();
	~GameSystem();


private:

};

GameSystem::GameSystem()
{
}

GameSystem::~GameSystem()
{
}
