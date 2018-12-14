// RotationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "RotationDlg.h"
#include "afxdialogex.h"


// RotationDlg dialog

IMPLEMENT_DYNAMIC(RotationDlg, CDialogEx)

RotationDlg::RotationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RotationDlg, pParent)
	, m_radio_interp(0)
{

}

RotationDlg::~RotationDlg()
{
}

void RotationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BILINEAR_S, m_radio_interp);
	DDX_Control(pDX, IDC_EDIT_ROTATIONRATE, m_EditRotateRate);
}


BEGIN_MESSAGE_MAP(RotationDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RotationDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_BILINEAR_S, IDC_RADIO_BICUBIC_S, OnRadioClicked)
END_MESSAGE_MAP()


// RotationDlg message handlers


void RotationDlg::OnBnClickedOk()
{
	CString cstrAngle;
	m_EditRotateRate.GetWindowTextA(cstrAngle);
	dRotateRate = atof(cstrAngle);
	CDialog::OnOK();
}

void RotationDlg::OnRadioClicked(UINT msg)
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