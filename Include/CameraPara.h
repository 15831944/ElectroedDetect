//�궨���ļ�---���幫�õĺ�
#ifndef _CAMERA_PARA_H_
#define _CAMERA_PARA_H_

#include <string.h>

typedef unsigned int        UINT;
typedef unsigned long       DWORD;
typedef wchar_t             WCHAR;
typedef unsigned char       BYTE;

#define FILEDATAMAXNUM 500
#define CAMERANUM   2

//���Ӳ����������Ϣ
//���Ӳ����������Ϣ
typedef struct tagCameraGrabPara
{
	//���ID��������
	int m_iCamID;
	//�������
	char m_szCamName[128];

	char m_szSerialNum[128];

	UINT m_iPixelFormat;       //ͼ���ʽ	
	UINT m_bReverseX;          //ͼ���Ƿ�ˮƽ��ת
	UINT m_bReverseY;          //ͼ���Ƿ�ֱ��ת

	UINT m_uImageWidth;        //ͼ����
	UINT m_uLineHeight;        //�����ɨ���
	UINT m_uImageHeight;       //ͼ��߶�
	UINT m_uImageChannel;      //ͼ��ͨ��
	UINT m_uXOffset;           //ͼ��x����ƫ��
	UINT m_uYOffset;           //ͼ��y����ƫ��	

	UINT m_cBWhite;            //0 ---��ƽ��ر� 
							   //1 ---��ƽ������ʱ����һ�� 
							   //2 ---�û��Զ����ƽ��ֵ
	float m_uBRatioAbsR;       //��ƽ��ֵ R ʵ��ֵ���ô�ֵ����100	
	float m_uBRatioAbsG;       //��ƽ��ֵ G ʵ��ֵ���ô�ֵ����100
	float m_uBRatioAbsB;       //��ƽ��ֵ B ʵ��ֵ���ô�ֵ����100

	UINT m_uBRatioRawR;
	UINT m_uBRatioRawG;
	UINT m_uBRatioRawB;

	UINT m_iTriggerMode;       //����ģʽ 0---�ر�        1---��	
	UINT m_cTriggerSource;     //����Դ   0---line1����� 1---����	
	UINT m_cTriggerActivation; //�������� 0---�����ش���  1---�½��ش���

	//UINT m_uTriggerDelay;      //�ӳٴ��� ʱ�� ����ֵ ΢�� Abs(us)	
	//BYTE m_cExposureTimeMode;  //�ع�ʱ��ģʽ 0---�ڲ���ʱ�� 
	//                           //             1---�ⴥ��ʱ����	
	//BYTE m_cExposureAuto;      //�Զ��ع�ģʽ 0�رգ�1�Զ��ع�	

	UINT m_uExposureTime;      //�ع�ʱ�� Abs(us)����ֵ΢��

	float m_iFrameRate;

	double m_uGainTime;

	tagCameraGrabPara()             //���������Ϣ�ĳ�ֵ
	{
		m_iCamID = 0;

		memset(m_szCamName, '\0', 128);
		memset(m_szSerialNum, '\0', 128);

		m_iPixelFormat = 1;       //ͼ���ʽ	
		m_bReverseX = 0;          //ͼ���Ƿ�ˮƽ��ת
		m_bReverseY = 0;

		m_uImageWidth = 0;        //ͼ����	
		m_uImageHeight = 0;       //ͼ��߶�
		m_uImageChannel = 1;      //ͼ��ͨ��
		m_uXOffset = 0;           //ͼ��x����ƫ��
		m_uYOffset = 0;           //ͼ��y����ƫ��	
		//m_uImageSize = 0;         //ͼ�񳤶�

		//m_uGain = 0;              //ͼ������

		m_cBWhite = 2;
		m_uBRatioAbsR = 0;           //��ƽ��ֵ R ʵ��ֵ���ô�ֵ����100	
		m_uBRatioAbsG = 0;           //��ƽ��ֵ G ʵ��ֵ���ô�ֵ����100
		m_uBRatioAbsB = 0;           //��ƽ��ֵ B ʵ��ֵ���ô�ֵ����100

		m_uBRatioRawR = 0;
		m_uBRatioRawG = 0;
		m_uBRatioRawB = 0;

		m_iTriggerMode = 0;         //����ģʽ 0---�ر�        1---��	
		m_cTriggerSource = 0;       //����Դ   0---line1����� 1---����	
		m_cTriggerActivation = 0;   //�������� 0---�����ش���  1---�½��ش���

		//m_uTriggerDelay = 0;      //�ӳٴ��� ʱ�� ����ֵ ΢�� Abs(us)	
		//m_cExposureTimeMode = 0;  //�ع�ʱ��ģʽ 0---�ڲ���ʱ�� 
		//                          //             1---�ⴥ��ʱ����	
		//m_cExposureAuto = 0;      //�Զ��ع�ģʽ 0�رգ�1�Զ��ع�	
		m_uExposureTime = 100;    //�ع�ʱ�� Abs(us)����ֵ΢��
		m_uGainTime = 0;

		m_iFrameRate = 0;
	};
}CameraGrabPara;

typedef struct tagImageUnitBase
{
	//���
	UINT m_dwWidth;
	//�߶�
	UINT m_dwHeight;
	//ͼ�񳤶�
	UINT m_dwImageDataLen;
	//ͼ���ʽ
	UINT   m_dwImageFormat;
	//ͼ������ָ��
	unsigned char* m_dwImageData;

	void Init()
	{
		m_dwHeight = m_dwWidth = 0;
		m_dwImageFormat = 0;
		m_dwImageDataLen = 0;
		m_dwImageData = NULL;
	}
}ImageUnitBase;

enum CameraEvent {ceOpen = 0, ceClose, ceStart, ceStop, ceGrabbing, ceIdle, cePause, ceNoPause};

#endif//_CAMERA_PARA_H_