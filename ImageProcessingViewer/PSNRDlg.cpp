// PSNRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "PSNRDlg.h"
#include "afxdialogex.h"


// PSNRDlg dialog

IMPLEMENT_DYNAMIC(PSNRDlg, CDialogEx)

PSNRDlg::PSNRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PSNRDlg, pParent)
{

}

PSNRDlg::~PSNRDlg()
{
}

void PSNRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit_File_Ori);
	DDX_Control(pDX, IDC_EDIT3, Edit_File_Dst);
	DDX_Control(pDX, IDC_EDIT4, Edit_PSNR);
}


BEGIN_MESSAGE_MAP(PSNRDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OPEN1, &PSNRDlg::OnBnClickedButtonOpen1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, &PSNRDlg::OnBnClickedButtonOpen2)
	ON_BN_CLICKED(IDC_BUTTON_PSNR, &PSNRDlg::OnBnClickedButtonPsnr)
	ON_BN_CLICKED(IDCANCEL, &PSNRDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// PSNRDlg message handlers


void PSNRDlg::OnBnClickedButtonOpen1()
{
	CFileDialog dlg(TRUE);
	CRawInfoDlg RawDlg;

	if (Ori != NULL)
	{
		MemFree2D(Ori, nHeight_Ori);
	}

	if (dlg.DoModal() == IDOK)
	{

		FILE *fp;
		fopen_s(&fp, dlg.GetPathName(), "rb");

		if (RawDlg.DoModal() == IDOK)
		{
			if (fp == NULL)
			{
				MessageBox("파일경로가 잘못됐어요 프로젝트 폴더 안에 옮겨주세요");
				return;
			}

			nHeight_Ori = RawDlg.nHeight;
			nWidth_Ori = RawDlg.nWidth;

			if (nHeight_Ori == 0 || nWidth_Ori == 0 || RawDlg.nFormat != 3)
			{
				MessageBox("입력 값이 잘못되었습니다.");
				return;
			}

			Edit_File_Ori.SetWindowTextA(dlg.GetFileTitle());
			Ori = MemAlloc2D(nHeight_Ori, nWidth_Ori, 0);

			for (int h = 0; h < nHeight_Ori; h++)
			{
				fread(Ori[h], sizeof(unsigned char), nWidth_Ori, fp);
			}
		}
		fclose(fp);
	}
}


void PSNRDlg::OnBnClickedButtonOpen2()
{
	CFileDialog dlg(TRUE);
	CRawInfoDlg RawDlg;

	if (Dst != NULL)
	{
		MemFree2D(Dst, nHeight_Dst);
	}

	if (dlg.DoModal() == IDOK)
	{
		FILE *fp;
		fopen_s(&fp, dlg.GetPathName(), "rb");
		if (RawDlg.DoModal() == IDOK)
		{
			nHeight_Dst = RawDlg.nHeight;
			nWidth_Dst = RawDlg.nWidth;

			if (nHeight_Dst == 0 || nWidth_Dst == 0 || RawDlg.nFormat != 3)
			{
				MessageBox("입력 값이 잘못되었습니다.");
				return;
			}

			Edit_File_Dst.SetWindowTextA(dlg.GetFileTitle());
			Dst = MemAlloc2D(nHeight_Dst, nWidth_Dst, 0);

			for (int h = 0; h < nHeight_Dst; h++)
			{
				fread(Dst[h], sizeof(unsigned char), nWidth_Dst, fp);
			}
		}
		fclose(fp);
	}
}


void PSNRDlg::OnBnClickedButtonPsnr()
{
	if (Ori == NULL || NULL)
	{
		MessageBox("원본 영상과 비교 영상을 불러오지 않았습니다.");
		return;
	}

	if (nHeight_Ori != nHeight_Dst || nWidth_Ori != nWidth_Dst)
	{
		MessageBox("원본 영상과 비교 영상의 크기가 다릅니다.");
		return;
	}

	CString strTemp;
	double dPSNR = GetPSNR(Ori, Dst, nHeight_Ori, nWidth_Ori);

	strTemp.Format(_T("%f"), dPSNR);
	Edit_PSNR.SetWindowTextA(strTemp);
}

double PSNRDlg::GetPSNR(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth) {

	double mse = 0;
	double PSNR = 0;
	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			mse += abs(Img_in[h][w] - Out[h][w])*abs(Img_in[h][w] - Out[h][w]);
		}
	}
	mse /= (double)(nHeight*nWidth);
	return PSNR = 10 * log10(255.0 * 255.0 / mse);
}

void PSNRDlg::OnBnClickedCancel()
{
	if (Ori != NULL)
	{
		MemFree2D(Ori, nHeight_Ori);
	}

	if (Dst != NULL)
	{
		MemFree2D(Dst, nWidth_Dst);
	}

	CDialogEx::OnCancel();
}
