//#include "Public.h"
#include <time.h>

#define _CRTDBG_MAP_ALLOC   //���Ǿ�����Ҫ����䣬������и���䣬��ӡ���������ļ����������ȸ���ֱ�۵���Ϣ

#pragma once

#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>

#define MAX_FILE_UTI     512

class __declspec(dllexport) CUtility
{
public:
	CUtility();
	~CUtility();

	LARGE_INTEGER m_PerfFreq;
	LARGE_INTEGER m_PerfStart;
	LARGE_INTEGER m_PerfEnd;

	void StartTime();
	void EndTime();

	float dTime;

public:
	/*!
	\fun extern CString Utility_getApplicationPath(void);
	\brief ��ȡӦ�ó���·��.
	*/
	void Utility_getApplicationPath(char* dstPath);
	
	/*!
	\fun extern CString Utiltiy_getApplicationDir(void);
	\brief ��ȡӦ�ó������ڵ�Ŀ¼.
	*/
	void Utility_getApplicationDir(char* dstPath);
	
	/*!
	\fun extern CString Utility_GetCurTimeStr(void);
	\brief ��ȡ��ǰʱ����ַ��� ��ʽ: yyyy-mm-dd-hh-mm-ss.
	*/
	void Utility_GetCurTimeStr(char* dstPath);

	void Utility_GetCurYearMonthDayFormat(char* dstPath);
	void Utility_GetCurTimeStrFormat(char* dstPath);
	void Utility_GetCurTimeYearMonthDayFormat(char* dstPath);

	void Utility_GetCurTimeIndexFormat(char* dstPath);

	bool Utility_GetCurDayWeek(char* dstPath);

	void Utility_getDayStr(char* dstPath);
	void Utility_getHourStr(char* dstPath);
	
	/*!
	\fun extern CString Utility_getDateStr(void);
	\brief ��ȡ��ǰʱ����ַ��� ��ʽ: yyyy-mm.
	*/
	void Utility_getMonthStr(char* dstPath);
	
	/*!
	\fun extern BOOL Utility_CreateDirByPath(const char * path);
	\brief ����·��������Ӧ��Ŀ¼.
	*/
	BOOL Utility_CreateDirByPath (const char * srcPath);
	
	void Utility_getDirByPath (const char* srcPath, char* dstPath);

	void Utility_getFileNameByPath(const char* FilePath, char* FileName);
};

class __declspec(dllexport) CCheckMem
{
public:
	CCheckMem();
	~CCheckMem();
public:
	_CrtMemState s1, s2, s3;

	void MemCheckStartPoint();
	void MemCheckEndPoint();

	void MemDumpStatistics();
};