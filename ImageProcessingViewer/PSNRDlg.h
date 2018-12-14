#pragma once
#include "CRawInfoDlg.h"
#include "ViewerDlg.h"

// PSNRDlg dialog

class PSNRDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PSNRDlg)

public:
	PSNRDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PSNRDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PSNRDlg };
#endif

public:
	unsigned char** Ori, **Dst;

	int nHeight_Ori, nWidth_Ori;
	int nHeight_Dst, nWidth_Dst;

	CEdit Edit_File_Ori, Edit_File_Dst, Edit_PSNR;
	double GetPSNR(unsigned char** Ori, unsigned char** Dst, int Height, int nWidth);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen1();
	afx_msg void OnBnClickedButtonOpen2();
	afx_msg void OnBnClickedButtonPsnr();
	afx_msg void OnBnClickedCancel();
};
