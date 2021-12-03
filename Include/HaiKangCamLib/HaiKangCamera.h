#ifndef _HAIKANGCAMERA_LIB_H_
#define _HAIKANGCAMERA_LIB_H_

#ifdef HAIKANGCAMERA_EXPORTS
#define HAIKANGCAMERA_DLLAPI __declspec(dllexport)
#else
#define HAIKANGCAMERA_DLLAPI __declspec(dllimport)
#endif

#include "CameraPara.h"

#include <string>
#include <malloc.h>
#include <string.h>
#include <vector>
#include <atltime.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>
#include <iostream>

//#include "Public.h"

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
typedef struct HAIKANGCAMERA_DLLAPI tagHKGrabStatus
{
	bool bIsOpen;
	bool bIsStart;
	bool bIsPause;
	bool bIsGrabbing;
}HKGrabStatus;

//��ȡ������ݻص�����
typedef LRESULT(__cdecl *HaiKangFunOnGetImage)(ImageUnitBase *PtrImageInfo, int CameraID, void* param);

//����ɼ����������
typedef LRESULT(__cdecl *HaiKangFunOnCameraEvent)(CameraEvent EventID, void* param);

class HAIKANGCAMERA_DLLAPI CHaiKangGrabBase
{
public:
	//����ɼ���ʼ��
	CHaiKangGrabBase();
	virtual ~CHaiKangGrabBase();

	/**********************************************************
	* �������ͼ�����ݻ�ȡ�Ļص����������ڶ�����������첽ȡ��
	* pDHFunOnGetImage �����ȡͼ��ص�����
	* param            �ص���������
    * ����ֵ TRUE-�ɹ� FALSE-ʧ��
    ************************************************************/
	void SetCallBackHaiKangGetImageFun(HaiKangFunOnGetImage PtrHaiKangFunOnGetImage, void* param);

	/*********************************************************
	* ����������д����¼��ص�����
	* pDHFunOnCameraEvent   ����¼��ص�����
	* param                 �ص���������
	***********************************************************/
	void SetCallBackHaiKangCameraEventFun(HaiKangFunOnCameraEvent PtrHaiKangFunOnCameraEvent, void* param);
	
	/*********************************************************
	* ��ͣ�ɼ�
	* bPause TRUE-��ͣ FALSE-����
    ***********************************************************/
	void HaiKangPauseGrab(bool bPause);

	/**********************************************************
	* ��ȡ�Ƿ���ͣ�ɼ���־
	* ����ֵ TRUE-��ͣ FALSE-����
	***********************************************************/
	bool IsHaiKangPauseGrab();

	/**********************************************************
	* ��������Ƿ��������� 
	* ����ֵ TRUE-���� FALSE-δ����
	**********************************************************/
	bool IsHaiKangConnected();

	bool IsHaiKangGrabbing();

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
	virtual bool HaiKangOpenCamera() = 0;
	
	/**********************************************
	* �ر��������
    *����ֵ TRUE-�ɹ� FALSE-ʧ��,������ɹ�������û���ú���
	************************************************/
	virtual bool HaiKangCloseCamera() = 0;

	/*********************************************
	* �����ɼ��߳̿�ʼ���������ɼ�
	* sOption �ɼ�ѡ�����CGrabOption����
    *����ֵ TRUE-�ɹ��������ɼ��̣߳�����sOption�����������вɼ� FALSE-ʧ��
	********************************************/
	virtual bool HaiKangStartGrab() = 0;
	
	/**********************************************
	* ֹͣͼ��ɼ��߳�
	* ����ֵ TRUE-�ɹ� FALSE-ʧ��
	***********************************************/
	virtual bool HaiKangStopGrab() = 0;
	
	/***************************************************
	* ����ɼ����ݣ�����ͬ���ɼ���һ�βɼ�һ��ͼƬ
	* pBuffer-out�������ڴ�ָ�룬���ڴ�ŷ��ص�����
	* dwBufferSize-inout���ڴ��С����ֹ���,����ʵ�����ݴ�С
	* pImageInfo-out,�ɼ�����ͼ����Ϣ
    * ����ֵ 0-�ɹ�������ֵʧ��, 10-�����������
	***************************************************/
	virtual bool HaiKangGrabImage() = 0;

	virtual bool HaiKangGrabLine() = 0;

public:
	//���ݽ��ջص�����
	HaiKangFunOnGetImage m_PtrHaiKangFunOnGetImage;
	void*                m_PtrHaiKangFunOnGetImageParam;
	//��ȡ��������¼��ص�����
	HaiKangFunOnCameraEvent m_PtrHaiKangFunOnCameraEvent;
	void*                   m_PtrHaiKangFunOnCameraEventParam;
	
	//�޸����״̬������ɵ��ô˺����ı�״̬
	void HaiKangCameraChangeEvent(CameraEvent CamEvent);

	//�������ָ��---����ɼ�����
	CameraGrabPara* m_PtrCameraPara;
	//ͼ����Ϣָ��---ͼ����Ϣ����
	ImageUnitBase*  m_PtrImageUnit;

public:
	//��ͣ�ɼ���־
	bool    m_bHaiKangPauseGrab;

	//�������������־
	bool    m_bHaiKangConnected;

	bool    m_bHaiKangGrabbing;

	bool    m_IsNormCameraStatus;
};


#endif  //_HAIKANGCAMERA_LIB_H_