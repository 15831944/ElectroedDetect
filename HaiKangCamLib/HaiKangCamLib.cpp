#include <LogFile/LogFile.h>

#include "CameraParams.h"
#include "MvCameraControl.h"
#include <HaiKangCamLib/HaiKangCamLib.h>

//using namespace GenApi;
// ch:����Դ | en:
#define SOFTWAREMODE            7
#define HAREWAREMODE            0

// ch:����ģʽ | en:
#define TRIGGER_OFF             0
#define TRIGGER_ON              1
#define TRIGGER_NOT_SET         -1

CLogFile  m_LogFile;

CHaiKangCameraGrab::CHaiKangCameraGrab()
{
	m_nTriggerCount = 0;
	m_nGrabImageNum = 0;
	m_nFailedImageNum = 0;

	m_IsNormCameraStatus = false;
}

CHaiKangCameraGrab::~CHaiKangCameraGrab()
{
	if (m_PtrCameraPara->m_uLineHeight > 0)   //LineHeight  ����0   ��ʾ�����ɨ���
	{
		free(m_PtrImageUnit->m_dwImageData);
		m_PtrImageUnit->m_dwImageData = NULL;
	}
}

bool CHaiKangCameraGrab::SetLineTriggerConf()
{
	//�����źŴ���Դ  0Ϊ�ⲿ����  1Ϊ����
	

	if (m_PtrCameraPara->m_iTriggerMode == 0)
	{
		MV_CC_SetTriggerMode(m_PtrDeviceHandle, TRIGGER_OFF);
		//MV_CC_SetTriggerSource(m_PtrDeviceHandle, HAREWAREMODE);
	}
	else
	{
		MV_CC_SetTriggerMode(m_PtrDeviceHandle, TRIGGER_ON);
		MV_CC_SetTriggerSource(m_PtrDeviceHandle, SOFTWAREMODE);
	}

	return true;
}

//���òɼ�֡��
bool CHaiKangCameraGrab::SetFrameRate()
{
	MV_CC_SetFrameRate(m_PtrDeviceHandle, m_PtrCameraPara->m_iFrameRate);
	return true;
}

//�����ع�ʱ��
bool CHaiKangCameraGrab::SetExposureTimeConf()
{
	MVCC_INTVALUE ExposureTimeInt;
	MV_CC_GetAutoExposureTimeLower(m_PtrDeviceHandle, &ExposureTimeInt);
	MV_CC_GetAutoExposureTimeUpper(m_PtrDeviceHandle, &ExposureTimeInt);

	//�ع�ʱ��
	if (m_PtrCameraPara->m_uExposureTime < ExposureTimeInt.nMax)
		m_PtrCameraPara->m_uExposureTime = m_PtrCameraPara->m_uExposureTime;
	else
		m_PtrCameraPara->m_uExposureTime = ExposureTimeInt.nMax;

	MV_CC_SetExposureAutoMode(m_PtrDeviceHandle, 0);
	MV_CC_SetExposureTime(m_PtrDeviceHandle, m_PtrCameraPara->m_uExposureTime);

	return true;
}

bool CHaiKangCameraGrab::SetGainConf()
{
	MV_CC_SetGainMode(m_PtrDeviceHandle, 0);
	MV_CC_SetGain(m_PtrDeviceHandle, m_PtrCameraPara->m_uGainTime);

	return true;
}

bool CHaiKangCameraGrab::SetReverseConf()
{
	//д����ط�ת����
	if (m_PtrCameraPara->m_bReverseX == 0)
		MV_CC_SetBoolValue(m_PtrDeviceHandle, "ReverseX", false);
	else
		MV_CC_SetBoolValue(m_PtrDeviceHandle, "ReverseX" , true);

	if (m_PtrCameraPara->m_bReverseY == 0)
		MV_CC_SetBoolValue(m_PtrDeviceHandle, "ReverseY", false);
	else
		MV_CC_SetBoolValue(m_PtrDeviceHandle, "ReverseY", true);

	return true;
}

