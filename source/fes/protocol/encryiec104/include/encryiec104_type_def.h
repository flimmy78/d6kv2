/**
@file iec104_type_def.h
@brief ����104��Լ�������Ͷ���ͷ�ļ�

@author chenkai 
@version 1.0.0
@date 2015-11-29
*/

#ifndef ENCRY_IEC104_TYPE_DEF_H_
#define ENCRY_IEC104_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"
/**
@brief ����IEC104���ͱ�ʶ

ֻ�г��˳��õģ��ɸ�����Ҫ�͹�Լ�ĵ�����
*/
enum TYPE_IDENTIFICATION
{
	M_SP_NA_1 = 0x01,	///<������Ϣ
	M_SP_TA_1 = 0x02,	///<��ʱ��ĵ�����Ϣ(��ʱ��SOE)
	M_DP_NA_1 = 0x03,	///<˫����Ϣ
	M_DP_TA_1 = 0x04,	///<��ʱ���˫����Ϣ(��ʱ��SOE)

	M_ST_NA_1 = 0x05,	///<��λ����Ϣ����ѹ���ֽ�ͷ��
	M_ST_TA_1 = 0x06,	///<��ʱ��Ĳ�λ����Ϣ����ѹ���ֽ�ͷ��
	M_BO_NA_1 = 0x07,	///<��վԶ���ն�״̬

	M_ME_NA_1 = 0x09,	///<����ֵ����һ��ֵ
	M_ME_TA_1 = 0x0a,	///<��ʱ��Ĳ���ֵ
	M_ME_NB_1 = 0x0b,	///<����ֵ����Ȼ�ֵ
	M_ME_NC_1 = 0x0d,	///<����ֵ���̸���ֵ
	M_IT_NA_1 = 0x0f,	///<�ۻ���
	M_IT_TA_1 = 0x10,	///<��ʱ��ĵ�������

	M_PS_NA_1 = 0x14,	///<��״̬����ĳ��鵥����Ϣ
	M_ME_ND_1 = 0x15,	///<����Ʒ�������Ĺ�һ������ֵ

	M_SP_TB_1 = 0x1e,	///<��CP56Time2a ʱ��ĵ�����Ϣ
	M_DP_TB_1 = 0x1f,	///<��ʱ��CP56Time2a��˫����Ϣ
	M_ME_TD_1 = 0x22,	///<��ʱ��CP56Time2a�ĵĲ���ֵ����һ��ֵ
	M_ME_TE_1 = 0x23,	///<��ʱ��CP56Time2a�ĵĲ���ֵ����Ȼ�ֵ
	M_ME_TF_1 = 0x24,	///<��ʱ��CP56Time2a�ĵĲ���ֵ���̸�����
	M_IT_TB_1 = 0x25,	///<��ʱ��CP56Time2a���ۼ���

	C_SC_NA_1 = 0x2d,	///<��������
	C_DC_NA_1 = 0x2e,	///<˫������
	C_RC_NA_1 = 0x2f,	///<��������

	C_IC_NA_1 = 0x64,	///<���ٻ�����
	C_CI_NA_1 =	0x65,	///<�ۼ����ٻ�����
	C_CS_NA_1 = 0x67	///<ʱ��ͬ������
};

/**
@brief ���������ڱ����е�λ��
*/
enum BYTE_POS_DEF
{
	START_ID = 0,

	NS       = 2,
	NS_L     = 2,
	NS_H     = 3,
	NR       = 4,
	NR_L     = 4,
	NR_H     = 5,

	APDU_ID  = 6,
	NUM      = 7,

	REASON   = 8,
	REASON_L = 8,
	REASON_H = 9,
	ADDR     = 10,
	ADDR_L   = 10,
	ADDR_H   = 11,

	INDEX_NO = 12,
	INDEX_L  = 12,
	INDEX_M  = 13,
	INDEX_H  = 14,
	

	YK_FRAME_LENTH = 0x0e,

	ACTIVE_CON = 14,
	VALUE    = 14,

