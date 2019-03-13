
// XhcDlg.h : header file
//

#pragma once

#include "CXhcMpg.h"

// CXhcMpgDlg dialog
class CXhcMpgDlg : public CDHtmlDialog
{
// Construction
public:
	CXhcMpgDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_XHC4MACH_DIALOG, IDH = IDR_HTML_XHC4MACH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Implementation
protected:
	HICON m_hIcon;
	HDEVNOTIFY m_hUsbDevNotify;
	CXhcMpg m_mpg;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	virtual BOOL DestroyWindow();
};
