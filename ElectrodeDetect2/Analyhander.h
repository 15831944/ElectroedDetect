#pragma once

#include "ZcStruct.h"

class CElectrodeDetect2Dlg;
class CPollingThread;

class CAnalyHander
{
public:
	CAnalyHander(int CameraIndex, CElectrodeDetect2Dlg* Parent);
	~CAnalyHander();
public:
	CElectrodeDetect2Dlg* m_pParent;
	CPollingThread* m_pThread;

	int m_CameraIndex;
	bool m_DetectResult;

	// ר����Dalsa����ɼ���ͼƬ�����ڡ�Բ������
	float m_CycleDistance;
	float m_HalfCycleDistance;

	std::queue<AnalyTask> m_Task;			// ���У�ͷ��Ϊ��

	bool AddImageTask(AnalyTask* Task);
	bool IsQueueEmpty();
	bool GetImageTask(AnalyTask* Task);
	bool AddWriteTask(AnalyTask* PtrAnalyTask, CElectrodeDetect2Dlg* dlgVision);
};