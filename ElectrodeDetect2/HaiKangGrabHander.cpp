#pragma once
#include "pch.h"
#include "ElectrodeDetect2Dlg.h"
#include "HaiKangGrabHander.h"
#include "Analyhander.h"

#include <DTTask/DTUtil.h>
#include <Utility/Utility.h>
#include <LogFile/LogFile.h>

// �����ȡ��ͼƬ����õĻص�����
LRESULT __cdecl FunCallBackHaiKangGetImg(ImageUnitBase* PtrImgInfo,int CameraID,void* param)
{
	CElectrodeDetect2Dlg* dlgVision = (CElectrodeDetect2Dlg*)param;

	//if (!dlgVision->m_IsDetecting)
	//{
	//	dlgVision->m_IsDetecting = true;
	//	dlgVision->m_ProductID++;
	//}

	AnalyTask CamAnalyTask;

	CamAnalyTask.Init();

	CamAnalyTask.PtrParaIn = NULL;
	CamAnalyTask.PtrParaOut = NULL;

	CamAnalyTask.PtrParaIn = dlgVision->m_PtrCSIVisionIn2;
	CamAnalyTask.PtrParaOut = dlgVision->m_PtrCSIVisionOut2;

	// ImageUnit��ʾͼ��Ļ������������ߡ���ʽ��ͼ������ָ��
	CamAnalyTask.ImageUnit.m_dwHeight = PtrImgInfo->m_dwHeight;
	CamAnalyTask.ImageUnit.m_dwWidth = PtrImgInfo->m_dwWidth;
	CamAnalyTask.ImageUnit.m_dwImageDataLen = PtrImgInfo->m_dwImageDataLen;
	CamAnalyTask.ImageUnit.m_dwImageFormat = PtrImgInfo->m_dwImageFormat;
	CamAnalyTask.ImageUnit.m_dwImageData = PtrImgInfo->m_dwImageData;

	CamAnalyTask.PictureId = dlgVision->m_CurrTotalNum_HaiKang;

	dlgVision->m_pHaiKangAnalyHander[dlgVision->m_CurrTotalNum_Dalsa % 4]->AddImageTask(&CamAnalyTask);

	dlgVision->m_CurrTotalNum_HaiKang++;

	return TRUE;
}

CHaiKangGrabHander::CHaiKangGrabHander(int cameraIndex, CameraGrabPara* dGrabPara, void* dlgDetection)
{
	m_ExitFlag = false;
	// �����������úõ����������ֵ������������������Ĳ�������
	memcpy(m_PtrCameraPara,dGrabPara,sizeof(CameraGrabPara));

	m_dlgDetection = dlgDetection;					// �����ڣ���ElectrodeDetectDlg

	m_cameraIndex = cameraIndex;					// �������
	m_cameraStatus = 0;								// ���״̬

	SetCallBackHaiKangGetImageFun(FunCallBackHaiKangGetImg,dlgDetection);	// ���û�ȡ��ͼƬ����õĺ���
}

CHaiKangGrabHander::~CHaiKangGrabHander()
{
	m_ExitFlag = true;

	if (m_cameraStatus != 0 && m_cameraStatus != 4)
		HaiKangCloseCamera();
}

void CHaiKangGrabHander::SetTriModeOn()
{
	SetLineTriggerConf();
}