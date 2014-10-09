// ResizeChildCtrl.cpp: implementation of the CResizeChildCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizeChildCtrl.h"
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResizeChildCtrl::CResizeChildCtrl()
{

}

CResizeChildCtrl::~CResizeChildCtrl()
{

}

void CResizeChildCtrl::ResizeClildCtrl(HWND dlgHwnd, CRect primalClientRect, CRect resultClientRect)
{
	double dRadioX = (double)resultClientRect.Width() / (double)primalClientRect.Width();
	double dRadioY = (double)resultClientRect.Height() / (double)primalClientRect.Height();

	RESIZEINFO resizeInfo;
	resizeInfo.dResizeRadioX = dRadioX;
	resizeInfo.dResizeRadioY = dRadioY;
	resizeInfo.parHwnd = dlgHwnd;

	// 使用系统提供的这个遍历可以避免GetWindowChild函数的死循环
	EnumChildWindows(dlgHwnd, CResizeChildCtrl::ResizeChildPor, (LPARAM)&resizeInfo);
}

BOOL CALLBACK CResizeChildCtrl::ResizeChildPor(HWND hwndChild, LPARAM lParam)
{
	RESIZEINFO* resizeInfo = (RESIZEINFO*)lParam;

	CRect rectChildWin;
	::GetWindowRect(hwndChild, &rectChildWin); // 取到的Rect是屏幕的坐标，需要将它转化为父窗口的客户区坐标

	// 转换运算
	CPoint lefttop, rightbot;
	lefttop.x = rectChildWin.left;
	lefttop.y = rectChildWin.top;
	rightbot.x = rectChildWin.right;
	rightbot.y = rectChildWin.bottom;
	ScreenToClient(resizeInfo->parHwnd, &lefttop);
	ScreenToClient(resizeInfo->parHwnd, &rightbot);

	// 此方法位置参数是相对父窗口的(原来以为和GetWindowRect类似也是相对屏幕..)
	::SetWindowPos(hwndChild, NULL,
		(int)(lefttop.x * resizeInfo->dResizeRadioX),
		(int)(lefttop.y * resizeInfo->dResizeRadioY),
		(int)((rightbot.x - lefttop.x) * resizeInfo->dResizeRadioX), // new width
		(int)((rightbot.y - lefttop.y) * resizeInfo->dResizeRadioY), // new height
		NULL);

	return TRUE;
}