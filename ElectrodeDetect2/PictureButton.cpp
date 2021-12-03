#include "pch.h"
#include "PictureButton.h"

#include "GlobalDefine.h"

#define BTN_PARENT_NUM	1
#define BTN_CHILD_NUM	2

// CMFCButton������
BEGIN_MESSAGE_MAP(CPictureButton, CMFCButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CPictureButton::CPictureButton()
{
	m_IsOver = false;    m_IsSelect = false;
	m_IsLogin = false;	 m_BtnType = 1;

	img_Select = NULL;		// ѡ��
	img_Normal = NULL;		// ��ͨ
	img_Over = NULL;		// ����
	img_ForBid = NULL;		// ��ֹ
}

CPictureButton::~CPictureButton()
{
	m_FrameBrush.DeleteObject();
	m_CommonBrush.DeleteObject();
}

// �������е�OnDrawItem()�б����ã���Ӧ����WM_DRAWITEM����Ӧ����
// ��������Ը���nIDCtl���ж����ĸ��ӿؼ���
void CPictureButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nSaveDC = pDC->SaveDC();

	CRect rct = lpDrawItemStruct->rcItem;

	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC, rct.Width(), rct.Height());
	CBitmap* pOldBitmap = MemDC.SelectObject(&MemBitmap);

	if(m_BtnType == 1)
		MemDC.FillSolidRect(0, 0, rct.Width(), rct.Height(), RGB(50, 49, 48));		// �������ڰ�ť����ָ����ɫ���ʻ�����
	else if(m_BtnType == 2)
		MemDC.FillSolidRect(0, 0, rct.Width(), rct.Height(), RGB(0, 0, 0));			// ���Ӵ��ڰ�ť����ָ����ɫ���ʻ�����
		
	Graphics graphics(MemDC.m_hDC);

	if (!m_IsLogin && img_ForBid != NULL)
	{
		graphics.DrawImage(img_ForBid, rct.left, rct.top, rct.Width(), rct.Height());
	}
	else
	{
		if (m_IsSelect)
		{
			graphics.DrawImage(img_Select, rct.left, rct.top, rct.Width(), rct.Height());
		}
		else
		{
			if (m_IsOver)
				graphics.DrawImage(img_Over, rct.left, rct.top, rct.Width() + 1, rct.Height());
			else
				graphics.DrawImage(img_Normal, rct.left, rct.top, rct.Width() + 1, rct.Height());
		}
	}

	MemDC.SetBkMode(TRANSPARENT);
	pDC->BitBlt(0, 0, rct.Width(), rct.Height(), &MemDC, 0, 0, SRCCOPY);
	pDC->RestoreDC(nSaveDC);

	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void CPictureButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CMFCButton::PreSubclassWindow();
}

void CPictureButton::SetButtonOver(bool IsOver)
{
	m_IsOver = IsOver;
}

void CPictureButton::SetButtonForBid(bool IsOver)
{
	m_IsLogin = IsOver;
}

void CPictureButton::SetButtonSelect(bool IsSelect)
{
	m_IsSelect = IsSelect;
}

void CPictureButton::SetButtonType(int BtnType)
{
	m_BtnType = BtnType;
}

void CPictureButton::TriggerInvalidate()
{
	Invalidate();
	UpdateWindow();
}

void CPictureButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (img_Over == NULL)
	{
		CMFCButton::OnMouseMove(nFlags, point);
		return;
	}

	TRACKMOUSEEVENT csTME;						// ׷������¼�
	csTME.cbSize = sizeof(csTME);
	csTME.dwFlags = TME_LEAVE | TME_HOVER;
	csTME.hwndTrack = m_hWnd;
	csTME.dwHoverTime = 5;
	::_TrackMouseEvent(&csTME);

	SetButtonOver(true);

	CMFCButton::OnMouseMove(nFlags, point);
}

void CPictureButton::OnMouseLeave()
{
	if (img_Over == NULL)
	{
		CMFCButton::OnMouseLeave();
		return;
	}

	SetButtonOver(false);
	TriggerInvalidate();
	CMFCButton::OnMouseLeave();
}

// ������ͼƬ
void CPictureButton::LoadNormalImage(Image* srcImage)
{
	img_Normal = srcImage;
}

// ��긲����ȥ��ͼƬ
void CPictureButton::LoadOverImage(Image* srcImage)
{
	img_Over = srcImage;
}

// ��������ͼƬ
void CPictureButton::LoadSelectImage(Image* srcImage)
{
	img_Select = srcImage;
}

