
// captureMFCDlg.h : 头文件
//

#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "direct.h"
#include "io.h"
#include "Windows.h"
#include "sys/timeb.h"
#include "CvvImage.h"
#include "easysize.h"
#include   <vfw.h>   
using namespace cv;
using namespace std;

#pragma once


// CcaptureMFCDlg 对话框
class CcaptureMFCDlg : public CDialogEx
{
	DECLARE_EASYSIZE
// 构造
public:
	CcaptureMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CAPTUREMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	virtual void OnOk();
	virtual void OnCancel();
	BOOL PreTranslateMessage(MSG*   pMsg);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int SaveImg(Mat mat, int flag);
	afx_msg void AutoRecoard(int savetime);
	afx_msg void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAutosave();
	afx_msg void OnBnClickedManualsave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeAutotime();
	afx_msg void OnStnClickedShowxximgtext();
	afx_msg void OnStnClickedShowxximage();
};
