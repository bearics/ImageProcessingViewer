#pragma once


// FilterDlg dialog

class FilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FilterDlg)

public:
	FilterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~FilterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FilterDlg };
#endif

public:
	CStatic m_EditFilterSize;

	int nFilterSize;
	int nFilterType;

	int m_radio_FilterType;
	afx_msg void OnClickedRadio(UINT msg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

};
