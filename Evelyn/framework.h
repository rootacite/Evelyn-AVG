// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <afxwin.h>
#include <atlimage.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <fstream>
#include <iostream>

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define VIEW_EVENT_TEST (WM_USER)+100
#define VIEW_EVENT_START WM_USER+101
#define VIEW_EVENT_EXIT WM_USER+102
#define VIEW_EVENT_NEXT WM_USER+103

