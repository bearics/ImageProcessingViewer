
// ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessingViewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// ViewerDlg dialog



ViewerDlg::ViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_IMAGEPROCESSINGVIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_IN, m_PicOri);
	DDX_Control(pDX, IDC_PICTURE_OUT, m_PicProc);
}

BEGIN_MESSAGE_MAP(ViewerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &ViewerDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &ViewerDlg::OnFileSave)
	ON_COMMAND(ID_FILE_CLOSE, &ViewerDlg::OnFileClose)
	ON_COMMAND(ID_EDIT_CONVERT, &ViewerDlg::OnEditConvert)
	ON_COMMAND(ID_EDIT_SCALING, &ViewerDlg::OnEditScaling)
	ON_COMMAND(ID_EDIT_ROTATION, &ViewerDlg::OnEditRotation)
	ON_COMMAND(ID_HISTOGRAM_GETHISTOGRAM, &ViewerDlg::OnHistogramGethistogram)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &ViewerDlg::OnHistogramHistogramequalization)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &ViewerDlg::OnHistogramHistogrammatching)
	ON_COMMAND(ID_EDIT_FILTER, &ViewerDlg::OnEditFilter)
	ON_COMMAND(ID_NOISEREDUCTION_ADDNOISE, &ViewerDlg::OnNoisereductionAddnoise)
	ON_COMMAND(ID_NOISEFILTERING_MEANFILTER, &ViewerDlg::OnNoisefilteringMeanfilter)
	ON_COMMAND(ID_NOISEFILTERING_ORDER, &ViewerDlg::OnNoisefilteringOrder)
	ON_COMMAND(ID_NOISEREDUCTION_GETPSNR, &ViewerDlg::OnNoisereductionGetpsnr)
END_MESSAGE_MAP()


// ViewerDlg message handlers

BOOL ViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void ViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void ViewerDlg::DisplayImage(Mat DispMat, bool bInOut)
{
	if (bInOut)
	{
		GetDlgItem(IDC_PICTURE_IN)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicOri.GetDC();
		m_PicOri.GetClientRect(&rect);
	}
	else
	{
		GetDlgItem(IDC_PICTURE_OUT)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicProc.GetDC();
		m_PicProc.GetClientRect(&rect);
	}

	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;

	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}

void ViewerDlg::DisplayBitmap(CDC *pDC, CRect rect, IplImage* DispIplImage)
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DispIplImage->width;
	bitmapInfo.bmiHeader.biHeight = DispIplImage->height;

	if (DispIplImage->nChannels == 3)
	{
		IplImage* ReverseImg = cvCreateImage(CvSize(DispIplImage->width, DispIplImage->height), 8, 3);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3] = DispIplImage->imageData[(DispIplImage->height - h - 1)*DispIplImage->widthStep + w * 3];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 1] = DispIplImage->imageData[(DispIplImage->height - h - 1)*DispIplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h*DispIplImage->widthStep + w * 3 + 2] = DispIplImage->imageData[(DispIplImage->height - h - 1)*DispIplImage->widthStep + w * 3 + 2];
			}
		}

		bitmapInfo.bmiHeader.biBitCount = DispIplImage->depth * DispIplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, DispIplImage->width, DispIplImage->height, ReverseImg->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
	}

	else
	{
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 1);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h*DispIplImage->widthStep + w] = DispIplImage->imageData[(DispIplImage->height - h)*DispIplImage->widthStep + w];
			}
		}

		IplImage* tmp = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount = tmp->depth*tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);
	}
}

