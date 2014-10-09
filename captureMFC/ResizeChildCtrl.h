// ResizeChildCtrl.h: interface for the CResizeChildCtrl class.
// 子控件自适应窗口大小的改变
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZECHILDCTRL_H__7DCD7886_0472_4EC7_A859_17C5F19ACA59__INCLUDED_)
#define AFX_RESIZECHILDCTRL_H__7DCD7886_0472_4EC7_A859_17C5F19ACA59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 传递大小变化的信息
struct RESIZEINFO
{
	double dResizeRadioX;
	double dResizeRadioY;
	HWND parHwnd;
};

class CResizeChildCtrl
{
public:
	CResizeChildCtrl();
	virtual ~CResizeChildCtrl();

public:
	// 遍历子控件，更改大小
	static void ResizeClildCtrl(HWND dlgHwnd, CRect primalClientRect, CRect resultClientRect);
	// EnumChildWindows() 回调函数，更改单个子控件大小
	static BOOL CALLBACK ResizeChildPor(HWND hwndChild, LPARAM lParam);
};

#endif // !defined(AFX_RESIZECHILDCTRL_H__7DCD7886_0472_4EC7_A859_17C5F19ACA59__INCLUDED_)