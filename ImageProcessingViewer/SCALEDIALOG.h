#pragma once


// SCALEDIALOG dialog

class SCALEDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(SCALEDIALOG)

public:
	SCALEDIALOG(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SCALEDIALOG();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCALEDIALOG };
#endif

public:
	CStatic m_EditScaleRate;

	double dScaleRate;
	int nInterpolationType;




protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_interp;
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioClicked(UINT msg);
};
