#pragma once

#include "CameraPara.h"
#include <DalsaCamLib/DalsaCamLib.h>

class CElectrodeDetect2Dlg;			// ������

class CDalsaGrabHander : public CDalseCameraGrab
{
public:
	//��ʼ���ɼ��߳�
	CDalsaGrabHander(int cameraIndex, CameraGrabPara* dGrabPara, void* dlgDetection);
	~CDalsaGrabHander();

public:

	int m_cameraIndex;

	int  m_cameraStatus;

	bool m_ExitFlag;

	void* m_dlgDetection;

	unsigned char* pTempBuffer;
};