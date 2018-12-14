// OrderStatisticFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "OrderStatisticFilterDlg.h"
#include "afxdialogex.h"


// OrderStatisticFilterDlg dialog

IMPLEMENT_DYNAMIC(OrderStatisticFilterDlg, CDialogEx)

OrderStatisticFilterDlg::OrderStatisticFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OrderStatisticFilterDlg, pParent)
{

}

OrderStatisticFilterDlg::~OrderStatisticFilterDlg()
{
}

void OrderStatisticFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MEDIAN, m_radio_osf);
	DDX_Control(pDX, IDC_EDIT_MINSIZE, Edit_FilterSize1);
	DDX_Control(pDX, IDC_EDIT_MAXSIZE, Edit_FilterSize2);
}


BEGIN_MESSAGE_MAP(OrderStatisticFilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &OrderStatisticFilterDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MEDIAN, IDC_RADIO_ADAP, OnRadioClicked)
END_MESSAGE_MAP()


// OrderStatisticFilterDlg message handlers


void OrderStatisticFilterDlg::OnBnClickedOk()
{
	CString strTemp1, strTemp2;

	Edit_FilterSize1.GetWindowTextA(strTemp1);
	Edit_FilterSize2.GetWindowTextA(strTemp2);

	nFilterSize1 = atoi(strTemp1);
	nFilterSize2 = atoi(strTemp2);

	CDialogEx::OnOK();
}

void OrderStatisticFilterDlg::OnRadioClicked(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_osf)
	{
	case 0:

		nOSFType = 4;
		break;

	case 1:

		nOSFType = 5;
		break;
	}
}