	SYNC_VALUE = 20		
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
@brief ���ĳ����ֽ�
*/
enum DATA_CON
{
	FIRST = 0X68,   //<IEC104����ͷ
	ENCRY_FIRST = 0X16,   //<����IEC104����ͷ
	U_LEN = 0X04,   //<U֡�̶�����
	S_LEN = 0X04,    //<S֡�̶�����
	VSQ_SQ = 0x80,   //<VSQ
	VSQ_SINGLE = 1,
	NEGATE = 1,    //ȡ����־
	STATE_SP_ON = 1,  //�����λ
	STATE_SP_OFF = 0,  //�����λ
	STATE_DP_ON = 0X02,  //˫���λ
	STATE_DP_OFF = 0X01,  //˫���λ
	QDI_GENERAL_CALL = 0x14  //���ٻ��޶���QDI
};

/**
@brief S֡
*/
enum U_TYPE
{
	STARTDT_ACT   = 0x04|0x03,    // 0x07
	STARTDT_CON   = 0x08|0x03,    // 0x0b
	STOPDT_ACT    = 0x10|0x03,    // 0x13
	STOPDT_CON    = 0x20|0x03,    // 0x23
	TESTFR_ACT    = 0x40|0x03,    // 0x43
	TESTFR_CON    = 0x80|0x03     // 0x83
};

/**
@brief ����ԭ��
*/
enum COT_TYPE
{
	COT_PERCYC	     = 1,   //����/ѭ��
	COT_BACK         = 2,  //����ɨ��
	COT_SPONT        = 3,  //ͻ��
	COT_INIT         = 4,  //��ʼ��
	COT_REQ		     = 5,  //���������
	COT_ACT          = 6,  //����
	COT_ACTCON       = 7,  //����ȷ��
	COT_DEACT        = 8,  //ֹͣ����
	COT_DEACTCON     = 9,  //ֹͣ����ȷ��
	COT_ACTTERM      = 10, //�������
 	COT_RETREM		 = 11, //Զ����������ķ�����Ϣ
	COT_RETLOC		 = 12, //������������ķ�����Ϣ
    COT_FILE		 = 13, //�ļ�����

	COT_INTROGEN     = 20, //���ٻ�
	COT_PIINTROGEN   =37,  //�������ٻ�

	COT_UNKOWN_TYPE  = 44,
	COT_UNKOWN_COT   = 45,
	COT_UNKOWN_ADDR  = 46,
	COT_UNKOWN_INDEX = 47
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
@brief ASDU�е���Ϣ�壬�����ֽ�������
*/
struct    DATA_STRUCT
{
	unsigned char	buf[ASDU_DATA_LEN];
};

/**
@brief ���͡�������Žṹ
*/
union NSR_FORMAT
{
	int16u num;
};

/**
@brief I֡ʱ������ͷ
*/
struct I_FORMAT    // I : Information Transmit Format ��ŵ���Ϣ�����ʽ
{
	int16u ns;   //�������к�
	int16u nr;   //�������к�
};

/**
@brief S֡ʱ������ͷ
*/
struct S_FORMAT    // S : Numbered Supervisory Function ��ŵļ��ӹ��ܸ�ʽ
{
	unsigned char  s_1;
	unsigned char  s_2;
	int16u nr;
};

/**
@brief U֡ʱ������ͷ
*/
struct U_FORMAT    // U : Unnumbered Control Function ����ŵĿ��ƹ��ܸ�ʽ
{
	unsigned char  u_type;
	unsigned char  u_2;
	unsigned char  u_3;
	unsigned char  u_4;
};


/**
@brief ���ٻ�/ң�ؽṹ
*/
struct C_STRUCT
{

	int16u  index_no;	///<��Ϣ���ַ
	unsigned char pos_holder;	///<104�涨��
	unsigned char qdi;
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
@brief APCI : Application Protocol Control Information Ӧ�ù�Լ������Ϣ
*/
struct P104_APCI  
{
	unsigned char    start_id;    // buf[0] �����ַ�
	unsigned char    apdu_len;    // buf[1] APDU�ĳ���

	union				 // buf[2-5] ������
	{
		I_FORMAT type_i;
		S_FORMAT type_s;
		U_FORMAT type_u;
		unsigned char    buf[CTL_FLD];
	};
};

/**
@brief ASDU : Application Service Data Unit Ӧ�÷������ݵ�Ԫ
*/
struct P104_ASDU
{
	unsigned char    type;        // buf[6] type identification ���ͱ�ʶ
	unsigned char    vsq;		 // buf[7] variable structure qualifier �ɱ�ṹ�޶���

	int16u    cot;       // buf[8-9] cause of transmission ����ԭ��
	int16u   addr;       // buf[10-11] common address Ӧ�÷������ݵ�Ԫ������ַ