void ViewerDlg::ResizeImage(IplImage* src, IplImage** dst, CRect* rect)
{
	int ndst_width = 0, ndst_height = 0;

	if (src->width > src->height)
	{
		ndst_width = rect->Width();
		ndst_height = (src->height*rect->Width()) / src->width;
	}
	else
	{
		ndst_width = (src->width * rect->Height()) / src->height;
		ndst_height = rect->Height();
	}
	(*dst) = cvCreateImage(cvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);

	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

void MemFree2D(unsigned char **Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}

void ViewerDlg::OnFileOpen()
{
	CFileDialog dlg(TRUE);

	if (isImageOpened)
	{
		if (ch_in_1 != NULL)
		{
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL)
		{
			if (nFormat == FORMAT_YUV444)
			{
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL)
		{
			if (nFormat == FORMAT_YUV420)
			{
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL)
		{
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetFileExt() != "jpg" && dlg.GetFileExt() != "JPG" && dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "yuv")
		{
			MessageBox("JPG 또는 YUV, Gray 파일이 아닙니다.");
			return;
		}

		InpFileName = dlg.GetFileTitle();

		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG")
		{
			Ori_Img = imread((const char*)dlg.GetPathName());
			nHeight_in = Ori_Img.rows;
			nWidth_in = Ori_Img.cols;
			if (Ori_Img.channels() == 3)
			{
				ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);

				for (int h = 0; h < nHeight_in; h++)
				{
					for (int w = 0; w < nWidth_in; w++)
					{
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						ch_in_1[h][w] = RGB[2];
						ch_in_2[h][w] = RGB[1];
						ch_in_3[h][w] = RGB[0];
					}
				}
				nFormat = FORMAT_RGB;
			}
			else
			{
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
				for (int h = 0; h < nHeight_in; h++)
				{
					for (int w = 0; w < nWidth_in; w++)
					{
						ch_in_gray[h][w] = Ori_Img.at<unsigned char>(h, w);
					}
				}
				nFormat = FORMAT_GRAY;
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
		else
		{
			CRawInfoDlg RawInfoDlg;
			if (RawInfoDlg.DoModal() == IDOK)
			{
				nHeight_in = RawInfoDlg.nHeight;
				nWidth_in = RawInfoDlg.nWidth;
				nFormat = RawInfoDlg.nFormat;

				if (nHeight_in == 0 || nWidth_in == 0)
				{
					MessageBox("값을 잘 못 입력하였습니다.");
					return;
				}

				FILE *fp_in;
				fopen_s(&fp_in, (const char*)dlg.GetPathName(), "rb");

				if (nFormat == FORMAT_GRAY)
				{
					ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_gray[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					for (int h = 0; h < nHeight_in; h++)
					{
						for (int w = 0; w < nWidth_in; w++)
						{
							Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
						}
					}
				}
				else if (nFormat == FORMAT_YUV444)
				{
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_1, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_2, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_3, sizeof(unsigned char), nWidth_in, fp_in);
					}
				}
				else
				{
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					ch_in_3 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					for (int h = 0; h < nHeight_in; h++)
					{
						fread(ch_in_1, sizeof(unsigned char), nWidth_in, fp_in);
					}
					for (int h = 0; h < nHeight_in / 2; h++)
					{
						fread(ch_in_2, sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
					for (int h = 0; h < nHeight_in / 2; h++)
					{
						fread(ch_in_3, sizeof(unsigned char), nWidth_in / 2, fp_in);
					}
				}
				fclose(fp_in);

				isImageOpened = true;
				DisplayImage(Ori_Img, true);
			}

		}
	}
}


void ViewerDlg::OnFileSave()
{
	// TODO: Add your command handler code here
}


void ViewerDlg::OnFileClose()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened)
	{
		if (ch_in_1 != NULL)
		{
			MemFree2D(ch_in_1, nHeight_in);
		}
		if (ch_in_2 != NULL)
		{
			if (nFormat == FORMAT_YUV444)
			{
				MemFree2D(ch_in_2, nHeight_in / 2);
			}
			else
				MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL)
		{
			if (nFormat == FORMAT_YUV420)
			{
				MemFree2D(ch_in_3, nHeight_in / 2);
			}
			else
				MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL)
		{
			MemFree2D(ch_in_gray, nHeight_in);
		}
	}

	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}



void ViewerDlg::OnEditConvert()
{

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened)
	{
		if (nFormat == FORMAT_GRAY)
		{
			MessageBox("입력 영상이 회색조 영상입니다.");
			return;
		}

		if (nFormat == FORMAT_YUV444 || nFormat == FORMAT_YUV420)
		{
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = ch_in_1[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			return;
		}

		if (nFormat == FORMAT_RGB)
		{
			if (ch_in_gray == NULL)
			{
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
			}

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					ch_in_gray[h][w] = saturate_cast<unsigned char>(0.299*ch_in_1[h][w] + 0.587*ch_in_2[h][w] + 0.114*ch_in_3[h][w]);
				}
			}

			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}

			DisplayImage(Proc_Img, false);
			return;
		}
	}
	else
	{
		MessageBox("불러온 영상이 없습니다.");
	}

}


