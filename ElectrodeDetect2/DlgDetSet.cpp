#include "pch.h"
#include "ElectrodeDetect2.h"
#include "DlgDetSet.h"
#include "afxdialogex.h"

#include "./../ZCVisionTool/ZCVisionTool.h"

#include "DlgDetSet_Cycle.h"
#include "DlgDetSet_UpBack.h"


// CDlgDetSet �Ի���

IMPLEMENT_DYNAMIC(CDlgDetSet, CDialogEx)

CDlgDetSet::CDlgDetSet(CWnd* pParent, CElectrodeDetect2Dlg* Parent)
	: CDialogEx(IDD_DETECTSETTING_DIALOG, pParent)
{
	m_pParent = Parent;
	m_SelectButton = IDC_MFCBUTTON_CYCLE;
	m_DlgDetSetCycle = NULL;
	m_DlgDetSetUpBack = NULL;
	m_SelectButton = IDC_MFCBUTTON_CYCLE;
}

CDlgDetSet::~CDlgDetSet()
{
}

void CDlgDetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_ELECTRODET, m_ShowImgControl);
	DDX_Control(pDX, IDC_MFCBUTTON_CLOSE, m_Btn_Close);
	DDX_Control(pDX, IDC_MFCBUTTON_LOAD, m_Btn_Load);
	DDX_Control(pDX, IDC_MFCBUTTON_PREVIMG, m_Btn_PrevImg);
	DDX_Control(pDX, IDC_MFCBUTTON_NEXTIMG, m_Btn_NextImg);
}


BEGIN_MESSAGE_MAP(CDlgDetSet, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON_CLOSE, &CDlgDetSet::OnBnClickedMfcbutton1)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MFCBUTTON_CYCLE, &CDlgDetSet::OnBnClickedMfcbuttonCycle)
	ON_BN_CLICKED(IDC_MFCBUTTON_UPBACK, &CDlgDetSet::OnBnClickedMfcbuttonUpback)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_LOAD, &CDlgDetSet::OnBnClickedMfcbuttonLoad)
	ON_BN_CLICKED(IDC_MFCBUTTON_PREVIMG, &CDlgDetSet::OnBnClickedMfcbuttonPrevimg)
	ON_BN_CLICKED(IDC_MFCBUTTON_NEXTIMG, &CDlgDetSet::OnBnClickedMfcbuttonNextimg)
END_MESSAGE_MAP()


// CDlgDetSet ��Ϣ�������


void CDlgDetSet::OnBnClickedMfcbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);

	m_pParent->m_Btn_DetSetting.SetButtonSelect(FALSE);
	m_pParent->m_Btn_DetSetting.TriggerInvalidate();
}


