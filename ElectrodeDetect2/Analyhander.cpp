
#include "pch.h"
#include "ElectrodeDetect2Dlg.h"

#include "FileHander.h"
#include "TaskThread.h"
#include "Analyhander.h"
#include "SaveImgHander.h"

#include <DTTask/DTUtil.h>
#include <LogFile/LogFile.h>
#include <Utility/Utility.h>

#include "./../ZCVisionTool/ZCVisionTool.h"

extern CLogFile m_LogFile;

//��ǰ�߳��ж��ַ�����
bool AddOpenTask(CElectrodeDetect2Dlg* dlgVision)
{
	FileTask* PtrFileTask;
	PtrFileTask = new FileTask;
	PtrFileTask->Init();
	PtrFileTask->TaskType = TASK_OPEN;

	int QuestTime = 0;
	while (true)
	{
		if (dlgVision->m_PtrFileHander->AddFileTask(PtrFileTask) || QuestTime > 10)
			break;

		QuestTime++;

		Sleep(5);
	}

	free(PtrFileTask);

	return true;
}

bool CAnalyHander::AddWriteTask(AnalyTask* PtrAnalyTask, CElectrodeDetect2Dlg* dlgVision)
{
	FileTask* PtrFileTask;
	PtrFileTask = new FileTask;
	PtrFileTask->Init();
	PtrFileTask->TaskType = TASK_WRITE;
	PtrFileTask->DataId = PtrAnalyTask->PictureId;

	if (PtrAnalyTask->DetectResult != TOOL_PASS)
		PtrFileTask->iResult = 1;
	else
		PtrFileTask->iResult = 0;

	if (PtrAnalyTask->DetectResult == TOOL_ERROR_OUT_RANGE)
	{
		float CurDif = ((VisionParaOut*)PtrAnalyTask->PtrParaOut)->CycleDistance * dlgVision->m_YResolution - dlgVision->m_CyclePara.stdCycleLength;
		float FixedDif = CurDif - int(CurDif / dlgVision->m_CyclePara.dev) * dlgVision->m_CyclePara.dev;

		PtrFileTask->CycleValue = dlgVision->m_CyclePara.stdCycleLength + FixedDif;

		CurDif = ((VisionParaOut*)PtrAnalyTask->PtrParaOut)->HalfCycleDistance * dlgVision->m_YResolution - dlgVision->m_HalfCyclePara.stdCycleLength;
		FixedDif = CurDif - int(CurDif / dlgVision->m_HalfCyclePara.dev) * dlgVision->m_HalfCyclePara.dev;

		PtrFileTask->HalfCycleValue = dlgVision->m_HalfCyclePara.stdCycleLength + FixedDif;
	}
	else
	{
		PtrFileTask->CycleValue = ((VisionParaOut*)PtrAnalyTask->PtrParaOut)->CycleDistance * dlgVision->m_YResolution;
		PtrFileTask->HalfCycleValue = ((VisionParaOut*)PtrAnalyTask->PtrParaOut)->HalfCycleDistance * dlgVision->m_YResolution;
	}

	m_CycleDistance = PtrFileTask->CycleValue;
	m_HalfCycleDistance = PtrFileTask->HalfCycleValue;

	dlgVision->m_PtrFileHander->AddFileTask(PtrFileTask);

	free(PtrFileTask);
	return true;
}

