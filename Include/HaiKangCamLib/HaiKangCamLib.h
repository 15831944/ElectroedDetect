#ifndef _HAIKANGCAMLIB_LIB_H_
#define _HAIKANGCAMLIB_LIB_H_

#include <HaiKangCamLib/HaiKangCamera.h>

//typedef Pylon::CBaslerGigECamera Camera_t;

#define CAM_HAIKANG_BUF_NUM    32

//���֮������ͬ��
struct HAIKANGCAMERA_DLLAPI HaiKangSyncSignal
{
	HaiKangSyncSignal()
	{
		//���캯���д����¼�����
		hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

		bStart = false;   bSetEvent = false;
	}

	~HaiKangSyncSignal()
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

class HAIKANGCAMERA_DLLAPI CHaiKangCameraGrab : public CHaiKangGrabBase
{
public:
	CHaiKangCameraGrab();
	virtual ~CHaiKangCameraGrab();

	virtual bool HaiKangOpenCamera();
	virtual bool HaiKangCloseCamera();

	//ԭʼ�߼�
	virtual bool HaiKangStartGrab();
	virtual bool HaiKangStopGrab();

	virtual bool HaiKangGrabImage();

	virtual bool HaiKangGrabLine();

public:
	//���ô�����ʽ
	bool SetLineTriggerConf();
	//�����ع�ʱ��
	bool SetExposureTimeConf();
	bool SetGainConf();

	bool IncreaseExpourseTime();
	bool SetFrameRate();
	//���òɼ���ת
	bool SetReverseConf();
	//����ͼ��AOI����
	bool SetImageAOIConf();

	//���òɼ����ݵ�ͼ���ʽ
	bool SetPixelFormatConf();

	//���������ź�
	//bool m_IsSoftTrigger;
	bool SetTriggerSoftware();
	bool GetExposureTimeConf();

	bool GetCameraSerialNumber(char* SerialNumber);

	bool GetCameraNormStatus();

	int GetCameraImg();			// ��������
public:
	HaiKangSyncSignal m_SyncSignal;//��ͨ��ͬ���ź�
	HaiKangSyncSignal m_TriggerSyncSignal;

	//ͳ����Ϣ
	DWORD m_nTriggerCount;    //�����ź�����
	DWORD m_nFailedImageNum;  //�ɼ�ʧ������
	DWORD m_nGrabImageNum;    //�ɼ��ɹ�����

public:     //Dalsa�����ȫ�ֲɼ��豸
	//void *m_PtrHaiKangTLayer;
	//void *m_PtrHaiKangCamera;

	void* m_PtrDeviceHandle;   //������

	//Camera_t::StreamGrabber_t *m_PtrBalserStream;
	//BYTE* m_PtrImageData[CAM_HAIKANG_BUF_NUM];
};

#endif  //_HAIKANGCAMLIB_LIB_H_