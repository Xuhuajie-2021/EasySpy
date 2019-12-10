
// EasySpyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasySpy.h"
#include "EasySpyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEasySpyDlg* g_this = NULL;
HHOOK g_lastHook = NULL;
HINSTANCE g_module = NULL;
//hook function
LRESULT WINAPI MessageProc(int nCode, WPARAM w, LPARAM l)
{
	int i = 0;
	i++;

	return CallNextHookEx(g_lastHook, nCode, w, l);
}



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


// CEasySpyDlg �Ի���



CEasySpyDlg::CEasySpyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEasySpyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_rect = { 245, 300, 280, 325 };
	g_this = this;
}

void CEasySpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_listout);
}

void CEasySpyDlg::DrawClickArea()
{
	CDC* hdc = GetDC();
	hdc->Rectangle(&m_rect);
	hdc->TextOutW(250, 302, _T("spy"));
	ReleaseDC(hdc);
}

BEGIN_MESSAGE_MAP(CEasySpyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CEasySpyDlg ��Ϣ�������

BOOL CEasySpyDlg::OnInitDialog()
{
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEasySpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEasySpyDlg::OnPaint()
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
		//��һ���������
		DrawClickArea();
	}
}

void CEasySpyDlg::OnLButtonDown(UINT nFlags,CPoint point)
{
	//���������ť��
	if (PtInRect(&m_rect,point))
	{
		//�����״��
		SetCursor(LoadCursor(NULL,IDC_CROSS));
		//��������ƶ�
		SetCapture();
		m_catching = true;
	}
}

void CEasySpyDlg::OnLButtonUp(UINT nFlags,CPoint point)
{
	if (m_catching)
	{
		//����ת��
		ClientToScreen(&point);  //�ͻ���λ��ת��Ϊ��Ļλ��
		//�鿴����Ĵ��ھ��
		HWND hCaptured = ::WindowFromPoint(point);
		DWORD dwThreadId;
		HOOKPROC hkprcSysMsg=NULL;
		dwThreadId = GetWindowThreadProcessId(hCaptured, NULL);
		if (g_lastHook)
		{
			UnhookWindowsHookEx(g_lastHook);
		}
		//����ȥloaddll
		if (!g_module)
		{
			g_module = LoadLibrary(TEXT("E:\\����\\��Դ����\\hookdemo\\trunk\\EasySpy\\Debug\\DllInject.dll"));
		}
		if (g_module)
		{
			hkprcSysMsg = (HOOKPROC)GetProcAddress(g_module, "GetMsgProc");
		}
		if (hkprcSysMsg)
		{
			//����ȥ������Ϣ
			g_lastHook = SetWindowsHookEx(WH_GETMESSAGE, hkprcSysMsg, g_module, dwThreadId);
		}
			
		//�ָ������ʽ
		SetCursor(LoadCursor(NULL, IDC_ARROW)); //����Ǳ���֮ǰ�������״
		ReleaseCapture();
	}

	m_catching = false;
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEasySpyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CEasySpyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message>WM_USER)
	{
		CString strFormat;
		strFormat.Format(L"0x%04X", message - (WM_USER + 100));
		m_listout.AddString(strFormat);
	}
	return __super::WindowProc(message, wParam, lParam);
}

