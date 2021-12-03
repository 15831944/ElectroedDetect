#include "pch.h"
#include "CColorStatusBar.h"

CColorStatusBar::CColorStatusBar(void)
{
	Color = new COLORREF[8];
	Text = new CString[8];
}

CColorStatusBar::~CColorStatusBar(void)
{
	if (Color != NULL)
	{
		delete[]Color;
	}

	if (Text != NULL)
	{
		delete[]Text;
	}
}

BEGIN_MESSAGE_MAP(CColorStatusBar, CStatusBar)

END_MESSAGE_MAP()

void CColorStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//DRAWITEMSTRUCT Ϊ��Ҫ�Ի�Ŀؼ����߲˵����ṩ�˱�Ҫ����Ϣ
	
	//��ȡID
	UINT unID = lpDrawItemStruct->itemID;
	CDC dc;
	//hDC���ӵ�CDC����
	dc.Attach(lpDrawItemStruct->hDC);
	//�����豸�����Ķ���ı���ģʽΪ
	dc.SetBkMode(TRANSPARENT);
	//lpDrawItemStruct->rcItemָ���˽������Ƶľ�������
	//����һ����������
	CRect rect(&lpDrawItemStruct->rcItem);
	//������ɫ Color[unID]--����ID��ֵ������ɫ
	dc.SetTextColor(Color[unID]);
	//��� Text[unID]����ID��ֵ�����ı�
	dc.TextOut(rect.left + 1, rect.top, Text[unID]);
	//����DC
	dc.Detach();
}


void CColorStatusBar::SetPaneText(int nIndex, LPCTSTR lpszNewText, COLORREF crItemColor)
{
	//�����鸳ֵ
	Text[nIndex] = lpszNewText;
	Color[nIndex] = crItemColor;
	//��һ��״̬���ؼ��ĸ��������������ı� SBT_OWNERDRAW| SBT_NOBORDERS -���ʹ��û��3D���
	GetStatusBarCtrl().SetText("", nIndex, SBT_OWNERDRAW | SBT_NOBORDERS);
}