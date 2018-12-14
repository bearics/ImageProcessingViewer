#pragma once


// MeanFilterDlg dialog

class MeanFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MeanFilterDlg)

public:
	MeanFilterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MeanFilterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MeanFilterDlg };
#endif

public:
	int nFilterSize;
	int nMeanFilterType;
	int Q;

	CEdit Edit_FilterSize;
	CEdit Edit_Q;

	int m_radio_mf;

	
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioClicked(UINT);
};
