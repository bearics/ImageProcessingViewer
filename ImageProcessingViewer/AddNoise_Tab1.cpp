// AddNoise_Tab1.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "AddNoise_Tab1.h"
#include "afxdialogex.h"


// AddNoise_Tab1 dialog

IMPLEMENT_DYNAMIC(AddNoise_Tab1, CDialogEx)

AddNoise_Tab1::AddNoise_Tab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddNoise_Tab1, pParent)
{

}

AddNoise_Tab1::~AddNoise_Tab1()
{
}

void AddNoise_Tab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GNMEAN, Edit_GN_Mean);
	DDX_Control(pDX, IDC_EDIT_GNSTDEV, Edit_GN_Stdev);
}


BEGIN_MESSAGE_MAP(AddNoise_Tab1, CDialogEx)
END_MESSAGE_MAP()


// AddNoise_Tab1 message handlers
