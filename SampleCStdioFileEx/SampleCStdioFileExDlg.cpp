
// SampleCStdioFileExDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleCStdioFileEx.h"
#include "SampleCStdioFileExDlg.h"
#include "afxdialogex.h"
#include "StdioFileEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSampleCStdioFileExDlg dialog



CSampleCStdioFileExDlg::CSampleCStdioFileExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SAMPLECSTDIOFILEEX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleCStdioFileExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleCStdioFileExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &CSampleCStdioFileExDlg::OnBnClickedOpen)
END_MESSAGE_MAP()


// CSampleCStdioFileExDlg message handlers

BOOL CSampleCStdioFileExDlg::OnInitDialog()
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

void CSampleCStdioFileExDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSampleCStdioFileExDlg::OnPaint()
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
HCURSOR CSampleCStdioFileExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSampleCStdioFileExDlg::OnBnClickedOpen()
{
	// TODO: Add your control notification handler code here
	CString strFilePath = _T("");
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files(*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFilePath = dlgFile.GetPathName();
	}
	SetDlgItemText(IDC_PATH, strFilePath);
	OpenFile(strFilePath);
}

void CSampleCStdioFileExDlg::OpenFile(CString strFilePath)
{
	if (!PathFileExists(strFilePath))
	{
		AfxMessageBox(_T("文件不存在！"));
		return;
	}

	CStdioFileEx exFile;
	if (exFile.Open(strFilePath, CFile::modeRead))
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_OUTPUT);
		CString strread, strtemp;
		pEdit->SetSel(0, -1);
		pEdit->Clear();
		while (exFile.ReadString(strtemp))
		{
			strread += strtemp;
		}
		pEdit->ReplaceSel(strread);
		UpdateData(true);
	}
	else
	{
		AfxMessageBox(_T("文件读取失败！"));
		return;
	}
}