void ViewerDlg::OnEditScaling()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)//영상이 열려있고 영상 포맷이 Gray
	{
		if (ImgScalingDlg.DoModal() == IDOK)
		{
			int nInterpolationType = ImgScalingDlg.nInterpolationType;
			double dScaleRate = ImgScalingDlg.dScaleRate;
			if (dScaleRate < 1 || dScaleRate >2)
			{
				MessageBox("스케일링 범위가 잘못되었습니다.");
				return;
			}
			int nHeight_Scale = nHeight_in * dScaleRate;
			int nWidth_Scale = nWidth_in * dScaleRate;

			unsigned char** Out = MemAlloc2D(nHeight_Scale, nWidth_Scale, 0);

			for (int h = 0; h < nHeight_Scale; h++)
			{
				for (int w = 0; w < nWidth_Scale; w++)
				{
					double h_Ori = h / dScaleRate;

					double w_Ori = w / dScaleRate;
					if (isInsideBoundary(nHeight_Scale, nWidth_Scale, h_Ori, w_Ori))
					{
						if (nInterpolationType == BILINEAR) {
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
						}
						else {
							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
						}

					}
				}
			}

			Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			Proc_Img = Mat(nHeight_Scale, nWidth_Scale, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}
			for (int h = 0; h < nHeight_Scale; h++)
			{
				for (int w = 0; w < nWidth_Scale; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}

			imshow("Original Image", Ori_Img);
			imshow("Scaled Image", Proc_Img);
			waitKey(0);
		}
	}
	else
	{
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

bool isInsideBoundary(int nHeight, int nWidth, double h, double w)
{
	if (h >= 0 && w >= 0 && h < nHeight && w < nWidth)
	{
		return true;
	}
	else return false;
}

unsigned char BilinearInterpolation(unsigned char** In,   int sy, int sx,  double ori_y, double ori_x )
{
	unsigned char m[2][2];

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			int py = floor(ori_y) + i < sy ? floor(ori_y) + i : floor(ori_y);
			int px = floor(ori_x) + j < sx ? floor(ori_x) + j : floor(ori_x);
			m[i][j] = In[py][px];
		}
	}

	double d1 = ori_x - floor(ori_x);
	double d2 = 1 - d1;
	double d3 = ori_y - floor(ori_y);
	double d4 = 1 - d3;

	return (unsigned char)(d4 * (d2 * (int)m[0][0] + d1 * (int)m[0][1]) + d3 * (d2 * (int)m[1][0] + d1 * (int)m[1][1]));

}

unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt)
{
	int h = (int)h_Cvt;
	int w = (int)w_Cvt;

	if (h == 0) {
		return 0;
	}
	else if (w == 0) {
		return 0;
	}
	else if (w > nWidth_Ori - 1 - 3) {
		return 0;
	}
	else if (h > nHeight_Ori - 1 - 3) {
		return 0;
	}

	else {
		double inverMatrix[4][4] = { { -0.166667, 0.5, -0.5, 0.166667 },
									{ 0.5, -1, 0.5,0 },
									{-0.333333, -0.5, 1, -0.166667},
									{ 0,	1,	0,	0 }
		};
		double first_cal[4];
		double mid_cal[4];
		double last_cal[4];
		int result;
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				first_cal[i] = inverMatrix[i][0] * In[(h - 1) + j][w - 1] + inverMatrix[i][1] * In[(h - 1) + j][w] +
					inverMatrix[i][2] * In[(h - 1) + j][w + 1] + inverMatrix[i][3] * In[(h - 1) + j][w + 2];
			}
			mid_cal[j] = (w_Cvt - w)*(w_Cvt - w)*(w_Cvt - w)*first_cal[0] + (w_Cvt - w)*(w_Cvt - w)*first_cal[1] + (w_Cvt - w)*first_cal[2] + first_cal[3];
		}

		for (int i = 0; i < 4; i++)
		{
			last_cal[i] = inverMatrix[i][0] * mid_cal[0] + inverMatrix[i][1] * mid_cal[1] +
				inverMatrix[i][2] * mid_cal[2] + inverMatrix[i][3] * mid_cal[3];
		}

		return result = (unsigned char) ((h_Cvt - h)*(h_Cvt - h)*(h_Cvt - h)*last_cal[0] + (h_Cvt - h)*(h_Cvt - h)*last_cal[1] + (h_Cvt - h)*last_cal[2] + last_cal[3]);
	}

}


