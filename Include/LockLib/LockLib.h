#ifndef __ZCYBLOCKLIB_H__
#define __ZCYBLOCKLIB_H__

#include "Public.h"

typedef struct tagLockInfo
{
	int  LockDataSize;               //ϵͳ��Ȩʱ��
	int  LockDataIndex;

	int  LockDataTime;           //Lockϵͳʱ��

	void Init()
	{
		LockDataSize = 0;  LockDataIndex = 0;

		LockDataTime = 0;
	}
}LockInfo;

DLLAPI int STDCALL ReadLockMessage(LockInfo* MemLock);

DLLAPI int STDCALL WriteLockMessage(LockInfo* MemLock);

//�жϼ����Ƿ�ɹ�
DLLAPI bool STDCALL IsLockMessageSucess(int SystemTime, int* RemainDataTime);

DLLAPI bool STDCALL IsLockerExist();

DLLAPI void STDCALL InitLockMessage(int SystemTime, int LockDataSize);

#ifdef UPPROCESS
#ifndef LOCKLIB_IMPLEMENT
#pragma comment(lib, "LockLib.lib")
#endif
#endif

#endif  //__ZCYBLOCKLIB_H__