bool CHaiKangCameraGrab::SetImageAOIConf()
{
	if (m_PtrCameraPara->m_uLineHeight > 0)
	{
		MV_CC_SetWidth(m_PtrDeviceHandle, m_PtrCameraPara->m_uImageWidth);
		MV_CC_SetHeight(m_PtrDeviceHandle, m_PtrCameraPara->m_uLineHeight);
	}
	else
	{
		MV_CC_SetWidth(m_PtrDeviceHandle, m_PtrCameraPara->m_uImageWidth);
		MV_CC_SetHeight(m_PtrDeviceHandle, m_PtrCameraPara->m_uImageHeight);

		MV_CC_SetAOIoffsetX(m_PtrDeviceHandle, m_PtrCameraPara->m_uXOffset);
		MV_CC_SetAOIoffsetY(m_PtrDeviceHandle, m_PtrCameraPara->m_uYOffset);
	}
	return true;
}

//bool CHaiKangCameraGrab::GetExposureTimeConf()
//{
//	MVCC_FLOATVALUE ExposureTime;
//	int ret = MV_CC_GetExposureTime(m_PtrDeviceHandle, &ExposureTime);
//
//	if (ret == MV_OK)
//		return true;
//	else
//		return false;
//}

bool CHaiKangCameraGrab::SetPixelFormatConf()
{
	if (m_PtrCameraPara->m_uImageChannel == 1)
	{
		MV_CC_SetPixelFormat(m_PtrDeviceHandle, PixelType_Gvsp_Mono8);
	}
	else
	{
		switch (m_PtrCameraPara->m_iPixelFormat)
		{
		case 1:
			MV_CC_SetPixelFormat(m_PtrDeviceHandle, PixelType_Gvsp_Mono8);
			break;
		case 2:
			MV_CC_SetPixelFormat(m_PtrDeviceHandle, PixelType_Gvsp_BayerRG8);
			break;
		case 3:
			MV_CC_SetPixelFormat(m_PtrDeviceHandle, PixelType_Gvsp_YUV422_Packed);
			break;
		}
	}

	return true;
}

// ����ɼ���ͼƬ��ὫͼƬ��Ϣ�����˺���
void __stdcall ImageCallBackEx(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUserParam)
{
	CHaiKangCameraGrab* PtrGrabCamera = (CHaiKangCameraGrab*)pUserParam;

	//��ʾͼ���ȡ������֡
	if (pFrameInfo == NULL)
	{
		PtrGrabCamera->m_IsNormCameraStatus = false;
		return;
	}

	if (PtrGrabCamera == NULL || PtrGrabCamera->m_bHaiKangGrabbing == false)
	{
		PtrGrabCamera->m_IsNormCameraStatus = false;
		return;
	}

	MV_ALL_MATCH_INFO struMatchInfo = { 0 };

	MV_MATCH_INFO_NET_DETECT stMatchInfoNetDetect;
	struMatchInfo.pInfo = &stMatchInfoNetDetect;

	struMatchInfo.nType = MV_MATCH_TYPE_NET_DETECT; // ch:���������Ͷ�����Ϣ | en:Net flow and lsot packet information
	memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_NET_DETECT));
	struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_NET_DETECT);

	int nRet = MV_CC_GetAllMatchInfo(PtrGrabCamera->m_PtrDeviceHandle, &struMatchInfo);

	if (nRet == MV_OK)
		PtrGrabCamera->m_nFailedImageNum = stMatchInfoNetDetect.nLostFrameCount;

	PtrGrabCamera->m_nGrabImageNum++;
	PtrGrabCamera->m_nTriggerCount = PtrGrabCamera->m_nFailedImageNum + PtrGrabCamera->m_nGrabImageNum;

	int iPixelSize = 1, dwImageLen = 0;

	/*if (PtrGrabCamera->m_PtrCameraPara->m_uImageChannel == 3)
	iPixelSize = 2;
	else
	iPixelSize = 1;*/

	PtrGrabCamera->m_PtrImageUnit->m_dwHeight = pFrameInfo->nHeight;
	PtrGrabCamera->m_PtrImageUnit->m_dwWidth = pFrameInfo->nWidth;

	if (PtrGrabCamera->m_PtrCameraPara->m_iPixelFormat == 1)
		PtrGrabCamera->m_PtrImageUnit->m_dwImageFormat = 1;
	else
		PtrGrabCamera->m_PtrImageUnit->m_dwImageFormat = 3;

	dwImageLen = PtrGrabCamera->m_PtrCameraPara->m_uImageHeight*PtrGrabCamera->m_PtrCameraPara->m_uImageWidth*iPixelSize;

	PtrGrabCamera->m_PtrImageUnit->m_dwImageDataLen = dwImageLen;
	PtrGrabCamera->m_PtrImageUnit->m_dwImageData = (unsigned char*)pData;

	//����ָ��Ϊ����ָ��
	//******�����ⲿ�����Ļ�ȡͼƬ���ݵĺ���
	PtrGrabCamera->m_PtrHaiKangFunOnGetImage(
		PtrGrabCamera->m_PtrImageUnit,
		PtrGrabCamera->m_PtrCameraPara->m_iCamID,
		PtrGrabCamera->m_PtrHaiKangFunOnGetImageParam);

	PtrGrabCamera->m_IsNormCameraStatus = true;

	PtrGrabCamera->HaiKangCameraChangeEvent(ceGrabbing);
}

