//�궨���ļ�---���幫�õĺ�
#ifndef _PUBLIC_FILE_H_
#define _PUBLIC_FILE_H_

#include <string>
#include <malloc.h>
#include <string.h>
#include <vector>
#include <atltime.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>
#include <iostream>
using namespace std;

#ifndef IGNORE_OPENCV
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc_c.h"
using namespace cv;
#endif

#define ARC_DIRECT_CLOSCWISE        1    
#define ARC_DIRECT_ANTI_CLOSCWISE   2

#define VL_MIN(x,y) (((x)<(y))?(x):(y))
#define VL_MAX(x,y) (((x)>(y))?(x):(y))
#define VL_PI        3.1415926
#define RADIAN       0.017453292519943 // pi/180
#define DEGREE       57.29577951308232 // 180/pi
#define MAX_FILE     512

/***************************************************************
       ��Թ��ߵļ������õ�����غ궨��
****************************************************************/
#define TOOL_INVALID                  0x0000         //��Ч
#define TOOL_PASS                     0x0001         //ͨ��
#define TOOL_ERROR                    0x0010
#define TOOL_ERROR_OUT_RANGE          0x0020
#define TOOL_NG_LESS_DIS              0x6110         //���벻����
#define TOOL_NG_NUM_RANGE             0x6120         //��Ŀ������
#define TOOL_NG_DISVAR_RANGE          0x6121         //�������
#define TOOL_NG_LENGTH_RANGE          0x6122         //���Ȳ�����
#define TOOL_NG_ANGLE_RANGE           0x6123         //�ǶȲ�����
#define TOOL_NG_EXIST_DEFECT          0x6124         //����ȱ��
#define TOOL_NG_RADIUS_RANGE          0x6125         //�뾶������
#define TOOL_NG_ECCENT_RANGE          0x6126         //�������ֵ������
#define TOOL_NG_LESS_ACCURAY          0x6127         //��ֵ������
#define TOOL_NG_STRNOTMATCH           0x6128         //�ַ�����ƥ��
#define TOOL_NG_NO_INFO               0x6129         //û�з���ƥ����Ϣ
#define TOOL_NG_LESS_SCORE            0x6130         //�÷ֲ�����

#define ANGLEERROR      1.6

#define ArrayInt std::vector<int>

#ifndef GenMsgLIB
#   if defined DLL_EXPORT
#       define GM_EXPORT __declspec(dllexport)
#   else
#       define GM_EXPORT __declspec(dllimport)
#   endif
#else
#    define  GM_EXPORT
#endif

#ifdef UPPROCESS
#define DLLAPI    extern "C"
#define STDCALL   _stdcall
#endif
#endif   //_PUBLIC_FILE_H_