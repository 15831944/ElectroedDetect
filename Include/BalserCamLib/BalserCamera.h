#ifndef _BALSERCAMERA_LIB_H_
#define _BALSERCAMERA_LIB_H_

#ifdef BALSERCAMERA_EXPORTS
#define BALSERCAMERA_DLLAPI __declspec(dllexport)
#else
#define BALSERCAMERA_DLLAPI __declspec(dllimport)
#endif

#include "CSIItemStruct.h"
/**********************************************************
��ȡ��������¼��ص�����
EventID,���״̬�ı��¼�ID
ucStatus,�����ǰ״̬����λ����
HGFEDCBA     
A-> 1-�����(ceOpen) 0-����ر�(ceClose)
B-> 1-�ɼ��߳�����(ceStart) 0-�ɼ��̹߳ر�(ceStop)
C-> 1-�ɼ���ͣ(cePause) 0-�ɼ�����
D-> 1-���ڲɼ�(ceGrabbing) 0-δ�ɼ�������(ceIdle)
EFGH������ȫ0
**********************************************************/
typedef struct BALSERCAMERA_DLLAPI tagBLGrabStatus
{
	bool bIsOpen;
	bool bIsStart;
	bool bIsPause;
	bool bIsGrabbing;
}BLGrabStatus;

//��ȡ������ݻص�����
typedef LRESULT(__cdecl *BalserFunOnGetImage)(ImageUnitBase *PtrImageInfo, int CameraID, void* param);

//����ɼ����������
typedef LRESULT (__cdecl *BalserFunOnCameraEvent)(CameraEvent EventID, void* param);

class BALSERCAMERA_DLLAPI CBalserGrabBase
{
public:
	//����ɼ���ʼ��
	CBalserGrabBase();
	virtual ~CBalserGrabBase();

	/**********************************************************
	* �������ͼ�����ݻ�ȡ�Ļص����������ڶ�����������첽ȡ��
	* pDHFunOnGetImage �����ȡͼ��ص�����
	* param            �ص���������
    * ����ֵ TRUE-�ɹ� FALSE-ʧ��
    ************************************************************/
	void SetCallBackBalserGetImageFun(BalserFunOnGetImage PtrBalserFunOnGetImage, void* param);

	/*********************************************************
	* ����������д����¼��ص�����
	* pDHFunOnCameraEvent   ����¼��ص�����
	* param                 �ص���������
	***********************************************************/
	void SetCallBackBalserCameraEventFun(BalserFunOnCameraEvent PtrBalserFunOnCameraEvent,void* param);
	
	/*********************************************************
	* ��ͣ�ɼ�
	* bPause TRUE-��ͣ FALSE-����
    ***********************************************************/
	void BalserPauseGrab(bool bPause);

	/**********************************************************
	* ��ȡ�Ƿ���ͣ�ɼ���־
	* ����ֵ TRUE-��ͣ FALSE-����
	***********************************************************/
	bool IsBalserPauseGrab();

	/**********************************************************
	* ��������Ƿ��������� 
	* ����ֵ TRUE-���� FALSE-δ����
	**********************************************************/
	bool IsBalserConnected();

	bool IsBalserGrabbing();

	/*********************************************************
	* ��ȡͼ�����
	* pImageInfo ͼ�����
    * ����ֵ TRUE-�ɹ� FALSE-ʧ��
	***********************************************************/
	bool GetImageInfo(ImageUnitBase *pImageInfo);

	/*********************************
	* ����ͼ�����
	* pImageInfo ͼ�����
    * ����ֵ TRUE-�ɹ� FALSE-ʧ��
	************************************/
	bool SetImageInfo(const ImageUnitBase *pImageInfo);

public:
	/********************************************
	* ����������� 
    *����ֵ TRUE-�ɹ� FALSE-ʧ��,������ɹ�������û���ú���
	***********************************************/
	virtual bool BalserOpenCamera() = 0;
	
	/**********************************************
	* �ر��������
    *����ֵ TRUE-�ɹ� FALSE-ʧ��,������ɹ�������û���ú���
	************************************************/
	virtual bool BalserCloseCamera() = 0;

	/*********************************************
	* �����ɼ��߳̿�ʼ���������ɼ�
	* sOption �ɼ�ѡ�����CGrabOption����
    *����ֵ TRUE-�ɹ��������ɼ��̣߳�����sOption�����������вɼ� FALSE-ʧ��
	********************************************/
	virtual bool BalserStartGrab() = 0;
	
	/**********************************************
	* ֹͣͼ��ɼ��߳�
	* ����ֵ TRUE-�ɹ� FALSE-ʧ��
	***********************************************/
	virtual bool BalserStopGrab() = 0;
	
	/***************************************************
	* ����ɼ����ݣ�����ͬ���ɼ���һ�βɼ�һ��ͼƬ
	* pBuffer-out�������ڴ�ָ�룬���ڴ�ŷ��ص�����
	* dwBufferSize-inout���ڴ��С����ֹ���,����ʵ�����ݴ�С
	* pImageInfo-out,�ɼ�����ͼ����Ϣ
    * ����ֵ 0-�ɹ�������ֵʧ��, 10-�����������
	***************************************************/
	virtual bool BalserGrabImage() = 0;

	virtual bool BalserGrabLine() = 0;

public:
	//���ݽ��ջص�����
	BalserFunOnGetImage m_PtrBalserFunOnGetImage;
	void*               m_PtrBalserFunOnGetImageParam;
	//��ȡ��������¼��ص�����
	BalserFunOnCameraEvent m_PtrBalserFunOnCameraEvent;
	void*                  m_PtrBalserFunOnCameraEventParam;
	
	//�޸����״̬������ɵ��ô˺����ı�״̬
	void BalserCameraChangeEvent(CameraEvent CamEvent);

	//�������ָ��---����ɼ�����
	CameraGrabPara* m_PtrCameraPara;
	//ͼ����Ϣָ��---ͼ����Ϣ����
	ImageUnitBase*  m_PtrImageUnit;

public:
	//��ͣ�ɼ���־
	bool    m_bBalserPauseGrab;

	//�������������־
	bool    m_bBalserConnected;

	bool    m_bBalserGrabbing;
	bool    m_IsNormCameraStatus;
};


#endif  //_BALSERCAMERA_LIB_H_