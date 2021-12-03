#ifndef _BALSERCAMLIB_LIB_H_
#define _BALSERCAMLIB_LIB_H_

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>

#include <BalserCamLib/BalserCamera.h>

typedef Pylon::CBaslerGigECamera Camera_t;

#define CAM_BALSER_BUF_NUM    32

//���֮������ͬ��
struct BALSERCAMERA_DLLAPI BalserSyncSignal
{
	BalserSyncSignal()
	{
		//���캯���д����¼�����
		hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

		bStart = false;   bSetEvent = false;
	}

	~BalserSyncSignal()
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

class BALSERCAMERA_DLLAPI CBalserCameraGrab : public CBalserGrabBase
{
public:
	CBalserCameraGrab();
	virtual ~CBalserCameraGrab();

	virtual bool BalserOpenCamera();
	virtual bool BalserCloseCamera();

	//ԭʼ�߼�
	virtual bool BalserStartGrab();
	virtual bool BalserStopGrab();

	virtual bool BalserGrabImage();

	virtual bool BalserGrabLine();

public:
	bool IsBalserGigeConnect();

	//���ô�����ʽ
	bool setLineTriggerConf();
	//�����ع�ʱ��
	bool setExposureTimeConf();
	//���òɼ���ת
	bool setReverseConf();
	//����ͼ��AOI����
	bool setImageAOIConf();

	//���òɼ����ݵ�ͼ���ʽ
	bool setPixelFormatConf();

	//��������Ĳɼ�֡��
	bool setAcquistionFrameRate();

	//��������İ�ƽ��
	bool setPixelBalanceConf();

	//���������ź�
	//bool m_IsSoftTrigger;
	bool setTriggerSoftware();

	bool getCameraSerialNumber(char* SerialNumber);

	bool GetCameraNormStatus();

public:
	BalserSyncSignal m_SyncSignal;//��ͨ��ͬ���ź�
	BalserSyncSignal m_TriggerSyncSignal;

	//ͳ����Ϣ
	DWORD m_nTriggerCount;    //�����ź�����
	DWORD m_nFailedImageNum;  //�ɼ�ʧ������
	DWORD m_nGrabImageNum;    //�ɼ��ɹ�����

public:     //Dalsa�����ȫ�ֲɼ��豸
	void *m_PtrBalserTLayer;
	void *m_PtrBalserCamera;

	Camera_t::StreamGrabber_t *m_PtrBalserStream;
	BYTE* m_PtrImageData[CAM_BALSER_BUF_NUM];
};

#endif  //_BALSERCAMLIB_LIB_H_