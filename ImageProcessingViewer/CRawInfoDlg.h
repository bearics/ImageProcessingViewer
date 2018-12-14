#pragma once

#include<opencv2/opencv.hpp>

// CRawInfoDlg dialog

class CRawInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRawInfoDlg)

public:
	CRawInfoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CRawInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRawInfoDlg };
#endif


public:
	int nHeight;
	int nWidth;
	int nFormat;
		
	CEdit m_EditHeight;
	CEdit m_EditWidth;
	
	bool isTrue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_radio_rawFileInfo;
	afx_msg void OnClickedRadio(UINT msg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
