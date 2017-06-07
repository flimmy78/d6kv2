
#ifndef _PARAM_MODBUS_H
#define _PARAM_MODBUS_H

#include <string>
using namespace std;

class CModbusParaConfig
{
public:
	unsigned short yx_func_code; //ң�Ź�����
	unsigned short yx_start_no;  //ң����ʼ���
	unsigned short yx_call_time; //ң���ٻ�ʱ��
	unsigned short yc_func_code; //ң�⹦����
	unsigned short yc_start_no;  //ң����ʼ���
	unsigned short yc_call_time; //ң���ٻ�ʱ��

	unsigned short yk_func_code; //ң�ع�����
	unsigned short yk_start_no;  //ң����ʼ���

	unsigned short yt_func_code;
	unsigned short yt_start_no;

	unsigned short kwh_func_code;
	unsigned short kwh_start_no;
	unsigned short kwh_call_time;
	unsigned short t0; //t0

public:
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

		yt_func_code=0x10;
		yt_start_no=0;

		kwh_func_code=0x04;
		kwh_call_time=1000;
		kwh_start_no=0;

		t0 = 15000 ;
	};
public:
	CModbusParaConfig()
	{
		set_default();
	};
};

#endif //_PARAM_MODBUS_H

