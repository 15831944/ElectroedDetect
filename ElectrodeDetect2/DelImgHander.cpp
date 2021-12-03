#include "pch.h"
#include "DelImgHander.h"

#include "ElectrodeDetect2Dlg.h"
#include "TaskThread.h"

LRESULT __cdecl FunCallBackDelImgThread(void* param)
{
	CDelImgHander* handle = (CDelImgHander*)param;

	bool flag = true;
	CElectrodeDetect2Dlg* Parent = handle->m_pParent;

	while (flag)
	{
		Parent->LoadDiskVolume("D");	// ��ȡD�̿ռ�
		if (Parent->m_Disk_Prop > Parent->m_Disk_Input_Prop)							// �ж�Ӳ��ʣ������ռ���Ƿ����ֶ�����ռ��-10%����
		{
			Parent->TravelFolder(Parent->m_strSaveImagePath.GetBuffer());				// �������ҳ����������ͼƬ�ļ���
			Parent->DelFolderByPath(Parent->m_FirstDelImgFolderPath.GetBuffer());
			int a = 1;
		}
		else
		{
			flag = false;
		}
	}
	Parent->LoadDiskVolume("D");	// ˢ��D�̿ռ�

	return true;
}

CDelImgHander::CDelImgHander(CElectrodeDetect2Dlg* Parent,int TimeInterval)
{
	m_pParent = Parent;

	// ʵ�����̶߳���
	m_pThread = new CPollingThread();
	// �����߳�������ִ�еĻص�����
	m_pThread->SetCallBackFun(FunCallBackDelImgThread,this, TimeInterval);

	HANDLE handle;
	handle = m_pThread->Start();
}

CDelImgHander::~CDelImgHander()
{
	// ֹͣ�߳�
	if (m_pThread->IsRuning())
		m_pThread->Stop();

	// �ͷ��̵߳��ڴ�ռ�
	delete m_pThread;
	m_pThread = NULL;
}