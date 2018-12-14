
// ViewerDlg.h : header file
//

#pragma once

#include <opencv2/opencv.hpp>
#include "CRawInfoDlg.h"
#include "SCALEDIALOG.h"
#include "RotationDlg.h"

using namespace cv;

// ViewerDlg dialog
class ViewerDlg : public CDialog
{
// Construction
public:
	ViewerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGEPROCESSINGVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	unsigned char **ch_in_1, **ch_in_2, **ch_in_3;
	unsigned char **ch_in_gray;

	int* Hist;

	int nHeight_in, nWidth_in;
	int nFormat;
	bool isImageOpened;

	CStatic m_PicOri;
	CStatic m_PicProc;
	CRawInfoDlg RawInfoDlg;
	SCALEDIALOG ImgScalingDlg;
	RotationDlg rotationDlg;

	// For Display
	Mat Ori_Img, Proc_Img;
	CRect rect;
	CDC* m_pDC;

	enum ImageFormat
	{
		FORMAT_RGB,
		FORMAT_YUV444,
		FORMAT_YUV420,
		FORMAT_GRAY
	};

	void DisplayImage(Mat DispMat, bool bInOut);
	void DisplayBitmap(CDC *pDC, CRect rect, IplImage* DisplplImage);
	void ResizeImage(IplImage* src, IplImage** dst, CRect* rect);

	enum InterpolationType
	{
		BILINEAR,
		BICUBIC
	};

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnEditConvert();
	afx_msg void OnEditScaling();
	afx_msg void OnEditRotation();
	afx_msg void OnHistogramGethistogram();
	afx_msg void OnHistogramHistogramequalization();
	afx_msg void OnHistogramHistogrammatching();
};

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal);
void MemFree2D(unsigned char **Mem, int nHeight);

// interpolation
bool isInsideBoundary(int nHeight, int nWidth, double h, double w);
unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);
unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);

void DisplayHistogram(int *Hist);