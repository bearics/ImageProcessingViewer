#pragma once

#include "AddNoise_Tab1.h"
#include "AddNoise_Tab2.h"

// AddNoise dialog

class AddNoise : public CDialogEx
{
	DECLARE_DYNAMIC(AddNoise)

public:
	AddNoise(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddNoise();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddNoise };
#endif
public:
	AddNoise_Tab1 tab1;
	AddNoise_Tab2 tab2;

	CWnd* m_pwndShow;

	float value_1;	// GN = Mean, SPN = SN_Prob
	float value_2;	// Gn = StDevm Spn = Pn_Prob
	int NoiseType;
	BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_mainTab;
	afx_msg void OnTcnSelchangeAddnoisetab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
};
