#pragma once
#include "afxbutton.h"
class CPictureButton :
	public CMFCButton
{
private:
	Image*  img_Select;
	Image* img_Normal;
	Image* img_Test;			// ����
	Image* img_Over;
	Image* img_ForBid;			// ��ֹ

	CBrush    m_FrameBrush;
	CBrush    m_CommonBrush;

	/* �Ƿ��¼ */
	bool m_IsLogin;
	bool m_IsOver;		//  �ƶ����Ϸ�
	bool m_IsSelect;	//  ѡ��״̬

	int m_BtnType;	// �Ƿ��Ӵ��ڵİ�ť������ǣ�����ť��ɫ�͸����ڰ�ť��ɫ����

	Image* LoadGDIImageFromResource(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance);
	Image* LoadGDIImageFromFile(LPCTSTR pszFileName);
public:
	CPictureButton();
	~CPictureButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void PreSubclassWindow();
	void TriggerInvalidate();
	void SetButtonOver(bool IsOver);
	void SetButtonSelect(bool IsSelect);
	void SetButtonForBid(bool IsOver);
	void SetButtonType(int BtnType);
	//void LoadNormalImage(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance);
	//void LoadOverImage(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance);
	//void LoadSelectImage(UINT nResID, LPCTSTR lpszResType, HINSTANCE hInstance);
	//void LoadNormalImage(LPCTSTR pszFileName);
	//void LoadOverImage(LPCTSTR pszFileName);
	//void LoadSelectImage(LPCTSTR pszFileName);
	void LoadNormalImage(Image* srcImage);
	void LoadOverImage(Image* srcImage);
	void LoadSelectImage(Image* srcImage);
	void LoadForBidImage(Image* srcImage);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};