BOOL CDlgDetSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��ʼ��GDI
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetClientRect(&m_ClientRect);

	m_BigFont.CreatePointFont(150, _T("΢���ź�"));
	m_MiddleFont.CreatePointFont(100, _T("΢���ź�"));
	m_BGCBrush = CreateSolidBrush(COLOR_DETBACKGROUND);

	// ��ʼ����ť��ɫ
	//InitMfcButtonStyle(IDC_MFCBUTTON_CLOSE, COLOR_BACKGROUND_SET, COLOR_TEXT_BLACK);
	InitMfcButtonStyle(IDC_MFCBUTTON_CYCLE, COLOR_DETBACKGROUND_SET_MENU, COLOR_TEXT_WHITE);
	InitMfcButtonStyle(IDC_MFCBUTTON_UPBACK, COLOR_DETBACKGROUND_SET_MENU, COLOR_TEXT_WHITE);
	
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CLOSE))->SetFont(&m_BigFont);
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CYCLE))->SetFaceColor(COLOR_SETBUTTON_CLICK);

	// ���ð�ť����
	GetDlgItem(IDC_MFCBUTTON_CYCLE)->SetFont(&m_MiddleFont);
	GetDlgItem(IDC_MFCBUTTON_UPBACK)->SetFont(&m_MiddleFont);

	char bgiPath[1024] = { '\0' };
	sprintf_s(bgiPath,"%s\\Image\\�������.png", m_pParent->AppPath);
	m_pBGImage = m_pParent->LoadFromFile(bgiPath);

	CRect rect;
	GetDlgItem(IDC_STATIC_RIGHT_REGIN)->GetWindowRect(&rect);
	// �Ӵ���
	m_DlgDetSetCycle = new CDlgDetSet_Cycle(NULL,m_pParent);
	m_DlgDetSetCycle->Create(IDD_DETECTSETTING_ELECYCLE_DIALOG,this);
	m_DlgDetSetCycle->ShowWindow(SW_SHOW);

	m_DlgDetSetUpBack = new CDlgDetSet_UpBack(NULL, m_pParent);
	m_DlgDetSetUpBack->Create(IDD_DETECTSETTING_UPBACK_DIALOG, this);
	m_DlgDetSetUpBack->ShowWindow(SW_HIDE);


	/* ����ͼƬ����ť */
	char imgPath[1024] = { '\0' };

	// �رհ�ť
	sprintf_s(imgPath, "%s\\Image\\Quit.png", m_pParent->AppPath);
	Img_Close_Normal = m_pParent->LoadFromFile(imgPath);
	sprintf_s(imgPath, "%s\\Image\\Quit_Over.png", m_pParent->AppPath);
	Img_Close_Over = m_pParent->LoadFromFile(imgPath);
	// Ӧ�ð�ť
	sprintf_s(imgPath, "%s\\Image\\Ӧ��_Normal.png", m_pParent->AppPath);
	Img_SubMit_Normal = m_pParent->LoadFromFile(imgPath);
	sprintf_s(imgPath, "%s\\Image\\Ӧ��_Over.png", m_pParent->AppPath);
	Img_SubMit_Over = m_pParent->LoadFromFile(imgPath);
	// ���ذ�ť
	sprintf_s(imgPath, "%s\\Image\\����_Normal.png", m_pParent->AppPath);
	Img_Load_Normal = m_pParent->LoadFromFile(imgPath);
	sprintf_s(imgPath, "%s\\Image\\����_Over.png", m_pParent->AppPath);
	Img_Load_Over = m_pParent->LoadFromFile(imgPath);
	// ��һ��
	sprintf_s(imgPath, "%s\\Image\\��һ��_Normal.png", m_pParent->AppPath);
	Img_PrevImg_Normal = m_pParent->LoadFromFile(imgPath);
	sprintf_s(imgPath, "%s\\Image\\��һ��_Over.png", m_pParent->AppPath);
	Img_PrevImg_Over = m_pParent->LoadFromFile(imgPath);
	// ��һ��
	sprintf_s(imgPath, "%s\\Image\\��һ��_Normal.png", m_pParent->AppPath);
	Img_NextImg_Normal = m_pParent->LoadFromFile(imgPath);
	sprintf_s(imgPath, "%s\\Image\\��һ��_Over.png", m_pParent->AppPath);
	Img_NextImg_Over = m_pParent->LoadFromFile(imgPath);

	m_Btn_Close.LoadNormalImage(Img_Close_Normal);
	m_Btn_Close.LoadOverImage(Img_Close_Over);
	m_Btn_Close.SetButtonType(2);
	m_Btn_SubMit.LoadNormalImage(Img_SubMit_Normal);
	m_Btn_SubMit.LoadOverImage(Img_SubMit_Over);
	m_Btn_SubMit.SetButtonType(2);
	m_Btn_Load.LoadNormalImage(Img_Load_Normal);
	m_Btn_Load.LoadOverImage(Img_Load_Over);
	m_Btn_Load.SetButtonType(2);
	m_Btn_PrevImg.LoadNormalImage(Img_PrevImg_Normal);
	m_Btn_PrevImg.LoadOverImage(Img_PrevImg_Over);
	m_Btn_PrevImg.SetButtonType(2);
	m_Btn_NextImg.LoadNormalImage(Img_NextImg_Normal);
	m_Btn_NextImg.LoadOverImage(Img_NextImg_Over);
	m_Btn_NextImg.SetButtonType(2);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgDetSet::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnLButtonDown(nFlags, point);
	// �����ô����ƶ�
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}


