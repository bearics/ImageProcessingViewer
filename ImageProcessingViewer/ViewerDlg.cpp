
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