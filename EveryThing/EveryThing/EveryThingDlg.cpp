
// EveryThingDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "EveryThing.h"
#include "EveryThingDlg.h"
#include "afxdialogex.h"

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


CEveryThingDlg::CEveryThingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EVERYTHING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEveryThingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BROWSER, folder_);
	DDX_Control(pDX, IDC_EDIT_FILE, file_);
	DDX_Control(pDX, IDC_LIST_INFO, list_info_);
}

BEGIN_MESSAGE_MAP(CEveryThingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CEveryThingDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CEveryThingDlg message handlers

BOOL CEveryThingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	list_info_.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	list_info_.InsertColumn(0, _T("Name"), 0, 100);
	list_info_.InsertColumn(1, _T("Size"), 0, 100);
	list_info_.InsertColumn(2, _T("Owner"), 0, 100);
	list_info_.InsertColumn(3, _T("Path"), 0, 100);
	list_info_.InsertColumn(4, _T("Time to Modify"), 0, 100);
	list_info_.InsertColumn(5, _T("Time to Created"), 0, 100);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEveryThingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEveryThingDlg::OnPaint()
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
HCURSOR CEveryThingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEveryThingDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	CString folder;
	folder_.GetWindowText(folder);

	CString file_name;
	file_.GetWindowText(file_name);

	CString path = folder + _T("\\") + file_name;

	if (PathFileExists(path)) {
		list_info_.InsertItem(0, file_name);  // Get file name
		list_info_.SetItemText(0, 3, path);   // Get path

		//Get file size
		WIN32_FIND_DATA file_data;
		HANDLE hFile = FindFirstFile(path, &file_data);
		DWORD file_size = file_data.nFileSizeLow;
		CString size;
		size.Format(_T("%d"), file_size);
		list_info_.SetItemText(0, 1, size);  

		//Get file owner
		SECURITY_DESCRIPTOR sd;
		SECURITY_INFORMATION si = OWNER_SECURITY_INFORMATION;
		DWORD size_needed = 0;
		GetFileSecurity(path, si, &sd, 0, &size_needed);
		SECURITY_DESCRIPTOR* pSD = (SECURITY_DESCRIPTOR*)malloc(size_needed);
		GetFileSecurity(path, si, pSD, size_needed, &size_needed);
		PSID pSID = NULL;
		BOOL bOwnerDefaulted;

		GetSecurityDescriptorOwner(pSD, &pSID, &bOwnerDefaulted);
		LPTSTR pName = NULL;
		LPTSTR pDomain = NULL;
		DWORD dwName = 0;
		DWORD dwDomain = 0;
		SID_NAME_USE sid;
		LookupAccountSid(NULL, pSID, pName, &dwName, pDomain, &dwDomain, &sid);

		pName = (LPTSTR)malloc(dwName * sizeof(TCHAR));
		pDomain = (LPTSTR)malloc(dwDomain * sizeof(TCHAR));
		LookupAccountSid(NULL, pSID, pName, &dwName, pDomain, &dwDomain, &sid);
		list_info_.SetItemText(0, 2, pName);

		//Get file time to modify
		SYSTEMTIME st;
		FileTimeToSystemTime(&file_data.ftLastWriteTime, &st);
		CString time;
		time.Format(_T("%d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		list_info_.SetItemText(0, 4, time);

		//Get file time to created
		FileTimeToSystemTime(&file_data.ftCreationTime, &st);
		time.Format(_T("%d-%d-%d %d:%d:%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		list_info_.SetItemText(0, 5, time);

		
	}
	else {
		MessageBox(_T("File not found!"), NULL, MB_ICONWARNING);
	}
	UpdateData(FALSE);
}
