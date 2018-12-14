#pragma once


// AddNoise_Tab2 dialog

class AddNoise_Tab2 : public CDialogEx
{
	DECLARE_DYNAMIC(AddNoise_Tab2)

public:
	AddNoise_Tab2(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddNoise_Tab2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddNoise_Tab2 };
#endif
public:
	CEdit Edit_SN_Prob;
	CEdit Edit_PN_Prob;
	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
