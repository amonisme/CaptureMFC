
// captureMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "captureMFC.h"
#include "captureMFCDlg.h"
#include "afxdialogex.h"
#include "easysize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CvCapture* capture;
IplImage* m_Frame;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
int timenum=100;//定时截图的时间

BEGIN_EASYSIZE_MAP(CcaptureMFCDlg)
	EASYSIZE(IDCANCEL, ES_KEEPSIZE, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON2, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDCANCEL, 0)
	EASYSIZE(IDC_BUTTON1, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDC_BUTTON2, 0)
	EASYSIZE(IDC_EDIT1, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDC_BUTTON1, 0)
	EASYSIZE(IDC_STATIC, ES_KEEPSIZE, ES_BORDER, ES_BORDER, IDC_EDIT1, 0)
	EASYSIZE(IDC_ShowImage, ES_BORDER, ES_BORDER, IDC_EDIT1, ES_BORDER, 0)
END_EASYSIZE_MAP

void CALLBACK EXPORT AutoSave(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT nIDEvent,   // timer identification
	DWORD dwTime    // system time
	);

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CcaptureMFCDlg 对话框



CcaptureMFCDlg::CcaptureMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcaptureMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcaptureMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, timenum);
	DDV_MinMaxInt(pDX, timenum, 1, 1000000);
}

BEGIN_MESSAGE_MAP(CcaptureMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CcaptureMFCDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CcaptureMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcaptureMFCDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CcaptureMFCDlg::OnEnChangeEdit1)
	ON_WM_TIMER()
	ON_WM_SIZE()


	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CcaptureMFCDlg 消息处理程序

BOOL CcaptureMFCDlg::OnInitDialog()
{
	INIT_EASYSIZE;
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO:  在此添加额外的初始化代码
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);  
	pDC = pwnd->GetDC();
	//pDC =GetDC();  
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	//GetDlgItem(IDC_ShowImage)->GetWindowRect(&rect);

	//启动摄像头
	if (!capture){
		capture = cvCaptureFromCAM(0);
	}
	if (!capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
	}
	IplImage* frame;
	frame = cvQueryFrame(capture);
	CvvImage cvvimage;
	cvvimage.CopyOf(frame, 1);
	if (true){
		cvvimage.DrawToHDC(hDC, &rect);
	}
	SetTimer(1, 10, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcaptureMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		case VK_RETURN:   //屏蔽Enter 
			return true;
		}
	}
	return   CDialog::PreTranslateMessage(pMsg);

}

void CcaptureMFCDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	//IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		GetDlgItem(IDC_ShowImage)->GetClientRect(&rect);
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);  
		//SaveImg(m_Frame,0);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CcaptureMFCDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CEdit * timebox;
	timebox = (CEdit*)GetDlgItem(IDC_EDIT1);
	timebox->GetDlgItemInt(timenum);
	if (timenum < 0 || timenum == 0)
	{
		AfxMessageBox(_T("请输入大于0的数"));
	}
	else
	{
		SetTimer(1, timenum, AutoSave);
	}
}

void CcaptureMFCDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	IplImage* s_Frame;
	s_Frame = cvQueryFrame(capture);
	SaveImg(s_Frame, 1);
}

void CcaptureMFCDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//timenum=100;
	//GetDlgItem(IDC_EDIT1)->GetDlgItemInt(timenum);
}
int CcaptureMFCDlg::SaveImg(Mat mat, int flag)
{
	//flag=1 不新建文件夹；flag=0新建文件夹
	char path[100] = "F:\\pic\\";
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&t));
	strcat(path, tmp);
	if (0 == flag && (_access(path, 0) == -1))
	{
		_mkdir(path);
	}
	else if (1 == flag)
	{
		strcat(path, "\\手工截图");
		_mkdir(path);
	}
	_mkdir(path);
	strftime(tmp, sizeof(tmp), "%H_%M_%S_", localtime(&t));

	//取毫秒
	struct timeb t1;
	ftime(&t1);
	int intmilltime = t1.millitm;
	char* milltime = new char;
	_itoa(intmilltime, milltime, 10);
	//cout << t1.millitm << endl;
	strcat(path, "\\");
	strcat(path, tmp);
	strcat(path, milltime);
	strcat(path, ".jpg");
	int save = cv::imwrite(path, mat);
	cout << path << (save ? " Saved!" : "Can't be saved") << endl;
	//imwrite(fileName,img);
	return save;
}

void CALLBACK EXPORT AutoSave(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT nIDEvent,   // timer identification
	DWORD dwTime    // system time
	)
{
	CcaptureMFCDlg c;
	if (!capture){
		capture = cvCaptureFromCAM(0);
	}
	if (!capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
		return;
	}
	IplImage* frame;
	frame = cvQueryFrame(capture);
	CvvImage cvvimage;
	cvvimage.CopyOf(frame, 1);
	if (true){
	//	c.GetDlgItem(IDC_ShowImage)->GetClientRect(&rect);
		cvvimage.DrawToHDC(hDC, &rect);
	}
	c.SaveImg(m_Frame, 0);
}