//static void __stdcall ReconnectDevice(unsigned int nMsgType, void* pUserParam)
//{
//	if (nMsgType != MV_EXCEPTION_DEV_DISCONNECT)
//		return;
//
//	CHaiKangCameraGrab* PtrGrabCamera = (CHaiKangCameraGrab*)pUserParam;
//
//	if (PtrGrabCamera->m_IsNormCameraStatus == true)
//	{
//		PtrGrabCamera->HaiKangStartGrab();
//
//		PtrGrabCamera->HaiKangCloseCamera();
//
//		Sleep(500);
//
//		PtrGrabCamera->HaiKangOpenCamera();
//
//		PtrGrabCamera->HaiKangStartGrab();
//	}
//}

static void __stdcall ReconnectDevice(unsigned int nMsgType, void* pUserParam)
{
	if (nMsgType != MV_EXCEPTION_DEV_DISCONNECT)
		return;

	CHaiKangCameraGrab* PtrGrabCamera = (CHaiKangCameraGrab*)pUserParam;

	if (PtrGrabCamera->m_IsNormCameraStatus == true)
	{
		PtrGrabCamera->HaiKangStartGrab();

		PtrGrabCamera->HaiKangCloseCamera();

		Sleep(500);

		PtrGrabCamera->HaiKangOpenCamera();

		PtrGrabCamera->HaiKangStartGrab();
	}
}

