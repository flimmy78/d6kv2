/**
@file fesscdshm.h
@brief ǰ�ü�scada�����ݹ����ڴ�ṹ�嶨��ͷ�ļ�

�ṩǰ�������ݹ����ڴ�Ĵ��������롢���ʽӿ�

@author zhoujian 
@version 1.0.0
@date 2014-01-02
*/
#ifndef FES_SCADA_SHM_H_
#define FES_SCADA_SHM_H_

#include <limits>

const unsigned char UCHAR_DEFAULT = (std::numeric_limits<unsigned char>::max)();
const float FLOAT_DEFAULT = (std::numeric_limits<float>::max)();
const unsigned long ULONG_DEFAULT = (std::numeric_limits<unsigned long>::max)();

#define		YK_CHECK_MSG_LEN		128

/**
@brief ǰ�ú��뼶ʱ��ṹ
*/
typedef struct
{
	short   		year;	///<�꣬��1900�꿪ʼ
	unsigned char   month;	///<��
	unsigned char	day;	///<��
	unsigned char	hour;	///<Сʱ
	unsigned char 	minute;	///<��
	unsigned char   second;	///<��
	short			milisecond;	///<����
}FETIME;

/**
@brief �������η�
*/
typedef struct
{
	FETIME time;
	unsigned char quality;			//����λ
	unsigned char self_time_flag;	//�Ƿ��Դ�ʱ��

	void set_default(void)
	{
		quality = UCHAR_DEFAULT;
		self_time_flag = UCHAR_DEFAULT;
	};
}FE_DATA_MODIFIER;

/**
@brief ң���ʱ���������
*/
typedef struct
{
	FE_DATA_MODIFIER modifier;
	float raw_yc;
	float processed_yc;

	void set_default(void)
	{
		modifier.set_default();
		raw_yc = FLOAT_DEFAULT;			//������
		processed_yc = FLOAT_DEFAULT;	//����ת������
	};
}FE_YC;

/**
@brief ң�Ŵ�ʱ���������
*/
typedef struct
{
	FE_DATA_MODIFIER modifier;
	unsigned char raw_yx;			//������ 0:��λ 1:��λ 2:��Ч
	unsigned char processed_yx;		//����ת������

	void set_default(void)
	{
		modifier.set_default();
		raw_yx = UCHAR_DEFAULT;
		processed_yx = UCHAR_DEFAULT;
	};
}FE_YX;

/**
@brief ��ȴ�ʱ���������
*/
typedef struct
{
	FE_DATA_MODIFIER modifier;
	unsigned long raw_kwh;			//������
	unsigned long processed_kwh;	//����ת������

	void set_default(void)
	{
		modifier.set_default();
		raw_kwh = ULONG_DEFAULT;
		processed_kwh = ULONG_DEFAULT;
	};
}FE_KWH;

/**
@brief ���͸�SCADA��ʵʱ���ݽṹ
*/
typedef struct
{
	enum
	{
		YX_REALDATA,
		YC_REALDATA,
		KWH_REALDATA
	};
	unsigned char	type;	///<�������ͣ�YX/YC/KWH
	unsigned char	master_flag;	///<�Ƿ���ͨ������
	short	channel_no;		///<�������ݵ�ͨ����
	short	rtu_no;			///<�ն˺�
	short	order;			///<���ݵ��
	union
	{
		FE_YX	yx_value;	///<ң��ֵ
		FE_YC	yc_value;	///<ң��ֵ
		FE_KWH	kwh_value;	///<���ֵ
	}value;					///<����ֵ
}FE_REAL_DATA_NODE;

/**
@brief ��������
*/
enum EVENT_TYPE
{
	RTU_SOE = 0x01,		///<��ͨSOE
	HOST_SOE = 0x02,	///<����SOE������ͨ��λ
	STAION_EVENT = 0x05,
	DAS_DIAGNOSE = 0x15,
};


/**
@brief ���͸�SCADA��ʵʱ��������ṹ
*/
typedef struct
{

	FETIME	time;	///<�����ʱ��

	unsigned char	source;	///<����Դ
	unsigned char	type;	///<�������ͣ�Ŀǰ��DNP��Լʹ��
	unsigned char	master_flag;
	short	postion_holder1;	///<ռλ��
	short	channel;		///<����ͨ��
	short   rtuno;			///<rtu��
	union
	{
		struct
		{
			unsigned char	value;	///<ң��ֵ
			unsigned char	postion_holder2;	///<ռλ��
			short	order;	///<ң�����

		}yx;				///<ң��
		float		ycval;	///<ң��ֵ
	}val;					///<ֵ

	////����typeΪ�������ʱ�õ����������
// 	BYTE	accident_type; //�¹�����:0--�޷����� 1-����ӵ� 2-����·
// 	BYTE	accident_line; //0-���� 1-A�� 2-B�� 3-C�� 4-8���� 9-AB 10-BC 11-AC 12-ABC
// 	WORD	postion_holder3;
// 	WORD	yx_order1;
// 	WORD	ftu_no1;
// 	WORD	yx_order2;
// 	WORD	ftu_no2;
}FE_DATA_EVENT_NODE;


/**
@brief ���͸�SCADA��ң�ط�У�ṹ
*/
typedef struct
{
	unsigned char	master_flag;	///<�Ƿ���ͨ������
	short	channel_no;		///<�������ݵ�ͨ����
	short	rtu_no;			///<�ն˺�
	short	yk_no;			///<ң�غ�
	int		yk_ret;			///<ң�ط�Уֵ
	char	msg[YK_CHECK_MSG_LEN]; ///<��У����
}FE_YK_CHECK_NODE;
#endif