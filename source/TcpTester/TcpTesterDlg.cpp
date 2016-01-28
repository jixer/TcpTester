
// TcpTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TcpTester.h"
#include "TcpTesterDlg.h"
#include "afxdialogex.h"
#include <winsock.h>
#include <ws2tcpip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CTcpTesterDlg dialog



CTcpTesterDlg::CTcpTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTcpTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTcpTesterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTcpTesterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTcpTesterDlg message handlers

BOOL CTcpTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTcpTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTcpTesterDlg::OnPaint()
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
HCURSOR CTcpTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTcpTesterDlg::OnBnClickedButton1()
{
	
	CString edit1;
	GetDlgItemText(IDC_EDITSERVER, edit1);

	CString edit2;
	GetDlgItemText(IDC_EDITPORT, edit2);

	CString edit3;
	GetDlgItemText(IDC_EDITTIMEOUT, edit3);	

	if (edit1 == "" || edit2 == "") {
		AfxMessageBox(_T("Please enter all required values!"), MB_OK | MB_ICONSTOP);
		return;
	}
	
	ClearResultsCtrl();
	SetStatus(CString("Testing connection"));

	int iResult = ConnectToAddr(edit1, edit2, edit3);
	CString msg = iResult ? CString("Connection failed!") : CString("Connection successful");
	SetStatus(msg);
	AppendToResultsCtrl(msg);
}

int CTcpTesterDlg::ConnectToAddr(CString addr, CString port, CString timeout){
	WSADATA wsaData;
	int iResult;

	AppendToResultsCtrl(CString("Setting up socket..\r\n"));
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	AppendToResultsCtrl(CString("Resolving address..\r\n"));
	
	iResult = getaddrinfo(CStringA(addr), CStringA(port), &hints, &result);
	if (iResult) {
		AppendToResultsCtrl(CString("Could not resolve address\r\n"));
		return iResult;
	}
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)result->ai_addr;
	char ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ipv4->sin_addr), ipAddress, INET_ADDRSTRLEN);

	AppendToResultsCtrl(CString("Resolved address to ") + CString(ipAddress) + CString(" \r\n"));
	ptr = result;

	SOCKET ConnectSocket = INVALID_SOCKET;
	AppendToResultsCtrl(CString("Connecting..\r\n"));
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	
	return iResult;
}



void CTcpTesterDlg::AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText)
{
	int nLength = edit.GetWindowTextLength();
	edit.SetSel(nLength, nLength);
	edit.ReplaceSel(pszText);
}

void CTcpTesterDlg::ClearResultsCtrl(){
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EDITOUTPUT);
	edit->SetWindowText(CString(" "));
	edit->SetWindowText(CString(""));
}
void CTcpTesterDlg::AppendToResultsCtrl(LPCTSTR pszText){
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EDITOUTPUT);
	AppendTextToEditCtrl(*edit, pszText);
}

void CTcpTesterDlg::SetStatus(LPCTSTR msg){
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EDITSTATUS);
	edit->SetWindowText(msg);
}