
// EveryThingDlg.h : header file
//

#pragma once


// CEveryThingDlg dialog
class CEveryThingDlg : public CDialogEx
{
// Construction
public:
	CEveryThingDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVERYTHING_DIALOG };
#endif

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
protected:
	CMFCEditBrowseCtrl folder_;
	CEdit file_;
	CListCtrl list_info_;
public:
	afx_msg void OnBnClickedButton1();
};
