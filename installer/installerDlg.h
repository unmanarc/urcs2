// installerDlg.h : header file
//
#include "install.h"
#pragma once


// CinstallerDlg dialog
class CinstallerDlg : public CDialog
{
// Construction
public:
	CinstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_INSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_startatwin;
	BOOL m_log;
	CString banner;
	CString servername;
	UINT m_lport;
	CString m_mhost;
	UINT m_mport;
	CString user;
	CString pass;
	afx_msg void OnBnClickedButton1();
};
