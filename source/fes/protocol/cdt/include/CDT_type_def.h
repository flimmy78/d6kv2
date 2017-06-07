/**
@file CDT_type_def.h
@brief 104��Լ�������Ͷ���ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/

#ifndef CDT_TYPE_DEF_H_
#define CDT_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief CDT���ͱ�ʶ

ֻ�г��˳��õģ��ɸ�����Ҫ�͹�Լ�ĵ�����
*/

/**
@brief ֡���
*/
enum TYPE_IDENTIFICATION
{
	INVALID_FRAME = -1,///<�Ƿ�֡
	//����֡���
	IMPORTANT_YC = 0x61,///<��Ҫң��
	SECOND_YC = 0xC2,	///<��Ҫң��
	COMM_YC = 0xB3,		///<һ��ң��
	STAT_YX = 0xF4,		///<ң��״̬
	KWH_PI = 0x85,		///<�����������ֵ
	YX_SOE = 0x26,		///<�¼�˳���¼
	YK_RET = 0x9E,		///<ң�ط�У

	//����֡���
	SEL_YK = 0x61,		///<ң��ѡ��
	EXCUTE_YK = 0xC2,	///<ң��ִ��
	CANCEL_YK = 0xB3,	///<ң�س���
	SEL_YT = 0xF4,		///<����ѡ��
	EXCUT_YT = 0x85,	///<����ִ��
	CANCEL_YT = 0x26,	///<��������
	SET_COMMAND = 0x57,	///<�趨����

	SYNC_TIME = 0x7A,	///<�趨ʱ��
	SET_TIME = 0x0B,	///<�趨ʱ��У��ֵ
	CALL_TIME = 0x4C,	///<�ٻ���վʱ��
	RESET = 0x3D,		///<��������
	BROADCAST = 0x9E,	///<�㲥����
	
};

/**
@brief ������
*/
enum CDT_FUNC_CODE
{
	FUNC_FREQUENCE	 = 0x8A,  //Ƶ��
	FUNC_RESET		 = 0x8b,  //��λ
	FUNC_BORADCAST   = 0x8C,  //�㲥
	FUNC_SEL_YK		 = 0xE0,  //ң��ѡ��
	FUNC_RET_YK		 = 0xE1,  //ң�ط�У
	FUNC_EXCUTE_YK   = 0xE2,  //ң��ִ��
	FUNC_CANCEL_YK   = 0xE3,  //ң�س���
	FUNC_SEL_YT		 = 0xE4,  //����ѡ��
	FUNC_RET_YT		 = 0xE5,  //������У
	FUNC_EXCUTE_YT   = 0xE6,  //����ִ��
	FUNC_CANCEL_YT	 = 0xE7,  //��������
	FUNC_SET_COMMAND = 0xE8,  //��������
	FUNC_BACKUP0	 = 0xE9,  //����
	FUNC_BACKUP1	 = 0xEA,  //����
	FUNC_BACKUP2	 = 0xEB,  //����
	FUNC_RTU_INFO	 = 0xEC,  //��վ״̬��Ϣ

	FUNC_SET_TIME	 = 0xED, //����ʱ��У��ֵ

};

/**
@brief ���������ڱ����е�λ��
*/
enum BYTE_POS_DEF
{
	START_ID = 0,

	CONTROL_BYTE_POS	= 6,
	FRAME_TYPE_POS      = 7,
	INFO_NUM_POS	    = 8,
	SRC_ADDR_POS	    = 9,
	DEST_ADDR_POS	    = 10,
	CALC_CODE_POS	    = 11,

	INFO_FIRS_POS		= 12,
};

/**
@brief ���ݳ���
*/
enum DATA_LEN
{ 
	COMMON_ADDR_LEN = 2,///<������ַ����
	COT_LEN = 2,		///<����ԭ�򳤶�
	INDEX_LEN = 3,		///<��Ϣ���ַ����
	CTL_FLD = 4,		///<�����򳤶�
	SINGLE_CHGYX_APDU_LEN = 14,  ///<��ң�ű�λ֡����
	HEAD_LEN = 10,		///<���Ŀ�ʼ����Ϣ��ĳ���
	ASDU_HEAD_LEN = 12, ///<�ӱ���ͷ��ASDU�ĳ���
	APDU_HEAD_LEN = 13, ///<APDU���Ĵӿ�������Ϣ����ֵ���ֳ���
	INFO_BEGIN_LEN = 15, //<�ӱ���ͷ����Ϣ����ֵ���ֳ���
	ASDU_DATA_LEN = 256	///<ASDU��Ϣ�峤�ȣ����ر����壬��Ϣ��ύ�ɷ�����������
};

/**
@brief ��������
*/
enum QUALITY_TYPE
{
	DATA_OVERLOAD = 0x01,    // ���    0:δ���    1:���
	DATA_BLOCKADE = 0x10,    // ����    0:δ������  1:������
	DATA_REPLACE  = 0x20,    // ȡ��    0:δ��ȡ��  1:��ȡ��
	DATA_CURRENT  = 0x40,    // ��ǰֵ  0:��ǰֵ    1:�ǵ�ǰֵ
	DATA_INVALID  = 0x80     // ��Ч    0:��Ч      1:��Ч
};

#pragma pack(1)
/**
@brief �������ֽ�
*/
struct CONTORL_BYTE		
{
	unsigned char control:4;
	unsigned char D:1;
	unsigned char S:1;
	unsigned char L:1;
	unsigned char E:1;
};

