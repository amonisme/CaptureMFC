
// captureMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "captureMFC.h"
#include "captureMFCDlg.h"
#include "afxdialogex.h"
#include "easysize.h"
#include  "vfw.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvCapture* capture;
IplImage* g_Frame;
CRect rect[2],w_rect;
CDC *pDC;
HDC hDC[2];
CWnd *pwnd;
int timenum = 100;//��ʱ��ͼ��ʱ��
int state = 0; //��ʼ��ͼ��ť��״̬
int window_width = 0;
int window_height = 0;

BEGIN_EASYSIZE_MAP(CcaptureMFCDlg)
	EASYSIZE(IDCANCEL, ES_KEEPSIZE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_ManualSave, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDCANCEL, 0)
	EASYSIZE(IDC_AutoSave, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDC_ManualSave, 0)
	EASYSIZE(IDC_AutoTime, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDC_AutoSave, 0)
	EASYSIZE(IDC_AutoTimeText, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDC_AutoTime, 0)
	//EASYSIZE(IDC_ShowXXImage, ES_BORDER, ES_BORDER, IDC_AutoTime, ES_BORDER, 0)
	//EASYSIZE(IDC_ShowXXImgText, ES_BORDER, ES_BORDER, IDC_ShowXXImgText,ES_BORDER, 0)
	EASYSIZE(IDC_ShowImage, ES_BORDER, ES_BORDER, IDC_AutoTimeText, ES_BORDER, 0)
	EASYSIZE(IDC_ShowImgText, ES_BORDER, ES_BORDER, IDC_AutoTimeText, ES_BORDER, 0)
	EASYSIZE(IDC_ImgPath, ES_BORDER, IDC_ShowImage, IDC_AutoTimeText, ES_BORDER, 0)
END_EASYSIZE_MAP

void CALLBACK EXPORT AutoSave(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT_PTR nIDEvent,   // timer identification
	DWORD dwTime    // system time
	);

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcaptureMFCDlg �Ի���



CcaptureMFCDlg::CcaptureMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcaptureMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcaptureMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AutoTime, timenum);
	//DDV_MinMaxInt(pDX, timenum, 1, 1000000);
}

BEGIN_MESSAGE_MAP(CcaptureMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CcaptureMFCDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_AutoSave, &CcaptureMFCDlg::OnBnClickedAutosave)
	ON_BN_CLICKED(IDC_ManualSave, &CcaptureMFCDlg::OnBnClickedManualsave)
	ON_EN_CHANGE(IDC_AutoTime, &CcaptureMFCDlg::OnEnChangeAutotime)
END_MESSAGE_MAP()


// CcaptureMFCDlg ��Ϣ�������

BOOL CcaptureMFCDlg::OnInitDialog()
{
	INIT_EASYSIZE;
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MAXIMIZE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ô��ڴ�С
	GetWindowRect(&w_rect);
	window_width = w_rect.Width();
	window_height = w_rect.Height();

	pwnd = GetDlgItem(IDC_ShowImage); 
	pDC = pwnd->GetDC();  
	hDC[0] = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect[0]);
	//hDC[1] = GetDlgItem(IDC_ShowXXImage)->GetDC()->GetSafeHdc();
	//GetDlgItem(IDC_ShowXXImage)->GetClientRect(&rect[1]);
	//��������ͷ
	if (!capture){
		capture = cvCaptureFromCAM(0);
	}
	if (!capture)
	{
		AfxMessageBox(_T("�޷�������ͷ"));
	}
	g_Frame = cvQueryFrame(capture);
	CvvImage cvvimage[2];
	cvvimage[0].CopyOf(g_Frame, 1);
	cvvimage[1].CopyOf(g_Frame, -1);
	if (true){
		cvvimage[0].DrawToHDC(hDC[0], &rect[0]);
		//cvvimage[1].DrawToHDC(hDC[1], &rect[1]);
	}
	SetTimer(TIMER1, 10, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CcaptureMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CcaptureMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CcaptureMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CcaptureMFCDlg::OnOk() 
{
	
}

void CcaptureMFCDlg::OnCancel()
{
	return;
}

BOOL    CcaptureMFCDlg::PreTranslateMessage(MSG*   pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:   //����Enter 
			return true;
		}
	}
	return   CDialog::PreTranslateMessage(pMsg);

}

void CcaptureMFCDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	cvReleaseCapture(&capture);
	CDialogEx::OnCancel();
}

void CcaptureMFCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	UPDATE_EASYSIZE;
}

void CcaptureMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);
	g_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage[2];
	m_CvvImage[0].CopyOf(g_Frame, 1);
	//m_CvvImage[1].CopyOf(g_Frame, -1);
	if (true)
	{
		GetDlgItem(IDC_ShowImage)->GetClientRect(&rect[0]);
		//GetDlgItem(IDC_ShowXXImage)->GetClientRect(&rect[1]);
		m_CvvImage[0].DrawToHDC(hDC[0], &rect[0]);
		//m_CvvImage[1].DrawToHDC(hDC[1], &rect[1]);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CcaptureMFCDlg::OnBnClickedAutosave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CEdit * timebox;
	timebox = (CEdit*)GetDlgItem(IDC_AutoTime);
	timebox->GetDlgItemInt(timenum);
	if (timenum < 0 ||  0 == timenum )
	{
		AfxMessageBox(_T("���������0����"));
	}
	else
	{
		if (0 == state)
		{
			state = 1;
			CWnd * pwnd_autosave = GetDlgItem(IDC_AutoSave);
			pwnd_autosave->SetWindowTextW(_T("ֹͣ��ͼ"));
			GetDlgItem(IDC_AutoTime)->EnableWindow(FALSE); 
			SetTimer(TIMER2, timenum, AutoSave);
		}
		else 
		{
			state = 0;
			CWnd * pwnd_autosave = GetDlgItem(IDC_AutoSave);
			pwnd_autosave->SetWindowTextW(_T("��ʼ��ͼ"));
			GetDlgItem(IDC_AutoTime)->EnableWindow(TRUE);
			SetTimer(TIMER2, 10, NULL);
		}	
	}
}

void CcaptureMFCDlg::OnEnChangeAutotime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CcaptureMFCDlg::OnBnClickedManualsave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SaveImg(g_Frame, 1);
}

int CcaptureMFCDlg::SaveImg(Mat mat, int flag)
{
	//flag=1 ���½��ļ��У�flag=0�½��ļ���
	char path[100] = R"(.\pic\)";
	_mkdir(path);
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
	strcat(path, tmp);
	_mkdir(path);
	if (0 == flag)
	{
		strcat(path, R"(\�Զ���ͼ)");
		if (_access(path, 0) == -1)
			_mkdir(path);
	}
	else if (1 == flag)
	{
		strcat(path, R"(\�ֶ���ͼ)");
		if (_access(path, 0) == -1)
			_mkdir(path);
	}
	strftime(tmp, sizeof(tmp), "%H_%M_%S_", localtime(&t));

	//ȡ����
	struct timeb t1;
	ftime(&t1);
	int intmilltime = t1.millitm;
	char* milltime = new char;
	_itoa(intmilltime, milltime, 10);
	//cout << t1.millitm << endl;
	strcat(path, R"(\)");
	strcat(path, tmp);
	strcat(path, milltime);
	strcat(path, ".jpg");
	int save = cv::imwrite(path, mat);
	CString showpath;
	showpath = strcat(path, "  Saved!");
	GetDlgItem(IDC_ImgPath)->SetWindowTextW(showpath);//��ʾͼ��洢·��
	/*CWnd * pWnd = AfxGetMainWnd();
	CDC *pDC = pWnd->GetDC();
	int x = 500;
	int y = 1000;
	TextOut(pDC->GetSafeHdc(),x,y, showpath,100);
	this->ReleaseDC(pDC);*/
	return save;
}

void CALLBACK EXPORT AutoSave(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT_PTR nIDEvent,   // timer identification
	DWORD dwTime    // system time
	)
{
	CWinApp* pThisApp = AfxGetApp();
	CWnd*pMainWnd = pThisApp->m_pMainWnd;
	CcaptureMFCDlg* pMainDlg=(CcaptureMFCDlg*) pMainWnd;
	if (pMainDlg == NULL)  return;
	pMainDlg->SaveImg(g_Frame, 0);
}

