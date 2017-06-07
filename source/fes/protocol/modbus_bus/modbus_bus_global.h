#ifndef MODBUS_BUS_GLOBAL_H
#define MODBUS_BUS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MODBUS_BUS_LIB
# define MODBUS_BUS_EXPORT Q_DECL_EXPORT
#else
# define MODBUS_BUS_EXPORT Q_DECL_IMPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"



#define 	CODE_VAL           0x03    //��õ�ǰ������ڲ�һ��������ǰ�Ĵ���ֵ
#define  	CODE_GetSetVAL     0x04    //��õ�ǰ������ڲ����üĴ���ֵ
#define  	CODE_SETVAL        0x10    //�޸ĵ�ǰ������ڲ����üĴ���ֵ

#define     CODE_ERROR_OPER    0x01    //�յ��������������ظ�
#define     CODE_ERROR_ILLEGAL 0x02    //�յ��Ƿ��Ĵ��������������ݳ��ȳ���
#define     CODE_ERROR_COMM_1  0xFF    //��Ӧ��ַ CG2361S �Ǳ�ʹ�����ͨѶ������
#define     CODE_ERROR_COMM_2  0xFE    //CG2361C ����Ӧ��ַ�� CG2361S ͨѶ����
#define     CODE_ERROR_COMM_3  0xFD    //CG2361M �� CG2361C ͨѶ����
#define     CODE_ERROR_COMM_4  0xFC    //CG2361C ���õĲ��µ�С�� CG2361M ������ֵ

#define     PER_MSG_GetValNum  120     //ÿ֡��������ȡ������
#define     MAX_SET_VAL_NUM    4       //������üĴ�����Ŀ

enum FRAME_TYPE///<֡���
{
	INVALID_FRAME = -1,///<�Ƿ�֡
};


enum
{
	BIT_ADDRESS = 0,
	BIT_FUNCODE = 1,
	BIT_DATA_LEN = 2,
	BIT_DATA_HEAD = 3,
};

enum
{
	MAX_BUFF_LENGTH = 512 ,
};

enum
{
	ANALOG_TYPE,
	BINARY_TYPE,
	KWH_TYPE,
};

typedef unsigned char   BIT08;  //08λ
typedef unsigned short  BIT16;  //16λ
typedef unsigned long   BIT32;  //32λ


#define MYLO8(w)            ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)            ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

//ÿ��װ�õĻ�������
class CDeviceData
{
public:
	int m_nDeviceID;    //װ��ID
	int m_nDeviceAddr;  //װ�õ�ַ
	//װ������
public:
	vector<int> m_arrAnalogStartAddr;//��ʼ��ַ����(�����е㰴��������ÿ֡����PER_FRAME_*�����ݣ�ÿ֡��һ�����ݵ���ʼ��ַ)
	vector<int> m_arrAnalogCountPerFrame;
	//��ǰװ��û��ң�ţ��ɲ�����
	vector<int> m_arrBinaryStartAddr;
	vector<int> m_arrBinaryCountPerFrame;
public:
	int m_nErrorCount;              //��ǰ������Ŀ
	int m_nLastHaltTime;            //����ʱ��
	bool  m_bIsHalt;                //װ���Ƿ����
};


#endif // MODBUS_BUS_GLOBAL_H
