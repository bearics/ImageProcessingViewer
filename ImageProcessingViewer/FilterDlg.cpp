// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "FilterDlg.h"
#include "afxdialogex.h"


// FilterDlg dialog

IMPLEMENT_DYNAMIC(FilterDlg, CDialogEx)

FilterDlg::FilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FilterDlg, pParent)
{

}

FilterDlg::~FilterDlg()
{
}

void FilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_AVGFILTER, m_radio_FilterType);
	DDX_Control(pDX, IDC_EDIT_FILTERSIZE, m_EditFilterSize);
}


BEGIN_MESSAGE_MAP(FilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &FilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FilterDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_AVGFILTER, IDC_RADIO_GAUFILTER, OnClickedRadio)
END_MESSAGE_MAP()


// FilterDlg message handlers


void FilterDlg::OnBnClickedOk()
{
	CString cstrTemp;
	m_EditFilterSize.GetWindowTextA(cstrTemp);
	nFilterSize = atoi(cstrTemp);

	CDialog::OnOK();
}


void FilterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

void FilterDlg::OnClickedRadio(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_FilterType)
	{
	case 0:
		nFilterType = 0;
		break;
	case 1:
		nFilterType = 1;
		break;
	case 2:
		nFilterType = 2;
		break;
	case 3:
		nFilterType = 3;
		break;
	}
}