void ViewerDlg::OnEditRotation()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)//영상이 열려있고 영상 포맷이 Gray
	{
		if (rotationDlg.DoModal() == IDOK)
		{
			int nInterpolationType = rotationDlg.nInterpolationType;
			double dRodateRate = rotationDlg.dRotateRate;
			
			if (dRodateRate < 0 || dRodateRate > 360)
			{
				MessageBox("회전 범위가 잘못되었습니다.");
				return;
			}

			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			int nHeight_2 = nHeight_in / 2,  nWidth_2 = nWidth_in / 2;

			double PI = 3.14;

			double h_Rotate = 0, w_Rotate = 0;

			double sinTheta = (double)sin(dRodateRate * (PI / 180.0));
			double cosTheta = (double)cos(dRodateRate * (PI / 180.0));

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++) {
					//회전한 좌표 계산
					int r_w = w - nWidth_2;
					int r_h = h - nHeight_2;

					w_Rotate = cosTheta * r_w - sinTheta * r_h + nWidth_2;
					h_Rotate = sinTheta * r_w + cosTheta * r_h + nHeight_2;

					if (isInsideBoundary(nHeight_in, nWidth_in, h_Rotate, w_Rotate))
					{
						if (nInterpolationType == BILINEAR) {
							Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
						else {

							Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						}
					}
				}
			}

			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++)
			{
				for (int w = 0; w < nWidth_in; w++)
				{
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			MemFree2D(Out, nHeight_in);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

void ViewerDlg::OnHistogramGethistogram()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL)
		{
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		//GetHistogram
		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist[ch_in_gray[h][w]]++;
			}
		}
		//Normalization
		int nMax = 0;

		for (int n = 0; n < 256; n++)
		{
			if (nMax < Hist[n])
			{
				nMax = Hist[n];
			}
		}

		double dNormalizeFactor = 255.0 / nMax;

		Mat HistDisp = Mat(256, 256, CV_8UC1);
		HistDisp = Scalar::all(0);

		for (int w = 0; w < 256; w++)
		{
			int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
			for (int h = 255; h > 255 - nNormalizedValue; h--)
			{
				HistDisp.at<unsigned char>(h, w) = 255;
			}
		}
		imshow("Histogram of Input Image", HistDisp);
		waitKey(0);
	}
}


void ViewerDlg::OnHistogramHistogramequalization()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL)
		{
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		// Get Histogram
		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist[ch_in_gray[h][w]]++;
			}
		}

		// Histogram Equalization
		double Hist_CDF[256] = { 0.0 };
		Mat Eq_Img = Mat(256, 256, CV_8UC1);
		int histSum = 0;

		for (int i = 0; i < 256; i++)
		{
			histSum = Hist[i] + histSum;
			Hist_CDF[i] = (double)histSum / (nHeight_in *nWidth_in);
		}
		
		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Eq_Img.at<unsigned char>(h, w) = (Hist_CDF[ch_in_gray[h][w]] * 255);
			}
		}

		// Display Result
		int *Hist_Eq = new int[256];
		memset(Hist_Eq, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist_Eq[Eq_Img.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(Eq_Img, false);
		DisplayHistogram(Hist);
		DisplayHistogram(Hist_Eq);

		delete[] Hist_Eq;
	}
}

void DisplayHistogram(int *Hist)
{
	//Normalization
	int nMax = 0;

	for (int n = 0; n < 256; n++)
	{
		if (nMax < Hist[n])
		{
			nMax = Hist[n];
		}
	}

	double dNormalizeFactor = 255.0 / nMax;

	Mat HistDisp = Mat(256, 256, CV_8UC1);
	HistDisp = Scalar::all(0);

	for (int w = 0; w < 256; w++)
	{
		int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
		for (int h = 255; h > 255 - nNormalizedValue; h--)
		{
			HistDisp.at<unsigned char>(h, w) = 255;
		}
	}

	imshow("Histogram", HistDisp);
	waitKey(0);
}


