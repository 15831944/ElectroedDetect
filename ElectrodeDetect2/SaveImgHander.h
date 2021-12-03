#pragma once

#include "ZcStruct.h"

class CElectrodeDetect2Dlg;
class CPollingThread;

class CSaveImgHander
{
public:
	CSaveImgHander(int CameraIndex, CElectrodeDetect2Dlg* Parent);
	~CSaveImgHander();

public:
	CElectrodeDetect2Dlg* m_pParent;
	CPollingThread* m_pThread;

	int m_CameraIndex;

	std::queue<AnalyTask> m_Task;			// ���У�ͷ��Ϊ��

	bool AddImageTask(AnalyTask* Task);
	bool IsQueueEmpty();
	bool GetImageTask(AnalyTask* Task);
};