/**
@brief �����ֽṹ
*/
struct CONTROL_STRUCT    // S : Numbered Supervisory Function ��ŵļ��ӹ��ܸ�ʽ
{
	CONTORL_BYTE  contor_byte;
	unsigned char  frame_type;
	unsigned char  info_num;
	unsigned char  src_addr;
	unsigned char  dest_addr;
	unsigned char  calc_byte;
};

/**
@brief ��Ϣ�ֽṹ
*/
struct INFO_STRUCT    // S : Numbered Supervisory Function ��ŵļ��ӹ��ܸ�ʽ
{
	unsigned char  func_code;
	unsigned char  info_no1;
	unsigned char  info_no2;
	unsigned char  info_no3;
	unsigned char  info_no4;
	unsigned char  bch_code;
};

/**
@brief SOE��Ϣ�ֽṹ
*/
struct SOE_STRUCT    // S : Numbered Supervisory Function ��ŵļ��ӹ��ܸ�ʽ
{
	unsigned char  func_code1;
	unsigned char  milsecond_L;
	unsigned char  milsecond_H;
	unsigned char  seconds;
	unsigned char  minute;
	unsigned char  bch_code1;
	unsigned char  func_code2;
	unsigned char  hour;
	unsigned char  day;
	unsigned char  data_no_L;
	unsigned char  data_no_H:4;
	unsigned char  back_info:3;
	unsigned char  stat:1;
	unsigned char  bch_code2;
};


/**
@brief �����еĶ�ʱ��ʱ���ʽ
*/
struct CP24Time2a		
{
	int16u millsecond;
	unsigned char minutes:6;
	unsigned char res:1;
	unsigned char iv:1;
};

/**
@brief �����еĳ�ʱ��ʱ���ʽ
*/
struct CP56TIME
{
	int16u millsecond;
	unsigned char minute:6;
	unsigned char RES1:1;
	unsigned char IV:1;	
	unsigned char hour:5;
	unsigned char RES2:2;
	unsigned char SU:1;
	unsigned char day:5;
	unsigned char wday:3;
	unsigned char month:4;
	unsigned char RES3:4;
	unsigned char year:7;
	unsigned char RES4:1;
};

/**
@brief ��ʱ�ṹ
*/
struct SYNC_TIME_STRUCT
{
	int16u  index_no;
	unsigned char	pos_holder;
	CP56TIME cp56time;
};

/**
@brief CDTң�ر��Ľṹ
*/
struct CDT_YK_STRUCT
{
	CONTROL_STRUCT  control_struct;;
	INFO_STRUCT		info_strcut1;
	INFO_STRUCT		info_strcut2;
	INFO_STRUCT		info_strcut3;
};

#pragma pack()

/**
@brief CDT������ṹ
*/
struct CDT_CONFIG
{
	short call_all_data_gap;///<���ټ����Ϊ-1ʱ������
	short call_kwh_gap;		///<�ٻ���ȼ����Ϊ-1ʱ���ٻ�
	short send_time_gap;	///<��ʱ�����Ϊ-1ʱ����ʱ

	unsigned short yx_begin;		///<ң����ʼ��ַ
	unsigned short yx_end;			///<ң�Ž�����ַ
	
	unsigned short yc_begin;		///<ң����ʼ��ַ
	unsigned short yc_end;			///<ң�������ַ
	
	unsigned short yx_soe_begin;	///<����˳���¼��ʼ��ַ
	unsigned short yx_soe_end;		///<����˳���¼������ַ
	

	unsigned short pi_data_begin;	///<����������ʼ��ַ
	unsigned short pi_data_end;		///<�������������ַ

	unsigned short add_yc_begin;	///<�ܼ�ң����ʼ��ַ
	unsigned short add_yc_end;		///<�ܼ�ң�������ַ


	unsigned short t0;		///<t0��ʱ���������ӳ�ʱ
	unsigned short t1;		///<t1��ʱʱ�䣬����һ��I֡��U֡�󣬱�����t1ʱ���ڽ��յ��Է���ȷ�ϣ�������Ϊ���������⣬��Ҫ�ر���������
	unsigned short t2;		///<t2��ʱʱ�䣬����һ��I֡����t2ʱ����δ���յ��µ�I֡������S֡����ȷ��
	unsigned short t3;		///<t3��ʱʱ�䣬���յ��Է�һ֡���ĺ󣬾���t3ʱ��û�н��յ��µı��ģ���Ҫ��Է�����U֡����

	/**
	@brief ������Ĭ��ֵ
	*/
	void set_default(void)
	{
		call_all_data_gap = 900;//���ټ��Ĭ��ֵ900��
		call_kwh_gap = 1200;	//�ٻ���ȼ��Ĭ��ֵ1200��
		send_time_gap = 300;	//��ʱ���300��

		yx_begin = 0xF0;
		yx_end = 0xFF;
		yc_begin = 0x00;
		yc_end = 0x7F;
		yx_soe_begin = 0x80;
		yx_soe_end = 0x81;
		pi_data_begin = 0xA0;
		pi_data_end = 0xDF;
		add_yc_begin = 0x86;
		add_yc_end = 0x89;

		//���³�ʱΪ����Ҫ���뼶��λ
		t0 = 30;
		t1 = 15;
		t2 = 10;
		t3 = 20;

	};

	/**
	@brief CDT������ṹ���캯��
	*/
	CDT_CONFIG()
	{
		set_default();
	};
};

#endif