void ViewerDlg::OnHistogramHistogrammatching()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL)
		{
			Hist = new int[256];
		}
		memset(Hist, 0, sizeof(int) * 256);

		// Get Histogram
		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist[ch_in_gray[h][w]]++;
			}
		}

		// Histogram Matching

		//선언 & 초기화
		Mat MatchImg(256, 256, CV_8UC1); Mat RefImg(nHeight_in, nWidth_in, CV_8UC1);
		int Hist_Ref[256] = { 0 }; int Hist_Mat[256] = { 0 };
		double Hist_CDF[256], Hist_CDF_Ref[256];

		MatchImg = Scalar::all(0);
		RefImg = Scalar::all(0);

		// Reference 영상 생성 및 히스토그램 계산
		srand(GetTickCount());
		for (int h = 0; h < RefImg.rows; h++)
		{
			for (int w = 0; w < RefImg.cols; w++)
			{
				RefImg.at<unsigned char>(h, w) = (unsigned char)rand() % 255;
				Hist_Ref[RefImg.at<unsigned char>(h, w)]++;
			}
		}

		// CDF 구하기
		Hist_CDF[0] = (double)Hist[0] / (nHeight_in * nWidth_in);
		Hist_CDF_Ref[0] = (double)Hist_Ref[0] / (nHeight_in * nWidth_in);
		for (int n = 1; n < 256; n++)
		{
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in * nWidth_in) + Hist_CDF[n - 1];
			Hist_CDF_Ref[n] = (double)Hist_Ref[n] / (nHeight_in * nWidth_in) + Hist_CDF_Ref[n - 1];
		}

		// Histogram Matching 과정을 통하여 Matching 결과를 Mat에 저장
		for (int h = 0; h < MatchImg.rows; h++)
		{
			for (int w = 0; w < MatchImg.cols; w++)
			{
				MatchImg.at<unsigned char>(h, w) = (Hist_CDF_Ref[(int)(Hist_CDF[ch_in_gray[h][w]] * 255)] * 255);
			}
		}


		//Display Result
		for (int h = 0; h < nHeight_in; h++)
		{
			for (int w = 0; w < nWidth_in; w++)
			{
				Hist_Mat[MatchImg.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(MatchImg, false);
		DisplayHistogram(Hist_Ref);
		DisplayHistogram(Hist_Mat);
	}
}

unsigned char** ViewerDlg::Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}

	for (int h = 0; h < nPadSize; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nPadSize; w++)
		{
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++)
	{
		for (int w = 0; w < nPadSize; w++)
		{
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}

	return Pad;
}

void ViewerDlg::OnEditFilter()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		FilterDlg filterDlg;
		unsigned char** Pad, **Out;

		if (filterDlg.DoModal() == IDOK)
		{
			int nFilterType = filterDlg.nFilterType;
			int nFilterSize = filterDlg.nFilterSize;
			switch (nFilterType)
			{
			case FILTER_AVERAGE:
				if (nFilterSize >= 3)
				{
					if (nFilterSize % 2 == 0)
					{
						nFilterSize++;
					}
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

					AverageFilter(Pad, Out, nHeight_in, nWidth_in, nFilterSize);

					MemFree2D(Pad, nHeight_in + (int)(nFilterSize / 2) * 2);
					MemFree2D(Out, nHeight_in);
				}
				else MessageBox("FilterSize가 잘못되었습니다. FilterSize는 3 이상의 정수 값을 가져야 합니다.");
				break;

			case FILTER_SMOOTH:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				SmoothFilter(Pad, Out, nHeight_in, nWidth_in);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			case FILTER_SHARPEN:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				SharpenFilter(Pad, Out, nHeight_in, nWidth_in);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			case FILTER_GAUSSIAN:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 3);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				GaussianFilter(Pad, Out, nHeight_in, nWidth_in);

				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);
				break;
			}
		}
	}
	else
	{
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}

}

void ViewerDlg::AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int PadSize = (int)(nFilterSize / 2);
	int filter = PadSize * 2 + 1;
	double sum = 0;
	Mat imgResult(nHeight, nWidth, CV_8UC1);

	unsigned char** Mask = MemAlloc2D(filter, filter, (double)1 / filter * filter);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			sum = 0;
			for (int i = 0; i < filter; i++)
				for (int j = 0; j < filter; j++)
					sum += In_Pad[h + i][w + j] * (1.0 / (double)(filter*filter));

			Out[h][w] = (int)sum;
		}
	}

	for (int h = 0; h < imgResult.rows; h++)
	{
		for (int w = 0; w < imgResult.cols; w++)
		{
			imgResult.at<unsigned char>(h, w) = Out[h][w];
		}
	}
	DisplayImage(imgResult, false);

	MemFree2D(Mask, filter);
}

