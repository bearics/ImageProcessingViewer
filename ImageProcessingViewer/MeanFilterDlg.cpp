// MeanFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "MeanFilterDlg.h"
#include "afxdialogex.h"


// MeanFilterDlg dialog

IMPLEMENT_DYNAMIC(MeanFilterDlg, CDialogEx)

MeanFilterDlg::MeanFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MeanFilterDlg, pParent)
{

}

MeanFilterDlg::~MeanFilterDlg()
{
}

void MeanFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILTERSIZE, Edit_FilterSize);
	DDX_Control(pDX, IDC_EDIT_HARMO, Edit_Q);
	DDX_Radio(pDX, IDC_RADIO_ARITH, m_radio_mf);
}


BEGIN_MESSAGE_MAP(MeanFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MeanFilterDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ARITH, IDC_RADIO_CONHARMO, &MeanFilterDlg::OnRadioClicked)
END_MESSAGE_MAP()


// MeanFilterDlg message handlers


void MeanFilterDlg::OnBnClickedOk()
{
	CString strTemp1, strTemp2;

	Edit_FilterSize.GetWindowTextA(strTemp1);
	Edit_Q.GetWindowTextA(strTemp2);

	nFilterSize = atoi(strTemp1);
	Q = atoi(strTemp2);

	CDialog::OnOK();
}

void MeanFilterDlg::OnRadioClicked(UINT msg)
{
	UpdateData(TRUE);
	nMeanFilterType = 0;
	switch (m_radio_mf)
	{
	case 0:
		nMeanFilterType = 0;
		break;
	case 1:
		nMeanFilterType = 1;
		break;
	case 2:
		nMeanFilterType = 2;
		break;
	case 3:
		nMeanFilterType = 3;
		break;
	}
}
