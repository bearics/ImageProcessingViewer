#pragma once


// AddNoise_Tab1 dialog

class AddNoise_Tab1 : public CDialogEx
{
	DECLARE_DYNAMIC(AddNoise_Tab1)

public:
	AddNoise_Tab1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddNoise_Tab1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddNoise_Tab1 };
#endif

public:
	CEdit Edit_GN_Mean;
	CEdit Edit_GN_Stdev;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
