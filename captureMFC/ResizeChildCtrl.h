// ResizeChildCtrl.h: interface for the CResizeChildCtrl class.
// �ӿؼ�����Ӧ���ڴ�С�ĸı�
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZECHILDCTRL_H__7DCD7886_0472_4EC7_A859_17C5F19ACA59__INCLUDED_)
#define AFX_RESIZECHILDCTRL_H__7DCD7886_0472_4EC7_A859_17C5F19ACA59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���ݴ�С�仯����Ϣ
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
	// �����ӿؼ������Ĵ�С
	static void ResizeClildCtrl(HWND dlgHwnd, CRect primalClientRect, CRect resultClientRect);
	// EnumChildWindows() �ص����������ĵ����ӿؼ���С
	static BOOL CALLBACK ResizeChildPor(HWND hwndChild, LPARAM lParam);
};

#endif // !defined(AFX_RESIZECHILDCTRL_H__7DCD7886_0472_4EC7_A859_17C5F19ACA59__INCLUDED_)