// �����
bool CHaiKangCameraGrab::HaiKangOpenCamera()
{
	bool IsFindCarema = false;
	const char* PtrSerialNumber = { '\0' };
	char ServerName[256] = { '\0' }, CameraName[256] = { '\0' };

	int  SerialNameIndex = 0, CameraNameIndex = 0;

	/**************************************************
	�豸����---ȷ���������ָ��
	***************************************************/
	// ch:ö���豸 | Enum device
	MV_CC_DEVICE_INFO_LIST stDeviceList;
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

	int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);

	if (MV_OK != nRet || stDeviceList.nDeviceNum == 0)
	{
		printf("Enum Devices fail! nRet [0x%x]\n", nRet);

		return false;
	}

	for (unsigned int hIndex = 0; hIndex < stDeviceList.nDeviceNum; hIndex++)
	{
		MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[hIndex];

		PtrSerialNumber = (const char*)(char*)pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber;
		//strcpy(SerialNumber, pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);

		//����������к��жϵ�ǰ�Ƿ�Ϊ��Ҫ�����
		if (!strcmp(PtrSerialNumber, m_PtrCameraPara->m_szSerialNum) == 1)
			continue;

		IsFindCarema = true;
		SerialNameIndex = hIndex;
		break;
	}

	if (!IsFindCarema)
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "CHaiKangCamGrab::δ����ָ�����кŵ��豸!");

		return false;
	}

	nRet = MV_CC_CreateHandle(&m_PtrDeviceHandle, stDeviceList.pDeviceInfo[SerialNameIndex]);

	if (MV_OK != nRet)
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "CHaiKangCamGrab::MV_CC_CreateHandle Failed!");

		return false;
	}

	nRet = MV_CC_OpenDevice(m_PtrDeviceHandle);

	if (MV_OK != nRet)
	{
		MV_CC_DestroyHandle(m_PtrDeviceHandle);
		m_PtrDeviceHandle = NULL;

		m_LogFile.LogFormat(NULL, LOG_INFO, "CHaiKangCamGrab::MV_CC_OpenDevice Failed!");

		return false;
	}

	if (!SetExposureTimeConf())
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "set exposuretime config fail.\n");

		return false;
	}

	//���þ�����
	if (!SetReverseConf())
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "set reverse config fail.\n");

		return false;
	}

	//����������ظ�ʽ
	if (!SetPixelFormatConf())
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "set PixelFormat config fail.\n");

		return false;
	}

	//����AOI����
	if (!SetImageAOIConf())
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "set ImageAOI config fail.\n");

		return false;
	}

	//���ô�������Դ
	if (!SetLineTriggerConf())
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "set ImageAOI config fail.\n");

		return false;
	}

	if (!SetFrameRate())
	{
		m_LogFile.LogFormat(NULL, LOG_INFO, "set FrameRate config fail.\n");

		return false;
	}

	m_nTriggerCount = 0;	m_nGrabImageNum = 0;	m_nFailedImageNum = 0;

	/**************************************************
	����ⲿ��������---�ⲿ��������
	***************************************************/
	m_PtrImageUnit->m_dwHeight = m_PtrCameraPara->m_uImageHeight;
	m_PtrImageUnit->m_dwWidth = m_PtrCameraPara->m_uImageWidth;

	m_PtrImageUnit->m_dwImageFormat = m_PtrCameraPara->m_uImageChannel;

	int iPixelSize = (m_PtrCameraPara->m_iPixelFormat == 0) ? 1 : 1;

	m_PtrImageUnit->m_dwImageDataLen = m_PtrImageUnit->m_dwHeight * m_PtrImageUnit->m_dwWidth * iPixelSize;

	m_IsNormCameraStatus = true;

	CHaiKangGrabBase::HaiKangOpenCamera();

	return true;
}

bool CHaiKangCameraGrab::HaiKangCloseCamera()
{
	m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::CloseCamera->enter");

	HaiKangStopGrab();

	if (m_PtrDeviceHandle != NULL)
	{
		MV_CC_CloseDevice(m_PtrDeviceHandle);

		MV_CC_DestroyHandle(m_PtrDeviceHandle);
		m_PtrDeviceHandle = NULL;

		m_bHaiKangConnected = false;
	}

	CHaiKangGrabBase::HaiKangCloseCamera();

	m_LogFile.LogFormat(NULL, LOG_INFO, "CHaiKangCameraGrab::CloseCamera->leave");

	return true;
}

//���ɼ������Ƶ����ڲ��������߳��߼�ת�Ƶ���ʼ�����ʱ
//�����߳����ڳ�ʼ����������ڽ���������쳣ʱ���ָ����
bool CHaiKangCameraGrab::HaiKangStartGrab()
{
	if (!IsHaiKangConnected())
		return false;

	if (IsHaiKangGrabbing())
		return false;

	if (m_IsNormCameraStatus == true)
	{
		MV_CC_SetImageNodeNum(m_PtrDeviceHandle, 30);

		//ע��ͼ��ɼ��ص�����
		int ret = MV_CC_RegisterImageCallBackEx(m_PtrDeviceHandle, ImageCallBackEx, this);

		//ע���쳣�¼��ص�����
		//MV_CC_RegisterExceptionCallBack(m_PtrDeviceHandle, ReconnectDevice, this);

		MV_CC_StartGrabbing(m_PtrDeviceHandle);

		//��ʾ���������ɼ�״̬
		m_bHaiKangGrabbing = true;
	}

	CHaiKangGrabBase::HaiKangStartGrab();

	return true;
}

bool CHaiKangCameraGrab::HaiKangStopGrab()
{
	if (!IsHaiKangConnected())
		return false;

	if (!IsHaiKangGrabbing())
		return false;

	if (m_IsNormCameraStatus == true)
	{
		MV_CC_StopGrabbing(m_PtrDeviceHandle);
	}

	CHaiKangGrabBase::HaiKangStopGrab();

	return true;
}

