// AddNoise_Tab2.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "AddNoise_Tab2.h"
#include "afxdialogex.h"


// AddNoise_Tab2 dialog

IMPLEMENT_DYNAMIC(AddNoise_Tab2, CDialogEx)

AddNoise_Tab2::AddNoise_Tab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddNoise_Tab2, pParent)
{

}

AddNoise_Tab2::~AddNoise_Tab2()
{
}

void AddNoise_Tab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SPROB, Edit_SN_Prob);
	DDX_Control(pDX, IDC_EDIT_PPROB, Edit_PN_Prob);
}


BEGIN_MESSAGE_MAP(AddNoise_Tab2, CDialogEx)
END_MESSAGE_MAP()


// AddNoise_Tab2 message handlers