void CDlgDetSet::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ChangeSize_Set(IDC_MFCBUTTON_CLOSE, cx, cy);
	ChangeSize_Set(IDC_MFCBUTTON_CYCLE, cx, cy);
	ChangeSize_Set(IDC_MFCBUTTON_UPBACK, cx, cy);
	ChangeSize_Set(IDC_STATIC_SET_MENU, cx, cy);
	ChangeSize_Set(IDC_STATIC_RIGHT_REGIN, cx, cy);
	ChangeSize_Set(IDC_CUSTOM_ELECTRODET, cx, cy);

	ChangeSize_Set(IDC_MFCBUTTON_LOAD, cx, cy);
	ChangeSize_Set(IDC_MFCEDITBROWSE_LOADIMG, cx, cy);
	ChangeSize_Set(IDC_MFCBUTTON_PREVIMG, cx, cy);
	ChangeSize_Set(IDC_MFCBUTTON_NEXTIMG, cx, cy);

	// ���ڳߴ�ı���Ӵ���λ��Ҳ���Ÿı�
	if (m_DlgDetSetCycle != NULL)
	{
		CRect rect;

		GetDlgItem(IDC_STATIC_RIGHT_REGIN)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_DlgDetSetCycle->MoveWindow(rect);
	}

	if (m_DlgDetSetUpBack != NULL)
	{
		CRect rect;

		GetDlgItem(IDC_STATIC_RIGHT_REGIN)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_DlgDetSetUpBack->MoveWindow(rect);
	}
}

void CDlgDetSet::ChangeSize_Set(UINT nID, int x, int y)
{
	CRect tempRect;

	CWnd* pWnd = GetDlgItem(nID);

	/* �ж��Ƿ����ı��� */
	bool isEdit = FALSE;
	char className[20];
	GetClassName(pWnd->GetSafeHwnd(), className, 20);
	isEdit = strcmp(className, "Edit") == 0;

	if (pWnd != NULL)
	{
		pWnd->GetWindowRect(&tempRect);
		ScreenToClient(&tempRect);

		if (isEdit)
		{
			tempRect.left = tempRect.left * x / m_ClientRect.Width();
			tempRect.top = tempRect.top * y / m_ClientRect.Height() + 3;
			tempRect.bottom = tempRect.bottom * y / m_ClientRect.Height() - 3;
			tempRect.right = tempRect.right * x / m_ClientRect.Width();
		}
		else
		{
			tempRect.left = tempRect.left * x / m_ClientRect.Width();
			tempRect.top = tempRect.top * y / m_ClientRect.Height();
			tempRect.bottom = tempRect.bottom * y / m_ClientRect.Height();
			tempRect.right = tempRect.right * x / m_ClientRect.Width();
		}

		pWnd->MoveWindow(tempRect);
	}
}

// ��ʼ��MFCButton����ʽ
void CDlgDetSet::InitMfcButtonStyle(int nID, COLORREF crFace, COLORREF crText)
{
	((CMFCButton*)GetDlgItem(nID))->m_bTransparent = FALSE;
	((CMFCButton*)GetDlgItem(nID))->m_bDontUseWinXPTheme = TRUE;
	((CMFCButton*)GetDlgItem(nID))->m_bDrawFocus = FALSE;
	((CMFCButton*)GetDlgItem(nID))->SetTextColor(crText);
	((CMFCButton*)GetDlgItem(nID))->SetFaceColor(crFace);
}