bool CHaiKangCameraGrab::SetTriggerSoftware()
{
	DWORD dwRet = 1;

	if (!IsHaiKangConnected())
		return dwRet;

	if (m_IsNormCameraStatus == true)
		MV_CC_SetTriggerSource(m_PtrDeviceHandle, SOFTWAREMODE);

	return true;
}

bool CHaiKangCameraGrab::HaiKangGrabImage()
{
	int iPixelSize = 1, dwImageLen = 0;
	if (m_PtrCameraPara->m_uImageChannel == 3)
		iPixelSize = 2;
	else
		iPixelSize = 1;

	dwImageLen = m_PtrCameraPara->m_uImageWidth*m_PtrCameraPara->m_uImageHeight*iPixelSize;

	if (!IsHaiKangConnected())
		return false;

	//Camera_t::StreamGrabber_t *ptrBalserStream = (Camera_t::StreamGrabber_t *)m_PtrBalserStream;

	//try
	//{
	//	//�����ⴥ����������
	//	//�����ÿ��ܵ��¶�֡���� 5*1000/m_GrabOption.m_iFrameRate ���޸�Ϊ����ģʽ  BPF 2013-07-06
	//	if (m_IsNormCameraStatus == true && ptrBalserStream->GetWaitObject().Wait(500))
	//	{
	//		// Get the grab result from the grabber's result queue
	//		Pylon::GrabResult Result;
	//		ptrBalserStream->RetrieveResult(Result);   //��ȡ�����ͼ��buffer���������

	//		if (Pylon::Grabbed == Result.Status())
	//		{
	//			if (m_PtrBalserFunOnGetImage != NULL)
	//			{
	//				m_PtrImageUnit->m_dwHeight = m_PtrCameraPara->m_uImageHeight;
	//				m_PtrImageUnit->m_dwWidth = m_PtrCameraPara->m_uImageWidth;
	//				m_PtrImageUnit->m_dwImageFormat = m_PtrCameraPara->m_uImageChannel;

	//				m_PtrImageUnit->m_dwImageDataLen = dwImageLen;
	//				m_PtrImageUnit->m_dwImageData = (unsigned char *)Result.Buffer();

	//				m_nTriggerCount = ptrBalserStream->Statistic_Total_Buffer_Count.GetValue();
	//				m_nFailedImageNum = ptrBalserStream->Statistic_Failed_Buffer_Count.GetValue();
	//				m_nFailedImageNum = ptrBalserStream->Statistic_Buffer_Underrun_Count.GetValue();

	//				//����ָ��Ϊ����ָ��
	//				m_PtrBalserFunOnGetImage(m_PtrImageUnit, m_PtrCameraPara->m_iCamID,
	//					m_PtrBalserFunOnGetImageParam);
	//			}

	//			BalserCameraChangeEvent(ceGrabbing);			

	//			ptrBalserStream->QueueBuffer(Result.Handle(), NULL);				
	//		}
	//		else
	//		{
	//			//m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::GetImageData->Error, no grabbed"); 

	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		//m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::GetImageData->Error, timeout"); 

	//		return false;
	//	}
	//}
	//catch(GenICam::GenericException &e)
	//{
	//	//m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::GrabImage->Error, exception:%s", e.GetDescription()); 

	//	m_IsNormCameraStatus = false;
	//}	

	return true;
}

