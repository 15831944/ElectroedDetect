#ifndef _DALSACAMLIB_LIB_H_
#define _DALSACAMLIB_LIB_H_

#include <DalsaCamLib/DalsaCamera.h>

#include "SapClassBasic.h"

#define CAM_BUF_NUM    12

//���֮������ͬ��
struct DALSACAMERA_DLLAPI CameraSyncSignal
{
	CameraSyncSignal()
	{
		//���캯���д����¼�����
		hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

		bStart = false;   bSetEvent = false;
	}

	~CameraSyncSignal()
	{
		//�ر��ź���
		CloseHandle(hEvent);

		bStart = FALSE;
	}

	//ͬ����ɣ���ʼ�ɼ�
	void Start()
	{
		//��ָ���¼������״̬����Ϊ�����ź�״̬
		ResetEvent(hEvent);

		bStart=true;
		bSetEvent=true;
	}
	//ֹͣ�ɼ�ʱ����
	void Stop()
	{
		bStart=false;
		bSetEvent=false;
	}

	//��ʼ����ɣ���������ź�
	BOOL Singnal()
	{
		//��ָ���¼������״̬����Ϊ���ź�״̬
		return SetEvent(hEvent);
	}

	bool IsSetEvent()
	{
		return bSetEvent;
	}

	HANDLE hEvent;
	bool   bStart;

private:
	bool bSetEvent;//�������ζ�δ����ź�
};


class DALSACAMERA_DLLAPI CDalseCameraGrab : public CCameraGrabBase
{
public:
	CDalseCameraGrab();
	virtual ~CDalseCameraGrab();

	void InitImageBuffer();

	virtual bool DalsaOpenCamera();
	virtual bool DalsaCloseCamera();

	//ԭʼ�߼�
	virtual bool DalsaStartGrab();
	virtual bool DalsaStopGrab();

	virtual bool DalsaGrabImage();

public:
	//���ô�����ʽ
	bool setLineTriggerConf();
	//�����ع�ʱ��
	bool setExposureTimeConf();
	//���òɼ���ת
	bool setReverseXConf();

	//����ͼ��Ҷ��Ƿ�ת
	bool setNegativeImageConf();
	//����ͼ��AOI����
	bool setImageAOIConf();

	//���òɼ����ݵ�ͼ���ʽ
	bool setPixelFormatConf();

	//��������Ĳɼ�֡��
	bool setAcquistionFrameRate();

	//��������İ�ƽ��
	//bool setPixelBalanceConf();

	bool setTriggerSoftware();

	bool getCameraSerialNumber(char* SerialNumber);

public:
	CameraSyncSignal m_SyncSignal;//��ͨ��ͬ���ź�
	CameraSyncSignal m_TriggerSyncSignal;

	//ͳ����Ϣ
	DWORD m_nTriggerCount;  //�����ź�����
	DWORD m_nFailedImageNum;//�ɼ�ʧ������
	DWORD m_nGrabImageNum;  //�ɼ��ɹ�����

	DWORD m_nGrabLineImageNum;//�ɼ���ɨͼ����Ŀ

	int   m_nCurrLineIndex;   //��ǰ�������

public:     //Dalsa�����ȫ�ֲɼ��豸
	SapLocation*     m_SapLocation;
	SapAcqDevice*    m_SapAcqDevice;
	SapBuffer*       m_SapBuffers;
	SapTransfer*     m_SapTransfer;
};

#endif  //_DAHUACAMLIB_LIB_H_