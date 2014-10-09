
// captureMFCDlg.cpp : ʵ���ļ�
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
int timenum=100;//��ʱ��ͼ��ʱ��

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
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);  
	pDC = pwnd->GetDC();
	//pDC =GetDC();  
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	//GetDlgItem(IDC_ShowImage)->GetWindowRect(&rect);

	//��������ͷ
	if (!capture){
		capture = cvCaptureFromCAM(0);
	}
	if (!capture)
	{
		AfxMessageBox(_T("�޷�������ͷ"));
	}
	IplImage* frame;
	frame = cvQueryFrame(capture);
	CvvImage cvvimage;
	cvvimage.CopyOf(frame, 1);
	if (true){
		cvvimage.DrawToHDC(hDC, &rect);
	}
	SetTimer(1, 10, NULL);

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CEdit * timebox;
	timebox = (CEdit*)GetDlgItem(IDC_EDIT1);
	timebox->GetDlgItemInt(timenum);
	if (timenum < 0 || timenum == 0)
	{
		AfxMessageBox(_T("���������0����"));
	}
	else
	{
		SetTimer(1, timenum, AutoSave);
	}
}

void CcaptureMFCDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	IplImage* s_Frame;
	s_Frame = cvQueryFrame(capture);
	SaveImg(s_Frame, 1);
}

void CcaptureMFCDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//timenum=100;
	//GetDlgItem(IDC_EDIT1)->GetDlgItemInt(timenum);
}
int CcaptureMFCDlg::SaveImg(Mat mat, int flag)
{
	//flag=1 ���½��ļ��У�flag=0�½��ļ���
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
		strcat(path, "\\�ֹ���ͼ");
		_mkdir(path);
	}
	_mkdir(path);
	strftime(tmp, sizeof(tmp), "%H_%M_%S_", localtime(&t));

	//ȡ����
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
		AfxMessageBox(_T("�޷�������ͷ"));
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