// ����MFCButton����ʽ
void CDlgDetSet::SetMfcButtonStyle(int nID, COLORREF crFace, COLORREF crText)
{
	((CMFCButton*)GetDlgItem(nID))->SetTextColor(crText);
	((CMFCButton*)GetDlgItem(nID))->SetFaceColor(crFace);
}

BOOL CDlgDetSet::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		CPoint tPoint = pMsg->pt;
		CRect rect;

		/*GetDlgItem(IDC_MFCBUTTON_CLOSE)->GetWindowRect(rect);
		if (rect.PtInRect(tPoint))
			SetMfcButtonStyle(IDC_MFCBUTTON_CLOSE, COLOR_BUTTON_OVER_QUIT, COLOR_TEXT_WHITE);
		else
			SetMfcButtonStyle(IDC_MFCBUTTON_CLOSE, COLOR_BACKGROUND_SET, COLOR_TEXT_BLACK);*/

		if (m_SelectButton != IDC_MFCBUTTON_CYCLE)
		{
			GetDlgItem(IDC_MFCBUTTON_CYCLE)->GetWindowRect(rect);
			if (rect.PtInRect(tPoint))
				((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CYCLE))->SetFaceColor(COLOR_SETBUTTON_OVER);
			else
				((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CYCLE))->SetFaceColor(COLOR_DETBACKGROUND_SET_MENU);
		}

		if (m_SelectButton != IDC_MFCBUTTON_UPBACK)
		{
			GetDlgItem(IDC_MFCBUTTON_UPBACK)->GetWindowRect(rect);
			if (rect.PtInRect(tPoint))
				((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_UPBACK))->SetFaceColor(COLOR_SETBUTTON_OVER);
			else
				((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_UPBACK))->SetFaceColor(COLOR_DETBACKGROUND_SET_MENU);
		}
	}

	if (pMsg->message == WM_LBUTTONUP)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_MFCBUTTON_CLOSE)->m_hWnd)
			SetMfcButtonStyle(IDC_MFCBUTTON_CLOSE, COLOR_BACKGROUND_SET, COLOR_TEXT_BLACK);
	}

	//if (pMsg->message == WM_LBUTTONDOWN)
	//{
	//	if (pMsg->hwnd == GetDlgItem(IDC_MFCBUTTON_CLOSE)->m_hWnd)
	//		((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CLOSE))->SetFaceColor(COLOR_BUTTON_CLICK_QUIT);
	//}

	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_DOWN
		|| pMsg->wParam == VK_RETURN || pMsg->wParam == VK_SHIFT
		|| pMsg->wParam == VK_SPACE || pMsg->wParam == VK_END
		|| pMsg->wParam == VK_HOME || pMsg->wParam == VK_LEFT
		|| pMsg->wParam == VK_UP || pMsg->wParam == VK_RIGHT
		|| pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgDetSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	Graphics graphics(dc.m_hDC);
	CRect rect;
	GetClientRect(&rect);

	// ����
	graphics.DrawImage(m_pBGImage, rect.left, rect.top, rect.Width(), rect.Height());
}


void CDlgDetSet::OnBnClickedMfcbuttonCycle()
{
	SwitchParaWindow(IDC_MFCBUTTON_CYCLE);
}


void CDlgDetSet::OnBnClickedMfcbuttonUpback()
{
	SwitchParaWindow(IDC_MFCBUTTON_UPBACK);
}

// �жϵ����ť
void CDlgDetSet::SwitchParaWindow(int SelectedButton)
{
	if (m_SelectButton == SelectedButton)
	{
		return;
	}

	m_SelectButton = SelectedButton;

	// �����а�ť��ɫ���
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CYCLE))->SetFaceColor(COLOR_DETBACKGROUND_SET_MENU);
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_UPBACK))->SetFaceColor(COLOR_DETBACKGROUND_SET_MENU);

	// �������д���
	m_DlgDetSetCycle->ShowWindow(SW_HIDE);
	m_DlgDetSetUpBack->ShowWindow(SW_HIDE);

	// ��ʾ������Ӵ���
	if (m_SelectButton == IDC_MFCBUTTON_CYCLE)
		ShowDetSetCYCLERegion();
	else if (m_SelectButton == IDC_MFCBUTTON_UPBACK)
		ShowDetSetUpBackRegion();
}