//�����ɨ���
bool CHaiKangCameraGrab::HaiKangGrabLine()
{
	int iPixelSize = 0, dwImageLen = 0;

	if (m_PtrCameraPara->m_uImageChannel == 3)
		iPixelSize = 2;
	else
		iPixelSize = 1;

	dwImageLen = m_PtrCameraPara->m_uImageWidth*m_PtrCameraPara->m_uLineHeight*iPixelSize;

	unsigned char* TempGrabData = NULL;

	if (!IsHaiKangConnected())
		return false;

	//Camera_t::StreamGrabber_t *ptrBalserStream = (Camera_t::StreamGrabber_t *)m_PtrBalserStream;

	////��ͬ���ź���λ---������ɼ�������ʼ����ͬ���ź�
	///*if(!m_SyncSignal.IsSetEvent())
	//	m_SyncSignal.Singnal();*/

	//try
	//{
	//	//�����ⴥ����������
	//	//�����ÿ��ܵ��¶�֡���� 5*1000/m_GrabOption.m_iFrameRate ���޸�Ϊ����ģʽ  BPF 2013-07-06
	//	if (m_IsNormCameraStatus == true && ptrBalserStream->GetWaitObject().Wait(500))
	//	{
	//		// Get the grab result from the grabber's result queue
	//		Pylon::GrabResult Result;
	//		ptrBalserStream->RetrieveResult(Result);   //��ȡ�����ͼ��buffer���������

	//		if (Pylon::Grabbed == Result.Status())
	//		{
	//			/*if(m_SyncSignal.bStart)
	//			{*/
	//			if (m_PtrBalserFunOnGetImage != NULL)
	//			{						
	//				m_nTriggerCount = ptrBalserStream->Statistic_Total_Buffer_Count.GetValue();
	//				m_nFailedImageNum = ptrBalserStream->Statistic_Failed_Buffer_Count.GetValue();
	//				m_nFailedImageNum = ptrBalserStream->Statistic_Buffer_Underrun_Count.GetValue();

	//				//��ǰͼ�����
	//				//if (m_nCurrLineIndex < m_PtrCameraPara->m_uImageHeight)
	//				//{
	//				//	TempGrabData = m_PtrImageUnit->m_dwImageData + m_nCurrLineIndex*m_PtrImageUnit->m_dwWidth*iPixelSize;

	//				//	memcpy(TempGrabData, (unsigned char *)Result.Buffer(), m_PtrCameraPara->m_uLineHeight*m_PtrImageUnit->m_dwWidth*iPixelSize);

	//				//	m_nCurrLineIndex += m_PtrCameraPara->m_uLineHeight;
	//				//}

	//				////��ͼ����������ͼ��Ҫ��
	//				//if (m_nCurrLineIndex >= m_PtrCameraPara->m_uImageHeight)
	//				//{
	//				//	m_nCurrLineIndex = 0;  m_nGrabLineImageNum++;

	//				//	m_PtrImageUnit->m_dwHeight = m_PtrCameraPara->m_uImageHeight;
	//				//	m_PtrImageUnit->m_dwWidth = m_PtrCameraPara->m_uImageWidth;
	//				//	m_PtrImageUnit->m_dwImageFormat = m_PtrCameraPara->m_uImageChannel;

	//				//	m_PtrImageUnit->m_dwImageDataLen = dwImageLen;

	//				//	//����ָ��Ϊ����ָ��
	//				//	m_PtrBalserFunOnGetImage(m_PtrImageUnit, m_PtrCameraPara->m_iCamID,
	//				//		m_PtrBalserFunOnGetImageParam);
	//				//}						
	//			}

	//			BalserCameraChangeEvent(ceGrabbing);
	//			//}					

	//			ptrBalserStream->QueueBuffer(Result.Handle(), NULL);				
	//		}
	//		else
	//		{
	//			//m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::GetImageData->Error, no grabbed"); 

	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		//m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::GetImageData->Error, timeout"); 

	//		return false;
	//	}
	//}
	//catch(GenICam::GenericException &e)
	//{
	//	//m_LogFile.LogFormat(NULL, LOG_INFO, "CBalserCameraGrab::GrabImage->Error, exception:%s", e.GetDescription()); 

	//	m_IsNormCameraStatus = false;
	//}	

	return true;
}

bool CHaiKangCameraGrab::GetCameraSerialNumber(char* SerialNumber)
{
	if (!IsHaiKangConnected())
		return false;

	/*Camera_t * ptrBalserCamera = (Camera_t *)m_ptrBalserCamera;

	ptrBalserCamera->DeviceModelName.GetValue();*/

	return true;
}

bool CHaiKangCameraGrab::GetCameraNormStatus()
{
	return m_IsNormCameraStatus;
}

// ����һ��
int CHaiKangCameraGrab::GetCameraImg()
{
	int result = MV_CC_TriggerSoftwareExecute(m_PtrDeviceHandle);
	return result;
}