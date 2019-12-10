
// EasySpyDlg.cpp : 实现文件
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


// CEasySpyDlg 对话框



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


// CEasySpyDlg 消息处理程序

BOOL CEasySpyDlg::OnInitDialog()
{
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

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEasySpyDlg::OnPaint()
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
		//画一个点击区域
		DrawClickArea();
	}
}

void CEasySpyDlg::OnLButtonDown(UINT nFlags,CPoint point)
{
	//鼠标点击到按钮上
	if (PtInRect(&m_rect,point))
	{
		//鼠标形状变
		SetCursor(LoadCursor(NULL,IDC_CROSS));
		//捕获鼠标移动
		SetCapture();
		m_catching = true;
	}
}

void CEasySpyDlg::OnLButtonUp(UINT nFlags,CPoint point)
{
	if (m_catching)
	{
		//坐标转换
		ClientToScreen(&point);  //客户端位置转化为屏幕位置
		//查看捕获的窗口句柄
		HWND hCaptured = ::WindowFromPoint(point);
		DWORD dwThreadId;
		HOOKPROC hkprcSysMsg=NULL;
		dwThreadId = GetWindowThreadProcessId(hCaptured, NULL);
		if (g_lastHook)
		{
			UnhookWindowsHookEx(g_lastHook);
		}
		//尝试去loaddll
		if (!g_module)
		{
			g_module = LoadLibrary(TEXT("E:\\代码\\开源代码\\hookdemo\\trunk\\EasySpy\\Debug\\DllInject.dll"));
		}
		if (g_module)
		{
			hkprcSysMsg = (HOOKPROC)GetProcAddress(g_module, "GetMsgProc");
		}
		if (hkprcSysMsg)
		{
			//这里去拦截消息
			g_lastHook = SetWindowsHookEx(WH_GETMESSAGE, hkprcSysMsg, g_module, dwThreadId);
		}
			
		//恢复鼠标样式
		SetCursor(LoadCursor(NULL, IDC_ARROW)); //最好是保存之前的鼠标形状
		ReleaseCapture();
	}

	m_catching = false;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

