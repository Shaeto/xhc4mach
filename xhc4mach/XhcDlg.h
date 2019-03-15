
// XhcDlg.h : header file
//

#pragma once

#include "CXhcMpg.h"

// CXhcMpgDlg dialog
class CXhcMpgDlg : public CDialogEx
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

// Implementation
protected:
	HICON m_hIcon;
	HDEVNOTIFY m_hUsbDevNotify;
	CXhcMpg m_mpg;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMpgListChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMpgStateChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMpgMach4Status(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedClose();
private:
	CListBox m_xhcListBox;
public:
	CButton m_btnMuch4Connect;
	CButton m_btnMuch4Disconnect;
	CEdit m_editMCPosAxisX;
	CEdit m_editMCPosAxisY;
	CEdit m_editMCPosAxisZ;
	CEdit m_editMCPosAxisA;
	CEdit m_editWCPosAxisX;
	CEdit m_editWCPosAxisY;
	CEdit m_editWCPosAxisZ;
	CEdit m_editWCPosAxisA;
	afx_msg void OnClickedMach4Connect();
	afx_msg void OnClickedMach4Disconnect();
};
