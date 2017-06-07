/**
@file modbust_type_def.h
@brief ���ϵ�������modbus��Լ�������Ͷ���ͷ�ļ�

@author lujiashun
@version 1.0.0
@date 2016-03-23
*/

#ifndef MODBUST_TYPE_DEF_H_
#define MODBUST_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief modbust���ͱ�ʶ

ֻ�г��˳��õģ��ɸ�����Ҫ�͹�Լ�ĵ�����
*/
enum TYPE_IDENTIFICATION
{
	C_CG_TA_1 = 0x01,	///<ȷ����Ϣ����ʱ�����

	M_SP_TA_1 = 0x0f,	///<ң�ţ���ʱ�����
	M_ME_TA_1 = 0x10	///<ң���ң������ʱ�����
};

/**
@brief ���ݳ���
*/
enum DATA_LEN
{ 
	COMMON_ADDR_LEN = 1,///<������ַ����

	CALL_DATA_LEN = 8,		///<�ٻ�yc��yx��ym����֡����
	CONFIRM_LINK_LEN = 16,  ///<ȷ�ϱ��ĳ���

// 	ASDU_DATA_LEN = 256	///<ASDU��Ϣ�峤�ȣ����ر����壬��Ϣ��ύ�ɷ�����������
	ASDU_DATA_LEN = 512	///<ASDU��Ϣ�峤�ȣ����ر����壬��Ϣ��ύ�ɷ�����������
};

#if defined (__unix)
#define         LOBYTE(w)       ( (unsigned char) (w) )
#define         HIBYTE(w)       ( (unsigned char) ( (unsigned short) (w)>>8) )
#define         LOWORD(l)       ( (unsigned short) (l) )
#define         HIWORD(l)	( (unsigned short) ( (unsigned int) (l)>>16) ) 
#endif

#pragma pack(1)

/**
@brief �����е�ʱ���ʽ
*/
struct MODBUST_TIME
{
	unsigned char year;
	unsigned char month;
	unsigned char day;	
	unsigned char hour;
	unsigned char minite;
	unsigned char secend;
};

/**
@brief �����еĵ����,���ֽ�����ǰ�����ֽ��ں�
*/
struct MODBUST_KWH
{
// 	int32u	data_kwh;	//4�ֽ�
	unsigned char data_kwh_hh;		//���ֽ�
	unsigned char data_kwh_hl;		//
	unsigned char data_kwh_lh;		//
	unsigned char data_kwh_ll;		//���ֽ�
};
/**
@brief �����е�ң����
*/
struct MODBUST_YC
{
// 	int16u	data_yc;	//2�ֽ�
	unsigned char data_yc_h;	//���ֽ�
	unsigned char data_yc_l;	//���ֽ�
};
/**
@brief APCI : Application Protocol Control Information Ӧ�ù�Լ������Ϣ
*/
struct MODBUST_APCI  
{
	unsigned char   addr;    // buf[0] װ�õ�ַ
	unsigned char   type;    // buf[1] ��������
	unsigned char	send_num_h;			//������Ÿ��ֽ�
	unsigned char	send_num_l;			//������ŵ��ֽ�
	unsigned char	data_begin_addr_h;	//��ʼ��ַ���ֽ�
	unsigned char	data_begin_addr_l;	//��ʼ��ַ���ֽ�
	unsigned char	data_num_h;			//ң�⣨ң�š�ң�����������ֽ�
	unsigned char	data_num_l;			//ң�⣨ң�š�ң�����������ֽ�
	unsigned char	data_byte_num_h;		//���ݲ����ֽ������ֽ�
	unsigned char	data_byte_num_l;		//���ݲ����ֽ������ֽ�
};

/**
@brief APDU : Application Protocol Data Unit Ӧ�ù�Լ���ݵ�Ԫ
*/
typedef struct MODBUST_APDU  
{
	MODBUST_APCI    apci;  //8�ֽ�
	unsigned char	buf[ASDU_DATA_LEN];
};

#pragma pack()


/**
@brief IEC104������ṹ
*/
struct MODBUST_CONFIG
{
	unsigned short yx_begin;	///<ң����ʼ��ַ
	unsigned short yc_begin;	///<ң����ʼ��ַ
	unsigned short kwh_begin;	///<�����ʼ��ַ

	unsigned char reply_data_flag;	///<�յ�ң�⡢ң�š�ң�����ݺ�ظ�ȷ��֡��1Ϊ�ظ���0Ϊ���ظ�

	/**
	@brief ������Ĭ��ֵ
	*/
	void set_default(void)
	{
		yx_begin = 0x0000;
		yc_begin = 0x0000;
		kwh_begin = 0x0bb8;

		reply_data_flag = 0;	//Ĭ�ϲ��ظ�
	};

	/**
	@brief IEC104������ṹ���캯��
	*/
	MODBUST_CONFIG()
	{
		set_default();
	};
};

#endif