
// EasySpyDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEasySpyDlg 对话框
class CEasySpyDlg : public CDialogEx
{
// 构造
public:
	CEasySpyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EASYSPY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void DrawClickArea();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnLButtonUp(UINT, CPoint);
	afx_msg HCURSOR OnQueryDragIcon();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	
	RECT m_rect ;
	
	bool m_catching = false;
	CListBox m_listout;
};
