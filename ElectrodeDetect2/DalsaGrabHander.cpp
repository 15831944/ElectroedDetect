#include "pch.h"
#include "DalsaGrabHander.h"
#include "Analyhander.h"
#include "ElectrodeDetect2Dlg.h"
#include "FileHander.h"

#include <DTTask/DTUtil.h>
#include <Utility/Utility.h>

LRESULT __cdecl FunCallBackDalsaGetImg(ImageUnitBase* PtrImgInfo, int CameraID, void* param)
{
	CElectrodeDetect2Dlg* dlgVision = (CElectrodeDetect2Dlg*)param;

	// ��ȡ��ͼƬ
	if (!dlgVision->m_IsDetecting)
	{
		dlgVision->m_IsDetecting = true;
		dlgVision->m_ProductID++;
		dlgVision->m_CurrTotalNum_Dalsa = 0;
		dlgVision->m_CurrTotalNum_HaiKang = 0;

		dlgVision->m_NCam1_Sum = 0;
		dlgVision->m_NCam1_Pass = 0;
		dlgVision->m_NCam1_Fail = 0;
	}

	AnalyTask CamAnalyTask;

	CamAnalyTask.Init();

	CamAnalyTask.PtrParaIn = dlgVision->m_PtrCSIVisionIn;
	CamAnalyTask.PtrParaOut = dlgVision->m_PtrCSIVisionOut;

	// ImageUnit��ʾͼ��Ļ������������ߡ���ʽ��ͼ������ָ��
	CamAnalyTask.ImageUnit.m_dwHeight = PtrImgInfo->m_dwHeight;
	CamAnalyTask.ImageUnit.m_dwWidth = PtrImgInfo->m_dwWidth;
	CamAnalyTask.ImageUnit.m_dwImageDataLen = PtrImgInfo->m_dwImageDataLen;
	CamAnalyTask.ImageUnit.m_dwImageFormat = PtrImgInfo->m_dwImageFormat;
	CamAnalyTask.ImageUnit.m_dwImageData = PtrImgInfo->m_dwImageData;

	CamAnalyTask.PictureId = dlgVision->m_CurrTotalNum_Dalsa;

	dlgVision->m_pDalsaAnalyHander[dlgVision->m_CurrTotalNum_Dalsa % 4]->AddImageTask(&CamAnalyTask);

	dlgVision->m_CurrTotalNum_Dalsa++;

	return TRUE;
}

CDalsaGrabHander::CDalsaGrabHander(int cameraIndex, CameraGrabPara* dGrabPara, void* dlgDetection)
{
	m_ExitFlag = false;
	// �����������úõ����������ֵ������������������Ĳ�������
	memcpy(m_pCameraPara, dGrabPara, sizeof(CameraGrabPara));

	m_dlgDetection = dlgDetection;					// �����ڣ���ElectrodeDetectDlg

	m_cameraIndex = cameraIndex;					// �������
	m_cameraStatus = 0;								// ���״̬

	SetCallBackGetImageFun(FunCallBackDalsaGetImg, dlgDetection);	// ���û�ȡ��ͼƬ����õĺ���
	
	if (m_pCameraPara->m_uImageChannel == 1)
		pTempBuffer = NULL;
	else
		pTempBuffer = (unsigned char*)malloc(m_pCameraPara->m_uImageHeight * m_pCameraPara->m_uImageWidth * 3);
}

CDalsaGrabHander::~CDalsaGrabHander()
{
	m_ExitFlag = true;

	if (m_cameraStatus != 0 && m_cameraStatus != 4)
		DalsaCloseCamera();
}
