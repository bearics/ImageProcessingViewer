// AddNoise.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "AddNoise.h"
#include "afxdialogex.h"


// AddNoise dialog

IMPLEMENT_DYNAMIC(AddNoise, CDialogEx)

AddNoise::AddNoise(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddNoise, pParent)
{

}

AddNoise::~AddNoise()
{
}

BOOL AddNoise::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strTab = _T("");
	strTab.Format(_T("Gaussian"));
	this->m_mainTab.InsertItem(0, strTab, 0);
	strTab.Format(_T("Salt&Pepper"));
	this->m_mainTab.InsertItem(1, strTab, 0);

	CRect rect;
	this->m_mainTab.GetClientRect(&rect);
	this->tab1.Create(IDD_AddNoise_Tab1, &this->m_mainTab);
	this->tab1.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);

	this->tab2.Create(IDD_AddNoise_Tab2, &this->m_mainTab);
	this->tab2.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOZORDER);

	this->m_pwndShow = &this->tab1;

	return TRUE;
}

void AddNoise::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDNOISETAB, m_mainTab);

}


BEGIN_MESSAGE_MAP(AddNoise, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ADDNOISETAB, &AddNoise::OnTcnSelchangeAddnoisetab)
	ON_BN_CLICKED(IDOK, &AddNoise::OnBnClickedOk)
END_MESSAGE_MAP()


// AddNoise message handlers


void AddNoise::OnTcnSelchangeAddnoisetab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString str1, str2;

	if (this->m_pwndShow != NULL)
	{
		this->m_pwndShow->ShowWindow(SW_HIDE);
		this->m_pwndShow = NULL;
	}

	int tabIndex = this->m_mainTab.GetCurSel();

	switch (tabIndex)
	{
	case 0:
		this->tab1.ShowWindow(SW_SHOW);
		this->m_pwndShow = &this->tab1;
		break;
	case 1:
		this->tab2.ShowWindow(SW_SHOW);
		this->m_pwndShow = &this->tab2;
		break;
	}

	*pResult = 0;
}


void AddNoise::OnBnClickedOk()
{
	CString str1, str2;
	AddNoise_Tab1* tab1 = NULL;
	AddNoise_Tab2* tab2 = NULL;
	int tabIndex = this->m_mainTab.GetCurSel();

	switch (tabIndex)
	{
	case 0:
		tab1 = (AddNoise_Tab1*)this->m_pwndShow;
		tab1->Edit_GN_Mean.GetWindowTextA(str1);
		tab1->Edit_GN_Stdev.GetWindowTextA(str2);
		NoiseType = 0;
		break;
	case 1:
		tab2 = (AddNoise_Tab2*)this->m_pwndShow;
		tab2->Edit_SN_Prob.GetWindowTextA(str1);
		tab2->Edit_PN_Prob.GetWindowTextA(str2);
		NoiseType = 1;
		break;
	}

	value_1 = atof(str1);
	value_2 = atof(str2);

	CDialogEx::OnOK();
}