	union
	{
		C_STRUCT cs;
		SYNC_TIME_STRUCT sts;
		DATA_STRUCT    data;
	};
};

/**
@brief 104��Ϣ���ַ�ṹ
*/
struct INFO_ADDR
{
	int16u addr1;
	int8u addr2;
};

/**
@brief YX�ṹ
*/
struct SEQ_M_NA_1
{
	int16u addr1;
	int8u addr2;
	unsigned char yx_value;
};

/**
@brief M_PS_NA_1YX�ṹ
*/
struct SEQ_M_PS_NA_1
{
	int16u value;
	int16u statuscheck;
	unsigned char quanity;
};

/**
@brief ��ʱ��yxsoe�ṹ
*/
struct	SEQ_M_TA_1
{
	INFO_ADDR	info_addr;
	unsigned char value;
	CP24Time2a short_time;
};

/**
@brief ��ʱ��yxsoe�ṹ
*/
struct SEQ_M_TB_1
{
	INFO_ADDR	info_addr;
	unsigned char value;
	CP56TIME long_time;
};

/**
@brief M_IT_NA_1��Ƚṹ
*/
struct SEQ_M_IT_NA_1
{
	int32s value;
	unsigned char quanity;
};

/**
@brief M_ME_NA_1ң��ṹ
*/
struct SEQ_M_ME_NA_1
{
	int16s value;
	unsigned char quanity;
};

/**
@brief M_ME_NC_1ң��ṹ
*/
struct SEQ_M_ME_NC_1
{
	floats value;
	unsigned char quanity;
};

/**
@brief M_ME_TA_1ң��ṹ
*/
struct SEQ_M_ME_TA_1
{
	INFO_ADDR	info_addr;
	int16s value;
	unsigned char quanity;
	CP24Time2a short_time;
};

/**
@brief APDU : Application Protocol Data Unit Ӧ�ù�Լ���ݵ�Ԫ
*/
typedef struct P104_APDU  
{
	P104_APCI    apci;  //6�ֽ�
	P104_ASDU    asdu;  //249�ֽ����
};

#pragma pack()


/**
@brief ENCRY_IEC104������ṹ
*/
struct ENCRY_IEC104_CONFIG
{
	unsigned short  yx_type;	//ң������
	unsigned short soe_type;	//SOE����
	unsigned short  yc_type;	//ң������
	unsigned short  kwh_type; //�������

	unsigned short  yk_type;	//ң�����ͣ������˫��


	short call_all_data_gap;///<���ټ����Ϊ-1ʱ������
	short call_kwh_gap;		///<�ٻ���ȼ����Ϊ-1ʱ���ٻ�
	short send_time_gap;	///<��ʱ�����Ϊ-1ʱ����ʱ

	unsigned short yx_begin;	///<ң����ʼ��ַ
	unsigned short yc_begin;	///<ң����ʼ��ַ
	unsigned short yk_begin;	///<ң����ʼ��ַ
	unsigned short kwh_begin;	///<�����ʼ��ַ

	unsigned short t0;		///<t0��ʱ���������ӳ�ʱ
	unsigned short t1;		///<t1��ʱʱ�䣬����һ��I֡��U֡�󣬱�����t1ʱ���ڽ��յ��Է���ȷ�ϣ�������Ϊ���������⣬��Ҫ�ر���������
	unsigned short t2;		///<t2��ʱʱ�䣬����һ��I֡����t2ʱ����δ���յ��µ�I֡������S֡����ȷ��
	unsigned short t3;		///<t3��ʱʱ�䣬���յ��Է�һ֡���ĺ󣬾���t3ʱ��û�н��յ��µı��ģ���Ҫ��Է�����U֡����

	unsigned short test_fr_cnt;	//δ�ظ��Ĳ���֡��ֵ
	unsigned short k;		///kֵ����k��I֡δ�õ��Է�S֡ȷ��ʱ��ֹͣ���䣬ȡֵ��Χ1-32767
	unsigned short w;		///<wֵ����ٽ��յ��Է�w��I֡ʱ����Է�����S֡��ȡֵ��Χ1-32767

	bool check_ns_nr;		///<��鷢�ͽ�����ţ���Ŵ�����Ҫ�ر����ӣ�1Ϊ��飬0Ϊ�����
	bool chg_to_soe;		///<ң��תsoe��1ΪSOEתң�أ�0Ϊ��ת 
	unsigned char soe_to_chg;	///<soeתң�ţ�1ΪSOEתң�أ�0Ϊ��ת
	unsigned char grp_yc_check_chg_flag;	///<���鵥��ң�ż���λ��־��1Ϊ��飬0Ϊ�����

	/**
	@brief ������Ĭ��ֵ
	*/
	void set_default(void)
	{
		yx_type = M_SP_NA_1;
		soe_type = M_SP_TB_1;
		yc_type = M_ME_NC_1;
		kwh_type = M_IT_NA_1;
		yk_type = C_DC_NA_1;

		call_all_data_gap = 900;//���ټ��Ĭ��ֵ900��
		call_kwh_gap = 1200;	//�ٻ���ȼ��Ĭ��ֵ1200��
		send_time_gap = 300;	//��ʱ���300��

		yx_begin = 0x0001;
		yc_begin = 0x4001;
		kwh_begin = 0x6401;
		yk_begin = 0x6001;

		//���³�ʱΪ����Ҫ���뼶��λ
		t0 = 30;
		t1 = 15;
		t2 = 10;
		t3 = 20;

		test_fr_cnt = 3;
		k = 12;
		w = 8;

		check_ns_nr = 0;
		chg_to_soe = 0;
		soe_to_chg = 0;
		grp_yc_check_chg_flag = 0;

	};

	/**
	@brief IEC104������ṹ���캯��
	*/
	ENCRY_IEC104_CONFIG()
	{
		set_default();
	};
};

#endif