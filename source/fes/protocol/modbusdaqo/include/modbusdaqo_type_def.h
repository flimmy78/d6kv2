/**
@file modbusdaqo_type_def.h
@brief ��ȫTCP modbus��Լ

@author chenkai
@version 1.0.0
@date 2016-07-21
*/

#ifndef MODBUSDAQO_TYPE_DEF_H_
#define MODBUSDAQO_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief modbusdaqo���ͱ�ʶ

ֻ�г��˳��õģ��ɸ�����Ҫ�͹�Լ�ĵ�����
*/
/**
@brief ֡���
*/
enum TYPE_IDENTIFICATION
{
	INVALID_FRAME = -1,///<�Ƿ�֡
	//����֡���
};

/**
@brief ���ĳ����ֽ�
*/
enum DATA_CON
{
	FIRST = 0xC6,   //<����ͷ
	SECOND = 0xC7 ,

	YC_CODE3 = 0x03 ,
	YX_CODE1 = 0x01 ,
	YK_CODE5 = 0x05 ,
	YK_CODE6 = 0x06 ,

	ERR_CODE = 0x80 ,

	PROTOCOL_FLAG = 0x00 ,

	CALL_YC = 0x20 ,
	CALL_YX = 0x30 ,
};

/**
@brief modbusdaqo������ṹ
*/
struct MODBUSDAQO_CONFIG
{
	unsigned short yx_func_code; //ң�Ź�����
	unsigned short yx_start_no; //ң����ʼ���
	unsigned short yx_call_time; //ң���ٻ�ʱ��
	unsigned short yc_func_code; //ң�⹦����
	unsigned short yc_start_no; //ң����ʼ���
	unsigned short yc_call_time; //ң���ٻ�ʱ��
	unsigned short yk_func_code; //ң�ع�����
	unsigned short yk_start_no; //ң����ʼ���
	unsigned short t0; //t0

	/**
	@brief ������Ĭ��ֵ
	*/
	void set_default(void)
	{
		yx_func_code = 1;
		yx_start_no = 3000;
		yx_call_time = 300;
		yc_func_code = 0x03;
		yc_start_no = 0 ;
		yc_call_time = 1000 ;
		yk_func_code = 0x06 ;
		yk_start_no = 9 ;
		t0 = 15000 ;
	};

	/**
	@brief IEC104������ṹ���캯��
	*/
	MODBUSDAQO_CONFIG()
	{
		set_default();
	};
};

#endif