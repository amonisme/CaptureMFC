
// captureMFCDlg.h : ͷ�ļ�
//

#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "direct.h"
#include "io.h"
#include "Windows.h"
#include "sys/timeb.h"
#include "CvvImage.h"
#include "easysize.h"
using namespace cv;
using namespace std;

#pragma once


// CcaptureMFCDlg �Ի���
class CcaptureMFCDlg : public CDialogEx
{
	DECLARE_EASYSIZE
// ����
public:
	CcaptureMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CAPTUREMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	virtual void OnOk();
	virtual void OnCancel();
	BOOL PreTranslateMessage(MSG*   pMsg);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int SaveImg(Mat mat, int flag);
	afx_msg int AutoRecoard(VideoCapture& cap);
	afx_msg void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeEdit1();
};