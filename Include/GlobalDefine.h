//�궨���ļ�---���幫�õĺ�
#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

#include "afxmt.h"
#include "Public.h"
#include "CameraPara.h"

#define COLOR_BACKGROUND_TITLE RGB(10,10,10)
#define COLOR_TEXT_TITLE RGB(14,230,255)
#define COLOR_TEXT_WHITE RGB(255,255,255)
#define COLOR_TEXT_GREEN RGB(66,200,90)

#define COLOR_TEXT_OK RGB(40,235,75)
#define COLOR_TEXT_NG RGB(250,70,70)
#define COLOR_TEXT_BLACK RGB(10,10,10)

#define COLOR_BACKGROUND_OK RGB(37,57,41)
#define COLOR_BACKGROUND_NG RGB(58,39,39)

#define COLOR_BACKGROUND_SET RGB(240,240,240)
#define COLOR_BACKGROUND_SET_MENU RGB(200,200,200)
#define COLOR_DETBACKGROUND_SET_MENU RGB(50,49,48)
#define COLOR_DETBACKGROUND RGB(0,0,0)

#define COLOR_BACKGROUND_SUB_REGION RGB(30,30,30)
#define COLOR_BORDER_SUB_REGION RGB(60,60,60)

#define COLOR_BACKGROUND_SUB_REGION_2 RGB(37,37,37)
#define COLOR_BACKGROUND_MAIN RGB(42,42,42)

#define COLOR_BUTTON_OVER_QUIT RGB(232,17,35)
#define COLOR_BUTTON_CLICK_QUIT RGB(243,112,120)

#define COLOR_BUTTON_OVER RGB(170,170,170)
#define COLOR_SETBUTTON_OVER RGB(142,169,219)
#define COLOR_BUTTON_CLICK RGB(140,140,140)
#define COLOR_SETBUTTON_CLICK RGB(68,114,196)

// �����ں��Ӵ����а�ť����ɫ
#define COLOR_BUTTON_PARENT RGB(50,49,48)
#define COLOR_BUTTON_CHILD RGB(0,0,0)

//�������е�ͳ����Ϣ
typedef struct tagStatisticElement
{
	UINT dwSumCount;         //��õ��ź�����
	UINT dwUsedTimeSum;      //��õĺ�ʱ����

	UINT dwMinTime;          //������Сʱ��
	UINT dwMaxTime;

	UINT nMinLimitTime;      //ͳ�Ƶ���Сʱ������ʱ�����
	UINT nMaxLimitTime;
	UINT nMinLimitCount;
	UINT nMaxLimitCount;

	UINT dwStart;

	CCriticalSection cs;

	char m_szMsg[MAX_FILE];

	void Init()
	{
		dwSumCount = 0; dwUsedTimeSum = 0;
		dwMinTime = 100000;
		dwMaxTime = 0;  dwStart = 0;

		nMinLimitTime = 30; nMaxLimitCount = 100;
		nMinLimitCount = nMaxLimitCount = 0;

		memset(m_szMsg, '\0', MAX_FILE);
	}

	void Start()
	{
		dwStart = GetTickCount64();
	}

	void Stop()
	{
		UINT dwTime;

		cs.Lock();

		dwTime = GetTickCount64() - dwStart;

		dwSumCount++;
		dwUsedTimeSum += dwTime;

		if(dwTime < nMinLimitTime)
			nMinLimitCount++;

		if(dwTime > nMaxLimitTime)
			nMaxLimitCount++;

		if(dwTime > dwMaxTime)
			dwMaxTime = dwTime;

		if(dwTime < dwMinTime)
			dwMinTime = dwTime;

		cs.Unlock();		
	}
}StatisticElement;

//typedef enum RunTimeEnum {seAnalyse1, seAnalyse2,seAnalyse3,seAnalyse4,seAnalyse5, seDisplay, seIOControl};
//
//#define  RUNTIMESTATUS    9
//���м�������ͳ����Ϣʵʱ��ʾ
typedef struct tagRunTimeElement
{
	//ͳ�Ƹ�������Ĳɼ�ʱ��
	StatisticElement GrabElement[2];
	//ͳ�Ƹ�������ķ���ʱ��
	StatisticElement AnalyElement[2];
	//ͳ�Ƹ����������ʾʱ��
	StatisticElement DisplayElement[2];
	StatisticElement SaveElement[2];
	StatisticElement ModbusElement;
	//ͳ�Ƹ��������IO�ź�ʱ��
	StatisticElement ControlElement;

	StatisticElement LocalClientElement;

	StatisticElement FileHanderElement;
	//ͳ�����еĴ�����Ϣ
	int dwRGBLostCount[CAMERANUM];	   //RGBת����֡��
	int dwAnalyLostCount[CAMERANUM];   //�������ж�֡��	
	int dwSaveLostCount[CAMERANUM];    //����ͼƬ��֡��

	int dwAnalyGood[CAMERANUM];       //����������Ŀ
	int dwAnalyseNG[CAMERANUM];       //������Ʒ��Ŀ

	int dwLaterAnalyCount[CAMERANUM]; //��������ʱ��Ŀ

	int dwExceptionCount[CAMERANUM];  //�쳣ģʽ�������쳣����

	void Init()
	{
		AnalyElement[0].Init();    AnalyElement[1].Init();

		GrabElement[0].Init();     GrabElement[2].Init();

		DisplayElement[0].Init();  DisplayElement[2].Init();

		SaveElement[0].Init();     SaveElement[2].Init();

		ModbusElement.Init(); FileHanderElement.Init();

		ControlElement.Init();

		for (int hIndex = 0; hIndex < CAMERANUM; hIndex++)
		{
			dwRGBLostCount[hIndex] = 0;
			dwAnalyLostCount[hIndex] = 0;
			dwSaveLostCount[hIndex] = 0;

			dwAnalyGood[hIndex] = dwAnalyseNG[hIndex] = 0;

			dwLaterAnalyCount[hIndex] = 0;

			dwExceptionCount[hIndex] = 0;
		}
	}
}RunTimeElement;


#endif  //_GLOBAL_DEFINE_H_