void ViewerDlg::SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth)
{
	int PadSize = 1;
	int filterSize = PadSize * 2 + 1;

	double filter[3][3] = {
		{ 1, 2, 1 },
		{ 2 , 4 , 2 },
		{ 1, 2 , 1}
	};

	Mat imgResult(nHeight, nWidth, CV_8UC1);
	
	double sum = 0;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			sum = 0;

			for (int i = 0; i < filterSize; i++)
				for (int j = 0; j < filterSize; j++)
					sum += In_Pad[h + i][w + j] * ((double)filter[i][j] / 16);

			Out[h][w] = (int)sum;
		}
	}

	for (int h = 0; h < imgResult.rows; h++)
	{
		for (int w = 0; w < imgResult.cols; w++)
		{
			imgResult.at<unsigned char>(h, w) = Out[h][w];
		}
	}
	DisplayImage(imgResult, false);
}

void ViewerDlg::SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth)
{
	int PadSize = 1;
	int filterSize = PadSize * 2 + 1;
	double filter[3][3] = {
			{0,-1,0},
			{-1,5,-1},
			{0,-1,0}
	};

	Mat imgResult(nHeight, nWidth, CV_8UC1);
	
	double sum = 0;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			sum = 0;
			for (int i = 0; i < filterSize; i++)
				for (int j = 0; j < filterSize; j++)
					sum += In_Pad[h + i][w + j] * ((double)filter[i ][j]);

			if (sum <= 0)sum = 0;
			if (sum >= 255)sum = 255;

			Out[h][w] = (unsigned char)sum;
		}
	}

	for (int h = 0; h < imgResult.rows; h++)
	{
		for (int w = 0; w < imgResult.cols; w++)
		{
			imgResult.at<unsigned char>(h, w) = Out[h][w];
		}
	}
	DisplayImage(imgResult, false);
}

void ViewerDlg::GaussianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth)
{
	int PadSize = 1;
	double gaussian[3][3];
	double result = 0;

	double stdv = 1;
	double r, s = 2.0 * stdv * stdv;
	double sum = 0.0;

	Mat imgResult(nHeight, nWidth, CV_8UC1);


	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			r = (x*x + y * y);
			gaussian[x + 1][y + 1] = (exp(-(r) / s)) / sqrt(3.14 * s);
			sum += gaussian[x + 1][y + 1];
		}
	}

	for (int i = 0; i < 3; ++i) // Loop to normalize the kernel
		for (int j = 0; j < 3; ++j)
			gaussian[i][j] /= sum;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			result = 0;
			for (int i = 0; i < PadSize*2 +1; i++)
				for (int j = 0; j < PadSize * 2 + 1; j++)
				{
					result += In_Pad[h + i][w + j] * gaussian[i][j];
				}

			Out[h][w] = result;
		}
	}

	for (int h = 0; h < imgResult.rows; h++)
	{
		for (int w = 0; w < imgResult.cols; w++)
		{
			imgResult.at<unsigned char>(h, w) = Out[h][w];
		}
	}
	DisplayImage(imgResult, false);

}

void ViewerDlg::OnNoisereductionAddnoise()
{
	AddNoise AddNoiseDlg;
	BYTE** NoiseImg = MemAlloc2D(nHeight_in, nWidth_in, 0);
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{

		if (AddNoiseDlg.DoModal() == IDOK)
		{
			CString OutFileName = InpFileName;
			float Value_1 = AddNoiseDlg.value_1;
			float Value_2 = AddNoiseDlg.value_2;
			int nNoiseType = AddNoiseDlg.NoiseType;

			if (nNoiseType == 0)
			{
				if (Value_2 < 1)
				{
					MessageBox("표준편차 값이 잘못되었습니다. 표준편차 값은 1 이상이여야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputGaussianNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "_GN.raw";

			}
			else
			{
				if (Value_1 <= 0 && Value_2 <= 0)
				{
					MessageBox("두 개의 확률 값 중 적어도 하나는 0보다 커야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}
				InputSaltPepperNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				OutFileName += "_SPN.raw";

			}

			FILE* fp;

			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++)
			{
				fwrite(NoiseImg[h], sizeof(unsigned char), nWidth_in, fp);
			}

			fclose(fp);
			MemFree2D(NoiseImg, nHeight_in);
		}
	}
}

void InputGaussianNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fMean, float fStdev)
{
	float fTemp = 0, fPDF[256] = { 0.0f };

	GetGaussianPDF(fPDF, 256, fMean, fStdev);

	srand(GetTickCount());

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			fTemp = (float)In[h][w] + GetNoise(fPDF, 256);
			Out[h][w] = static_cast<unsigned char>(fTemp);
		}
	}
}

