#ifndef _DALSACAMERA_LIB_H_
#define _DALSACAMERA_LIB_H_

#ifdef DALSACAMERA_EXPORTS
#define DALSACAMERA_DLLAPI __declspec(dllexport)
#else
#define DALSACAMERA_DLLAPI __declspec(dllimport)
#endif

#include "ZcStruct.h"

typedef struct DALSACAMERA_DLLAPI tagDHGrabStatus
{
	bool bIsOpen;
	bool bIsStart;
	bool bIsPause;
	bool bIsGrabbing;
}DHGrabStatus;

//��ȡ������ݻص�����
typedef LRESULT (__cdecl *DalsaFunOnGetImage)(ImageUnitBase *pImageInfo, int cameraID, void* param);

//����ͼƬǰ�Ļص����� ����TRUE�򱣴�ͼƬ�����򲻱���ͼƬ
typedef LRESULT (__cdecl *DalsaFunBeforeSaveImage)(ImageUnitBase *pImageInfo, void* param);

//����ɼ����������
typedef LRESULT (__cdecl *DalsaFunOnCameraEvent)(CameraEvent EventID, void* param);

class DALSACAMERA_DLLAPI CCameraGrabBase
{
public:
	//����ɼ���ʼ��
	CCameraGrabBase();
	virtual ~CCameraGrabBase();

	/**********************************************************
	* �������ͼ�����ݻ�ȡ�Ļص����������ڶ�����������첽ȡ��
	* pDHFunOnGetImage �����ȡͼ��ص�����
	* param            �ص���������
    * ����ֵ TRUE-�ɹ� FALSE-ʧ��
    ************************************************************/
	void SetCallBackGetImageFun(DalsaFunOnGetImage pDalsaFunOnGetImage, void* param);

	/**********************************************************
	* �������ͼ�����ݻ�ȡ�Ļص����������ڶ�����������첽ȡ��
	* pDHFunBeforeSaveImage	����ͼ��ǰ�Ļص�����
	* param					�ص���������
    *����ֵ TRUE-�ɹ� FALSE-ʧ��
    ***********************************************************/
	void SetCallBackBeforeSaveImageFun(DalsaFunBeforeSaveImage pDalsaFunBeforeSaveImage, void* param);

	/*********************************************************
	* ����������д����¼��ص�����
	* pDHFunOnCameraEvent   ����¼��ص�����
	* param                 �ص���������
	***********************************************************/
	void SetCallBackCameraEventFun(DalsaFunOnCameraEvent pDalsaFunOnCameraEvent,void* param);
	
	/*********************************************************
	* ��ͣ�ɼ�
	* bPause TRUE-��ͣ FALSE-����
    ***********************************************************/
	void DalsaPauseGrab(bool bPause);

	/**********************************************************
	* ��ȡ�Ƿ���ͣ�ɼ���־
	* ����ֵ TRUE-��ͣ FALSE-����
	***********************************************************/
	bool IsDalsaPauseGrab();

	/**********************************************************
	* ��������Ƿ��������� 
	* ����ֵ TRUE-���� FALSE-δ����
	**********************************************************/
	bool IsDalsaConnected();

	bool IsDalsaGrabbing();

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
	virtual bool DalsaOpenCamera() = 0;
	
	/**********************************************
	* �ر��������
    *����ֵ TRUE-�ɹ� FALSE-ʧ��,������ɹ�������û���ú���
	************************************************/
	virtual bool DalsaCloseCamera() = 0;

	/*********************************************
	* �����ɼ��߳̿�ʼ���������ɼ�
	* sOption �ɼ�ѡ�����CGrabOption����
    *����ֵ TRUE-�ɹ��������ɼ��̣߳�����sOption�����������вɼ� FALSE-ʧ��
	********************************************/
	virtual bool DalsaStartGrab() = 0;
	
	/**********************************************
	* ֹͣͼ��ɼ��߳�
	* ����ֵ TRUE-�ɹ� FALSE-ʧ��
	***********************************************/
	virtual bool DalsaStopGrab() = 0;
	
	/***************************************************
	* ����ɼ����ݣ�����ͬ���ɼ���һ�βɼ�һ��ͼƬ
	* pBuffer-out�������ڴ�ָ�룬���ڴ�ŷ��ص�����
	* dwBufferSize-inout���ڴ��С����ֹ���,����ʵ�����ݴ�С
	* pImageInfo-out,�ɼ�����ͼ����Ϣ
    * ����ֵ 0-�ɹ�������ֵʧ��, 10-�����������
	***************************************************/
	virtual bool DalsaGrabImage() = 0;

public:
	//���ݽ��ջص�����
	DalsaFunOnGetImage m_pDalsaFunOnGetImage;
	void*              m_pDalsaFunOnGetImageParam;
	//ͼƬ����ǰ�ص�����
	DalsaFunBeforeSaveImage m_pDalsaFunBeforeSaveImage;
	void*                   m_pDalsaFunBeforeSaveImageParam;
	//��ȡ��������¼��ص�����
	DalsaFunOnCameraEvent m_pDalsaFunOnCameraEvent;
	void*                 m_pDalsaFunOnCameraEventParam;
	
	//�޸����״̬������ɵ��ô˺����ı�״̬
	void DalsaCameraChangeEvent(CameraEvent CamEvent);

	//�������ָ��---����ɼ�����
	CameraGrabPara* m_pCameraPara;
	//ͼ����Ϣָ��---ͼ����Ϣ����
	ImageUnitBase*  m_pImageUnit;

public:
	//��ͣ�ɼ���־
	bool    m_bDalsaPauseGrab;

	//�������������־
	bool    m_bDalsaConnected;

	bool    m_bDalsaGrabbing;
};


#endif  //_DALSACAMERA_LIB_H_