// ��ֹ��ͼƬ
void CPictureButton::LoadForBidImage(Image* srcImage)
{
	img_ForBid = srcImage;
}
//void CPictureButton::LoadNormalImage(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance)
//{
//	if (img_Normal != NULL)
//	{
//		delete img_Normal;
//		img_Normal = NULL;
//	}
//	img_Normal = LoadGDIImageFromResource(nResID, lpszResType, hInstance);
//}
//
//void CPictureButton::LoadSelectImage(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance)
//{
//	if (img_Select != NULL)
//	{
//		delete img_Select;
//		img_Select = NULL;
//	}
//	img_Select = LoadGDIImageFromResource(nResID, lpszResType, hInstance);
//}
//
//void CPictureButton::LoadOverImage(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance)
//{
//	if (img_Over != NULL)
//	{
//		delete img_Over;
//		img_Over = NULL;
//	}
//	img_Over = LoadGDIImageFromResource(nResID, lpszResType, hInstance);
//}
//
//void CPictureButton::LoadNormalImage(LPCTSTR pszFileName)
//{
//	if (img_Normal != NULL)
//	{
//		delete img_Normal;
//		img_Normal = NULL;
//	}
//	img_Normal = LoadGDIImageFromFile(pszFileName);
//}
//
//void CPictureButton::LoadSelectImage(LPCTSTR pszFileName)
//{
//	if (img_Select != NULL)
//	{
//		delete img_Select;
//		img_Select = NULL;
//	}
//	img_Select = LoadGDIImageFromFile(pszFileName);
//}
//
//void CPictureButton::LoadOverImage(LPCTSTR pszFileName)
//{
//	if (img_Over != NULL)
//	{
//		delete img_Over;
//		img_Over = NULL;
//	}
//	img_Over = LoadGDIImageFromFile(pszFileName);
//}

Image* CPictureButton::LoadGDIImageFromResource(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance)
{
	Image* pImage = NULL;

	HRSRC hPic = FindResource(hInstance/*AfxGetResourceHandle()*/, MAKEINTRESOURCE(nResID), lpszResType);
	HANDLE hResData = NULL;
	if (!hPic || !(hResData = LoadResource(AfxGetResourceHandle(), hPic)))
	{
		::OutputDebugString(_T("Load (resource): Error loading resource: %d\n"));
		return NULL;
	}

	// ��ȡ��Դ���ݵĴ�С����GlobalAllocʹ��
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(), hPic);

	// ������Դ���ݴ�С������HGLOBAL�ڴ�
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwSize);
	if (!hGlobal)
	{
		::OutputDebugString(_T("Load (resource): Error allocating memory\n"));
		FreeResource(hResData);
		return NULL;
	}

	char *pDest = reinterpret_cast<char *> (GlobalLock(hGlobal));
	char *pSrc = reinterpret_cast<char *> (LockResource(hResData)); // ��ס��Դ
	if (!pSrc || !pDest)
	{
		::OutputDebugString(_T("Load (resource): Error locking memory\n"));
		GlobalFree(hGlobal);
		FreeResource(hResData);
		return NULL;
	};

	// ����Դ���ݿ�����HGLOBAL�ڴ��У����ڴ�����
	memcpy(pDest, pSrc, dwSize);
	FreeResource(hResData);
	GlobalUnlock(hGlobal);

	IStream *pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
	{
		return NULL;
	}
	pImage = Image::FromStream(pStream);

	pStream->Release();
	return pImage;
}

Image* CPictureButton::LoadGDIImageFromFile(LPCTSTR pszFileName)
{
	Image* pImage = NULL;
	ASSERT(pszFileName != NULL);

	CFile file;
	DWORD dwSize;

	// ���ļ�
	if (!file.Open(pszFileName,
		CFile::modeRead |
		CFile::shareDenyWrite))
	{
		TRACE(_T("Load (file): Error opening file %s\n"), pszFileName);
		return FALSE;
	};

	// �����ļ����ݴ�С����HGLOBAL�ڴ�  
	dwSize = (DWORD)file.GetLength();
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, dwSize);
	if (!hGlobal)
	{
		TRACE(_T("Load (file): Error allocating memory\n"));
		return FALSE;
	};

	char* pData = reinterpret_cast<char*>(GlobalLock(hGlobal));
	if (!pData)
	{
		TRACE(_T("Load (file): Error locking memory\n"));
		GlobalFree(hGlobal);
		return FALSE;
	};

	// ���ļ����ݶ���HGLOBAL�ڴ���  
	TRY
	{
		file.Read(pData, dwSize);
	}
	CATCH(CFileException, e);
	{
		TRACE(_T("Load (file): An exception occured while reading the file %s\n"),
			pszFileName);
		GlobalFree(hGlobal);
		e->Delete();
		file.Close();
		return FALSE;
	}
	END_CATCH

		GlobalUnlock(hGlobal);
	file.Close();

	// ����hGlobal�ڴ��е����ݴ���stream  
	IStream* pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
	{
		return FALSE;
	}

	// ��ͼƬ�ļ����ݼ��ص�Image������
	pImage = Image::FromStream(pStream);
	ASSERT(pImage != NULL);
	pStream->Release();

	return pImage;
}