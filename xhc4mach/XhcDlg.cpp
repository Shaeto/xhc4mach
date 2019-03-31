
// XhcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xhc4mach.h"
#include "XhcDlg.h"
#include "afxdialogex.h"
#include <devguid.h>
#include <dbt.h>
#include <string>
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CXhcMpgDlg dialog

CXhcMpgDlg::CXhcMpgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_XHC4MACH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINFRAME);
	m_hUsbDevNotify = 0;
}

void CXhcMpgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XHC_LIST, m_xhcListBox);
	DDX_Control(pDX, IDC_MACH4_CONNECT, m_btnMuch4Connect);
	DDX_Control(pDX, IDC_MACH4_DISCONNECT, m_btnMuch4Disconnect);
	DDX_Control(pDX, IDC_MC_POS_AXIS_A, m_editMCPosAxisA);
	DDX_Control(pDX, IDC_MC_POS_AXIS_Y, m_editMCPosAxisY);
	DDX_Control(pDX, IDC_MC_POS_AXIS_X, m_editMCPosAxisX);
	DDX_Control(pDX, IDC_MC_POS_AXIS_Z, m_editMCPosAxisZ);
	DDX_Control(pDX, IDC_WC_POS_AXIS_A, m_editWCPosAxisA);
	DDX_Control(pDX, IDC_WC_POS_AXIS_Y, m_editWCPosAxisY);
	DDX_Control(pDX, IDC_WC_POS_AXIS_X, m_editWCPosAxisX);
	DDX_Control(pDX, IDC_WC_POS_AXIS_Z, m_editWCPosAxisZ);
}

BEGIN_MESSAGE_MAP(CXhcMpgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_CLOSE, &CXhcMpgDlg::OnBnClickedClose)
	ON_MESSAGE(WM_MPG_LIST_CHANGED, OnMpgListChanged)
	ON_MESSAGE(WM_MPG_STATE_CHANGED, OnMpgStateChanged)
	ON_MESSAGE(WM_MPG_MACH4_STATUS, OnMpgMach4Status)
	ON_BN_CLICKED(IDC_MACH4_CONNECT, &CXhcMpgDlg::OnClickedMach4Connect)
	ON_BN_CLICKED(IDC_MACH4_DISCONNECT, &CXhcMpgDlg::OnClickedMach4Disconnect)
END_MESSAGE_MAP()


// CXhcMpgDlg message handlers

BOOL CXhcMpgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_btnMuch4Connect.EnableWindow(TRUE);
	m_btnMuch4Disconnect.EnableWindow(FALSE);

	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = GUID_DEVCLASS_HIDCLASS;

	m_hUsbDevNotify = RegisterDeviceNotification(GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	m_mpg.open(GetSafeHwnd());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CXhcMpgDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	BOOL bReturn = CWnd::OnDeviceChange(nEventType, dwData);

	if (nEventType == DBT_DEVICEARRIVAL || nEventType == DBT_DEVNODES_CHANGED) {
		m_mpg.rescan();
	}

	return bReturn;
}

LRESULT CXhcMpgDlg::OnMpgListChanged(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	std::list<std::wstring> list = m_mpg.devices();

	m_xhcListBox.ResetContent();
	for (const auto& s : list) {
		m_xhcListBox.AddString(s.c_str());
	}

	return 0;
}

afx_msg LRESULT CXhcMpgDlg::OnMpgStateChanged(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	wchar_t buf[128];

	CM4otionState s = m_mpg.state();

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.mc(AXIS_X));
	m_editMCPosAxisX.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.mc(AXIS_Y));
	m_editMCPosAxisY.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.mc(AXIS_Z));
	m_editMCPosAxisZ.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.mc(AXIS_A));
	m_editMCPosAxisA.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.wc(AXIS_X));
	m_editWCPosAxisX.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.wc(AXIS_Y));
	m_editWCPosAxisY.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.wc(AXIS_Z));
	m_editWCPosAxisZ.SetWindowText(buf);

	_snwprintf_s(buf, sizeof(buf), _T("%.5f"), s.wc(AXIS_A));
	m_editWCPosAxisA.SetWindowText(buf);

	return 0;
}

LRESULT CXhcMpgDlg::OnMpgMach4Status(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (wParam) {
		m_btnMuch4Connect.EnableWindow(FALSE);
		m_btnMuch4Disconnect.EnableWindow(TRUE);
	}
	else {
		m_btnMuch4Connect.EnableWindow(TRUE);
		m_btnMuch4Disconnect.EnableWindow(FALSE);
	}

	return 0;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXhcMpgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXhcMpgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CXhcMpgDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_hUsbDevNotify && !UnregisterDeviceNotification(m_hUsbDevNotify)) {

	}
	m_mpg.close();

	return CDialogEx::DestroyWindow();
}


void CXhcMpgDlg::OnBnClickedClose()
{
	EndDialog(0);
}


void CXhcMpgDlg::OnClickedMach4Connect()
{
	m_mpg.close();
	m_mpg.open(GetSafeHwnd());
}


void CXhcMpgDlg::OnClickedMach4Disconnect()
{
	m_mpg.close();
}
