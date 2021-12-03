#ifndef _GRABHAIKANG_HANDER_H_
#define _GRABHAIKANG_HANDER_H_

#include "CameraPara.h"						// �������
#include <HaiKangCamLib/HaiKangCamLib.h>

class CHaiKangGrabHander : public CHaiKangCameraGrab
{
public:
	CHaiKangGrabHander(int cameraIndex,CameraGrabPara* dGrabPara,void* dlgDetection);
	~CHaiKangGrabHander();

public:
	int m_cameraIndex;

	int  m_cameraStatus;		// 0Ϊ�����ʧ�ܣ�1Ϊ�򿪳ɹ���2Ϊ��ʼ�ɼ���3Ϊֹͣ�ɼ���4Ϊ����رճɹ�

	bool m_ExitFlag;

	void* m_dlgDetection;		// ������

	void SetTriModeOn();
};

#endif//_GRABHAIKANG_HANDER_H_