void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev)
{
	int n;
	int Center = nLength / 2;
	float x;
	double PI = 3.141592;

	for (n = 0; n < nLength; n++)
	{
		x = (float)(n - Center);
		EmptyPDF[n] = (1 / ((float)sqrt(2 * PI) * fStDev)) * exp(-pow(x - fMean, 2) / (2 * fStDev * fStDev));
	}
}

float GetNoise(float* PDF, int nLength)
{
	int n;
	int Center = nLength / 2;
	float fRand, fComp, fTemp = 0;
	float x = 0, fDiff;
	float* CDF = new float[nLength];

	CDF[0] = 0;

	fRand = (float)rand() / (RAND_MAX + 1); // 0~1 Uniform Distribution

	for (n = 1; n < nLength; n++)
	{
		CDF[n] = (PDF[n] + PDF[n - 1]) / 2 + CDF[n - 1];
		fDiff = fRand - CDF[n];
		if (fDiff < 0)
		{
			x = ((float)n - Center);
			break;
		}
	}
	fComp = (fRand - CDF[n - 1]) / (CDF[n] - CDF[n - 1]);

	delete[] CDF;

	return x + fComp;
}

void InputSaltPepperNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fSProb, float fPProb)
{
	float Low = fSProb;
	float High = 1.0f - fPProb;
	float fRand;

	srand(GetTickCount());

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			fRand = ((float)rand() / RAND_MAX);

			if (fRand < Low)
			{
				Out[h][w] = 255;
			}
			else if (fRand > High)
			{
				Out[h][w] = 0;
			}
			else Out[h][w] = In[h][w];
		}
	}
}

Mat GrayToMat(unsigned char** Img, int nHeight, int nWidth)
{
	Mat rtn = Mat(nHeight, nWidth, CV_8UC1);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			rtn.at<unsigned char>(h, w) = Img[h][w];
		}
	}

	return rtn;
}

void ViewerDlg::OnNoisefilteringMeanfilter()
{
	MeanFilterDlg MFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY)
	{
		if (MFDlg.DoModal() == IDOK)
		{
			int nFilterSize = MFDlg.nFilterSize;
			int nFilterType = MFDlg.nMeanFilterType;

			if (nFilterSize < 3)
			{
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}

			if (nFilterSize % 2 == 0)
			{
				nFilterSize++;
			}

			CString OutFileName = InpFileName;
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);


			switch (nFilterType)
			{
			case NF_ARITHMETIC:
				ArithmeticMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_AMF.raw";
				break;

			case NF_GEOMETRIC:
				GeometricMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_GMF.raw";
				break;

			case NF_HARMONIC:
				HarmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "HMF.raw";
				break;

			case NF_CONTRAHARMONIC:
				int Q = MFDlg.Q;
				ContraharmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, Q);
				OutFileName += "_CHMF.raw";
				break;
			}

			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

			FILE *fp;

			fopen_s(&fp, OutFileName, "wb");

			for (int h = 0; h < nHeight_in; h++)
			{
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}

			fclose(fp);
		}
	}
}

void ViewerDlg::ArithmeticMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double nTemp;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			nTemp = 0;
			for (int y = 0; y < nFilterSize; y++)
			{
				for (int x = 0; x < nFilterSize; x++)
				{
					nTemp += In_Pad[h + y][w + x];
				}
			}

			Out[h][w] = static_cast<unsigned char>(nTemp / (nFilterSize*nFilterSize));

		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}

void ViewerDlg::GeometricMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double Temp;
	double result;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Temp = 1;
			for (int y = 0; y < nFilterSize; y++)
			{
				for (int x = 0; x < nFilterSize; x++)
				{
					Temp *= In_Pad[h + y][w + x];
				}
			}

			result = pow(Temp, (double)1 / (nFilterSize*nFilterSize));
			Out[h][w] = static_cast<unsigned char>(result);
		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}


void ViewerDlg::HarmonicMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	double Temp;


	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			Temp = 0;
			for (int y = 0; y < nFilterSize; y++)
			{
				for (int x = 0; x < nFilterSize; x++)
				{
					Temp += ((double)1 / In_Pad[h + y][w + x]);
				}
			}

			Temp = (double)(nFilterSize*nFilterSize) / Temp;
			Out[h][w] = static_cast<unsigned char>(Temp);
		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}

