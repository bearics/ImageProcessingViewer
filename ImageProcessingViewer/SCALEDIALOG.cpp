// SCALEDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "SCALEDIALOG.h"
#include "afxdialogex.h"


// SCALEDIALOG dialog

IMPLEMENT_DYNAMIC(SCALEDIALOG, CDialogEx)

SCALEDIALOG::SCALEDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCALEDIALOG, pParent)
	, m_radio_interp(0)
{

}

SCALEDIALOG::~SCALEDIALOG()
{
}

void SCALEDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BILINEAR_S, m_radio_interp);
	DDX_Control(pDX, IDC_EDIT_SCALERATE, m_EditScaleRate);
}


BEGIN_MESSAGE_MAP(SCALEDIALOG, CDialogEx)
	ON_BN_CLICKED(IDOK, &SCALEDIALOG::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_BILINEAR_S, IDC_RADIO_BICUBIC_S, OnRadioClicked)
END_MESSAGE_MAP()


// SCALEDIALOG message handlers


void SCALEDIALOG::OnBnClickedOk()
{
	CString cstrTemp;
	m_EditScaleRate.GetWindowTextA(cstrTemp);
	dScaleRate = atof(cstrTemp);
	CDialog::OnOK();
}

void SCALEDIALOG::OnRadioClicked(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_interp)
	{
	case 0:
		nInterpolationType = 0;
		break;
	case 1:
		nInterpolationType = 1;
		break;
	}
}