// ��ʾ��������
void CDlgDetSet::ShowDetSetCYCLERegion()
{
	m_DlgDetSetCycle->ShowWindow(SW_SHOW);

	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CYCLE))->SetFaceColor(COLOR_SETBUTTON_CLICK);
}

// ��ʾ��������
void CDlgDetSet::ShowDetSetUpBackRegion()
{
	m_DlgDetSetUpBack->ShowWindow(SW_SHOW);

	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_UPBACK))->SetFaceColor(COLOR_SETBUTTON_CLICK);
}

HBRUSH CDlgDetSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(COLOR_TEXT_WHITE);
		pDC->SetBkMode(TRANSPARENT);

		return m_BGCBrush;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

// ����ָ���ļ����е�ͼƬ
void CDlgDetSet::FindLocalFile(char* lpPath)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	strcpy_s(szFind, lpPath);
	strcat_s(szFind, "\\*.bmp");

	CString str;

	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);

	if (INVALID_HANDLE_VALUE == hFind)
		return;

	m_LocalImageName.clear();
	m_LocalImageIndex = 0;

	CString cFileName;

	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		}
		else
		{
			str.Format("%s", FindFileData.cFileName);
			str.Replace(".bmp", "");

			m_LocalImageName.push_back(str);
		}

		if (!FindNextFile(hFind, &FindFileData))
			break;
	}

	FindClose(hFind);
}

// �����ļ��е�.bmpͼƬ�������б���
void CDlgDetSet::LoadLocalImage()
{
	int ImageNum = m_LocalImageName.size();

	if (ImageNum <= 0)		// ɾ��
		return;

	if (m_LocalImageIndex >= ImageNum)
	{
		if (ImageNum == 0)
			m_LocalImageIndex = 0;
		else
			m_LocalImageIndex = ImageNum - 1;
	}

	if (m_LocalImageIndex < 0)
	{
		m_LocalImageIndex = 0;
	}

	char FilePath[1024] = { '\0' };
	cv::Mat LabeledImg;

	sprintf_s(FilePath, "%s\\%s.bmp", m_LocalImageFolder.GetBuffer(), m_LocalImageName[m_LocalImageIndex].GetBuffer());

	m_LocalImage = imread(FilePath, IMREAD_GRAYSCALE);

	// ���÷���ͼƬ���㷨����������ɵ�ͼƬ��ʾ����
	Cam1_Function(m_LocalImage, m_pParent->m_PtrCSIVisionIn, m_pParent->m_PtrCSIVisionOut, LabeledImg);

	m_ShowImgControl.UpdateImageData(LabeledImg.data, LabeledImg.cols, LabeledImg.rows, LabeledImg.total() * LabeledImg.channels(), LabeledImg.channels());
	m_ShowImgControl.TriggerInvalidate();
}


void CDlgDetSet::OnBnClickedMfcbuttonLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString file_Floler;
	GetDlgItem(IDC_MFCEDITBROWSE_LOADIMG)->GetWindowText(file_Floler);

	if (file_Floler.IsEmpty())
	{
		MessageBox("��ѡ��Ҫ������ļ���");
		return;
	}

	FindLocalFile(file_Floler.GetBuffer());		// ���ļ����е�ͼƬ���Ƽ��ص�������
	m_LocalImageFolder = file_Floler;

	LoadLocalImage();
}


void CDlgDetSet::OnBnClickedMfcbuttonPrevimg()
{
	m_LocalImageIndex--;
	LoadLocalImage();
}


void CDlgDetSet::OnBnClickedMfcbuttonNextimg()
{
	m_LocalImageIndex++;
	LoadLocalImage();
}