LRESULT __cdecl FunCallBackAnalyThread(void* param)
{
	CAnalyHander* hander = (CAnalyHander*)param;
	
	if (hander->IsQueueEmpty())			
		return TRUE;

	AnalyTask analyTask;

	if (!hander->GetImageTask(&analyTask))
		return TRUE;

	CElectrodeDetect2Dlg* parent = hander->m_pParent;
	//  �̹߳���
	int CameraIndex = hander->m_CameraIndex;

	// ��ȡͼƬ����
	cv::Mat GrayImage(cvSize(analyTask.ImageUnit.m_dwWidth, analyTask.ImageUnit.m_dwHeight), CV_8U);
	memcpy(GrayImage.data,
		analyTask.ImageUnit.m_dwImageData,
		analyTask.ImageUnit.m_dwWidth * analyTask.ImageUnit.m_dwHeight);

	int index = analyTask.PictureId % 4;

	cv::Mat LabeledImg;

	if (CameraIndex == 0)		// ����Dalsa�����ʾ��ͼƬ
	{
		// �����㷨�������1��ͼƬ�����㷨������ɵ�ͼƬ��ʾ����
		Cam1_Function(GrayImage, (VisionParaIn*)analyTask.PtrParaIn, (VisionParaOut*)analyTask.PtrParaOut, LabeledImg);

		VisionParaOut* p = ((VisionParaOut*)analyTask.PtrParaOut);
		/*  �жϺ����Ƿ���ϱ�׼ */
		parent->DetectImageProcess_First(&analyTask);

		/* ���ݺ�������������º�������Ŀ */
		CString str_cam1;

		parent->m_NCam1_Sum++;
		str_cam1.Format("%d", parent->m_NCam1_Sum);
		parent->m_Cam1_Sum.SetWindowText(str_cam1);

		if (analyTask.DetectResult == TRUE)
		{
			parent->m_NCam1_Pass++;
		}
		else if (analyTask.DetectResult == FALSE)
		{
			parent->m_NCam1_Fail++;
		}

		str_cam1.Format("%d", parent->m_NCam1_Pass);
		parent->m_Cam1_Pass.SetWindowText(str_cam1);
		str_cam1.Format("%d", parent->m_NCam1_Fail);
		parent->m_Cam1_Fail.SetWindowText(str_cam1);

		// ��ӱ���csv�ļ����߳�
		if (!parent->m_PtrFileHander->m_IsOpenFile)
		{
			AddOpenTask(parent);
		}

		hander->AddWriteTask(&analyTask, parent);

		parent->m_Camera_1_List[index].SetDetPass(analyTask.DetectResult);
		parent->m_Camera_1_List[index].SetDistance(hander->m_CycleDistance, hander->m_HalfCycleDistance);
		parent->m_Camera_1_List[index].SetImgIndex(analyTask.PictureId);
		parent->m_Camera_1_List[index].UpdateImageData(LabeledImg.data, LabeledImg.cols, LabeledImg.rows, LabeledImg.total() * LabeledImg.channels(), LabeledImg.channels());
		parent->m_Camera_1_List[index].TriggerInvalidate();

		parent->m_AnalyFreeTime = 0;

		//  ����ͼƬ�߳�
		parent->m_pDalsaSaveImgHander[index]->AddImageTask(&analyTask);
	}
	else if(CameraIndex == 1)	// ���º��������ʾ��ͼƬ
	{
		// �����㷨�������2��ͼƬ�����㷨������ɵ�ͼƬ��ʾ���� 
		Cam2_Function(GrayImage, (VisionParaIn*)analyTask.PtrParaIn, (VisionParaOut*)analyTask.PtrParaOut, LabeledImg);

		parent->m_Camera_2_List[index].SetImgIndex(analyTask.PictureId);
		parent->m_Camera_2_List[index].UpdateImageData(LabeledImg.data, LabeledImg.cols, LabeledImg.rows, LabeledImg.total() * LabeledImg.channels(), LabeledImg.channels());
		parent->m_Camera_2_List[index].TriggerInvalidate();

		((VisionParaOut*)analyTask.PtrParaOut)->DetectResult = TOOL_PASS;
		//  ����ͼƬ�߳�
		parent->m_pHaiKangSaveImgHander[index]->AddImageTask(&analyTask);

		/* ��������ɼ�ͼƬ����Ŀ */
		CString str_cam2;

		parent->m_NCam2_Sum++;
		str_cam2.Format("%d", parent->m_NCam2_Sum);
		parent->m_Cam2_Sum.SetWindowText(str_cam2);

		if (((VisionParaOut*)analyTask.PtrParaOut)->DetectResult == TOOL_PASS)
		{
			parent->m_NCam2_Pass++;
			str_cam2.Format("%d", parent->m_NCam2_Pass);
			parent->m_Cam2_Pass.SetWindowText(str_cam2);
		}
		else if(((VisionParaOut*)analyTask.PtrParaOut)->DetectResult == TOOL_ERROR)
		{
			parent->m_NCam2_Fail++;
			str_cam2.Format("%d", parent->m_NCam2_Fail);
			parent->m_Cam2_Fail.SetWindowText(str_cam2);
		}
	}

	return true;
}

// CameraIndex��0��ʾDalsa������������1��ʾ�������
CAnalyHander::CAnalyHander(int CameraIndex, CElectrodeDetect2Dlg* Parent)
{
	m_pParent = Parent;
	m_CameraIndex = CameraIndex;

	std::queue<AnalyTask> emptyQueue;
	m_Task.swap(emptyQueue);

	m_DetectResult = true;

	m_pThread = new CPollingThread();		
	m_pThread->SetCallBackFun(FunCallBackAnalyThread, this, 10);

	HANDLE handle;
	handle = m_pThread->Start();
}

CAnalyHander::~CAnalyHander()
{
	if (m_pThread->IsRuning())
		m_pThread->Stop();

	delete m_pThread;
	m_pThread = NULL;

	std::queue<AnalyTask> emptyQueue;
	m_Task.swap(emptyQueue);
}

bool CAnalyHander::AddImageTask(AnalyTask* Task)
{
	m_Task.push(*Task);

	return true;
}

bool CAnalyHander::IsQueueEmpty()
{
	if (m_Task.size() <= 0)
		return true;

	return false;
}

bool CAnalyHander::GetImageTask(AnalyTask* Task)
{
	if (m_Task.size() <= 0)
		return false;

	*Task = m_Task.front();
	m_Task.pop();

	return true;
}