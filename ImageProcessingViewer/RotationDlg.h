#pragma once


// RotationDlg dialog

class RotationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RotationDlg)

public:
	RotationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RotationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RotationDlg };
#endif
	
public:
	CStatic m_EditRotateRate;

	double dRotateRate;
	int nInterpolationType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_interp;
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioClicked(UINT msg);
};