void ViewerDlg::ContraharmonicMeanFilter(unsigned char** Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, int Q)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	double up = 0;
	double down = 0;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			up = 0;
			down = 0;
			for (int y = 0; y < nFilterSize; y++)
			{
				for (int x = 0; x < nFilterSize; x++)
				{
					up += pow(In_Pad[h + y][w + x], Q + 1);
					down += pow(In_Pad[h + y][w + x], Q);
				}
			}
			Out[h][w] = static_cast<unsigned char>(up / down);
		}
	}
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}


void ViewerDlg::MedianFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	int med = (int)(nFilterSize*nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	unsigned char* sortMatrix = new unsigned char[nFilterSize*nFilterSize];

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			int sort_num = 0;
			for (int n = 0; n < nFilterSize; n++)
			{
				for (int m = 0; m < nFilterSize; m++)
				{
					sortMatrix[sort_num] = In_Pad[h + n][w + m];
					sort_num++;
				}
			}

			sort(sortMatrix, sortMatrix + nFilterSize * nFilterSize);
			Out[h][w] = sortMatrix[med];
		}
	}
	delete[] sortMatrix;
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}

void ViewerDlg::AdaptiveMedianFilter(unsigned char **Img_in, unsigned char **Out, int nHeight, int nWidth, int nFilterSize, int nFilterSize_Max)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** In_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);
	unsigned char* sortMatrix = new unsigned char[nFilterSize_Max*nFilterSize_Max];

	int nFilter_Sizeup;
	int A1, A2, Zxy, Zmed, B1, B2, Result;

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			A1 = 0, A2 = 0, Zmed = 0, B1 = 0, B2 = 0, Result = 0;
			nFilter_Sizeup = nFilterSize;

			Zxy = In_Pad[h + 1][w + 1];
			// Level A
			while (nFilter_Sizeup <= nFilterSize_Max)
			{
				// mask sortMatrix
				int sort_num = 0;
				memset(sortMatrix, 0, nFilterSize_Max*nFilterSize_Max);
				for (int n = 0; n < nFilter_Sizeup; n++)
					for (int m = 0; m < nFilter_Sizeup; m++)
					{
						// Out of Range
						if (h + n <= nHeight + 2 - 1 && h + m <= nWidth + 2 - 1)
						{
							sortMatrix[sort_num] = In_Pad[h + n][w + m];
							sort_num++;
						}
					}
				sort(sortMatrix, sortMatrix + sort_num);
				Zmed = sortMatrix[(sort_num) / 2];

				A1 = Zmed - sortMatrix[0];
				A2 = Zmed - sortMatrix[nFilter_Sizeup*nFilter_Sizeup - 1];

				// Level A → Level B
				if (A1 > 0 && A2 < 0)
					break;
				// Level A Repeat
				else
					nFilter_Sizeup += 2;
			}

			// Level B
			B1 = Zxy - sortMatrix[0];
			B2 = Zxy - sortMatrix[nFilter_Sizeup*nFilter_Sizeup - 1];
			if (B1 > 0 && B2 < 0)
			{
				Result = Zxy;
			}
			else
			{
				Result = Zmed;
			}

			Out[h][w] = (unsigned char)Result;
		}
	}
	delete[] sortMatrix;
	MemFree2D(In_Pad, nHeight + 2 * nPadSize);
}


void ViewerDlg::OnNoisefilteringOrder()
{
	OrderStatisticFilterDlg OSFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY)
	{
		if (OSFDlg.DoModal() == IDOK)
		{
			int nFilterSize = OSFDlg.nFilterSize1;
			int MaxFilterSize = OSFDlg.nFilterSize2;
			int nFilterType = OSFDlg.nOSFType;

			if (nFilterSize < 3)
			{
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}

			if (nFilterSize % 2 == 0)
			{
				nFilterSize++;
			}

			CString OutFileName = InpFileName;
			unsigned char** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			switch (nFilterType)
			{
			case NF_MEDIAN:
				ViewerDlg::MedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_MEDF.raw";
				break;

			case NF_ADAPTIVEMEDIAN:
				int MaxFilterSize = OSFDlg.nFilterSize2;
				AdaptiveMedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, MaxFilterSize);
				OutFileName += "_AMEDF.raw";
				break;
			}

			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

			FILE* fp;

			fopen_s(&fp, OutFileName, "wb");

			for (int h = 0; h < nHeight_in; h++)
			{
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}

			fclose(fp);
		}
	}
}


void ViewerDlg::OnNoisereductionGetpsnr()
{
	PSNRDlg PSNRDlg;
	PSNRDlg.DoModal();
}
