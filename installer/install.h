#pragma once
#include "afxwin.h"


// Cinstall dialog

class Cinstall : public CDialog
{
	DECLARE_DYNAMIC(Cinstall)

public:
	Cinstall(CWnd* pParent = NULL);   // standard constructor
	virtual ~Cinstall();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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
	CListBox m_list;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButton1();
};
