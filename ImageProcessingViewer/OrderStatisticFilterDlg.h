#pragma once


// OrderStatisticFilterDlg dialog

class OrderStatisticFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OrderStatisticFilterDlg)

public:
	OrderStatisticFilterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OrderStatisticFilterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OrderStatisticFilterDlg };
#endif

public:
	int m_radio_osf;
	int nFilterSize1;
	int nFilterSize2;
	int nOSFType;

	CEdit Edit_FilterSize1;
	CEdit Edit_FilterSize2;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioClicked(UINT msg);
};
