
// TcpTesterDlg.h : header file
//

#pragma once


// CTcpTesterDlg dialog
class CTcpTesterDlg : public CDialogEx
{
// Construction
public:
	CTcpTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TCPTESTER_DIALOG };

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
	int ConnectToAddr(CString addr, CString port, int timeout);
	void AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText);
	void ClearResultsCtrl();
	void AppendToResultsCtrl(LPCTSTR pszText);
	void SetStatus(LPCTSTR msg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
