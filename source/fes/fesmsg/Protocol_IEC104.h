#ifndef PROTOCOL_IEC104_H_
#define PROTOCOL_IEC104_H_
#include "msgview_string.h"


#pragma once
#define FIRST       0x68
#define S_LEN        4
//#define BYTE unsigned char

#ifndef BYTE
typedef unsigned char BYTE; 
#endif


/********************************************����ΪIEC104��Լ����********************************************************/

static QString remind_first_error =  _C2Q("(����ͷ����ȷ) ");
static QString remind_first = _C2Q("(����ͷ) ");
static QString remind_second_error =  _C2Q("(���ĳ��Ȳ���ȷ) ");
static QString remind_len =  _C2Q("(���ĳ���:");
static QString remind_second_type_s =  _C2Q("(֡����: S֡) ");
static QString remind_second_type_i =  _C2Q("(֡����: I֡) ");
static QString remind_second_type_u =  _C2Q("(֡����: U֡ ");
static QString remind_control_s=  _C2Q("(S֡������) ");
static QString remind_control_u =  _C2Q("(U֡������) ");
static QString remind_start_act =  _C2Q("��������) ");
static QString remind_start_con=  _C2Q("����ȷ��) ");
static QString remind_stop_act =  _C2Q("ֹͣ����) ");
static QString remind_stop_con=  _C2Q("ֹͣȷ��) ");
static QString remind_test_act =  _C2Q("���Լ���) ");
static QString remind_test_con =  _C2Q("����ȷ��) ");

static QString remind_send_no =  _C2Q("�������к�:");
static QString remind_recv_no =  _C2Q("�������к�:");

static QString remind_tab =  "\r";
static QString remind_space = " ";
static QString remind_leftkuohao = "(";
static QString remind_rightkuohao = ")";

static QString remind_type = _C2Q("����ֵ:");
static QString remind_call_limit = _C2Q("�ٻ��޶���(QOI):");
static QString remind_num =  _C2Q("��Ϣ��Ŀ:");
static QString remind_nonsequenced =  _C2Q("��˳��");
static QString remind_sequenced =  _C2Q("˳��");
static QString remind_transmit_reason =  _C2Q("����ԭ��:");
static QString remind_transmit_reason_burst_3 =  _C2Q("ͻ��");
static QString remind_transmit_reason_activation_6 =  _C2Q("����");
static QString remind_transmit_reason_activation_confirm_7 =  _C2Q("����ȷ��");

static QString remind_transmit_reason_activation_stop_8 =  _C2Q("ֹͣ����");
static QString remind_transmit_reason_activation_stop_confirm_9 =  _C2Q("ֹͣ����ȷ��");
static QString remind_transmit_reason_activation_teminate_10 =  _C2Q("������ֹ");
static QString remind_transmit_reason_respond_station_call_20 =  _C2Q("��Ӧվ�ٻ�");
static QString remind_respond_station_call_20 =  _C2Q("��Ӧվ�ٻ�");
static QString remind_transmit_addr =  _C2Q("������ַ:");
static QString remind_monitor = _C2Q("���ӷ���");
static QString remind_no_time_single_signal =  _C2Q("����ʱ�굥��ң��");
static QString remind_time_single_signal =  _C2Q("��ʱ�굥��ң��");
static QString remind_siq = _C2Q("��Ʒ�������ʵĵ�����Ϣ(SIQ):");
static QString remind_open = _C2Q("��λ");
static QString remind_close = _C2Q("��λ");

static QString remind_no_time_double_signal =  _C2Q("����ʱ��˫��ң��");
static QString remind_time_double_signal =  _C2Q("��ʱ��˫��ң��");
static QString remind_diq =  _C2Q("��Ʒ�������ʵ�˫����Ϣ(DIQ)��");
static QString remind_open_double = _C2Q("��λ");
static QString remind_close_double = _C2Q("��λ");
static QString remind_uncertainty_double = _C2Q("��ȷ�����м�״̬");

static QString remind_single_command = _C2Q("������");
static QString remind_sco = _C2Q("������(SCO):");
static QString remind_no_other = _C2Q("������Ķ���");
static QString remind_short_pulse = _C2Q("���������ʱ��");
static QString remind_long_pulse = _C2Q("���������ʱ��");
static QString remind_sustained_output = _C2Q("�������");
static QString remind_reserve =_C2Q("����");

static QString remind_double_command = _C2Q("˫����");
static QString remind_dco = _C2Q("˫����(DCO) ");
static QString remind_no_allowed = _C2Q("������");
static QString remind_open_ctl = _C2Q("�ط�");
static QString remind_close_ctl = _C2Q("�غ�");

static QString remind_time_single_command = _C2Q("��ʱ��ĵ�����");

static QString remind_time_double_command = _C2Q("��ʱ���˫����");


static QString remind_no_time_measuring =  _C2Q("����ʱ�겻��Ʒ�������Ĺ�һ������ֵ");

static QString remind_time_quality_measuring = _C2Q("��ʱ���Ʒ�������Ĺ�һ������ֵ");
static QString remind_measuring =  _C2Q("����ֵ");
static QString remind_qds =  _C2Q("Ʒ��������(QDS) ");
static QString remind_no_overflow =  _C2Q("δ���");
static QString remind_overflow =  _C2Q("���");
static QString remind_unblock =  _C2Q("δ������");
static QString remind_block =  _C2Q("������");
static QString remind_no_replace =  _C2Q("δ��ȡ��");
static QString remind_replace =  _C2Q("��ȡ��");
static QString remind_current_value =  _C2Q("��ǰֵ");
static QString remind_no_current_value =  _C2Q("�ǵ�ǰֵ");
static QString remind_valid =  _C2Q("��Ч");
static QString remind_invalid =  _C2Q("��Ч");


static QString remind_no_time_control_measuring =  _C2Q("��������һ��ֵ");
static QString remind_qos =  _C2Q("�趨�����޶���(QOS) ");
static QString remind_execute = _C2Q("ִ��");
static QString remind_select = _C2Q("ѡ��");

static QString remind_time_control_measuring =  _C2Q("��ʱ�����������һ��ֵ");


static QString remind_initialization = _C2Q("��ʼ������");
static QString remind_coi = _C2Q("��ʼ��ԭ��(COI) ");
static QString remind_coi_0 = _C2Q("���ص�Դ����");
static QString remind_coi_1 = _C2Q("�����ֶ���λ");
static QString remind_coi_2 = _C2Q("Զ����λ");
static QString remind_coi_other = _C2Q("����");
static QString remind_coi_unchanged = _C2Q("δ�ı䵱�ز����ĳ�ʼ��");
static QString remind_coi_changed = _C2Q("�ı䵱�ز����ĳ�ʼ��");


static QString remind_total_call = _C2Q("���ٻ�");
static QString remind_clock_sync = _C2Q("ʱ��ͬ��");

static QString remind_energy_call = _C2Q("����ٻ�");
static QString remind_energy_limit = _C2Q("���ܼ������ٻ������޶���(QCC)");
static QString remind_energy_fre_no_reset = _C2Q("���᲻��λ");
static QString remind_energy_fre_reset = _C2Q("���Ḵλ");
static QString remind_energy_counter_reset = _C2Q("��������λ");
static QString remind_energy_call_1 = _C2Q("�����������1��");
static QString remind_energy_call_2 = _C2Q("�����������2��");
static QString remind_energy_call_3 = _C2Q("�����������3��");
static QString remind_energy_call_4 = _C2Q("�����������4��");
static QString remind_energy_call_5 = _C2Q("�����������5��");



static QString remind_info_addr =  _C2Q("��Ϣ���ַ:");

static QString remind_yx_addr =  _C2Q("ң�����:");
static QString remind_yc_addr =  _C2Q("ң�����:");
static QString remind_yk_addr =  _C2Q("ң�����:");
static QString remind_yt_addr =  _C2Q("ң�����:");
static QString remind_ym_addr =  _C2Q("ң�����:");


static QString remind_me_na = _C2Q("����ң��ֵ:");

static QString remind_exe = _C2Q("ң��ִ�� ");
static QString remind_sel = _C2Q("ң��ѡ�� ");
static QString remind_ms = _C2Q("����:");
static QString remind_minute = _C2Q("����:");
static QString remind_hour = _C2Q("ʱ:");
static QString remind_day = _C2Q("��:");
static QString remind_month = _C2Q("��:");
static QString remind_year = _C2Q("��:");
static QString remind_control = _C2Q("���Ʒ��� ");

/********************************************����ΪIEC104��Լ����********************************************************/


enum TYPE_IDENTIFICATION
{
	SP_NA_1  = 0x01,
	DP_NA_3  = 0x03,
	ME_NA_9  = 0x09,
	ME_NC_13 = 0x0d,
	DC_45    = 0x2d,
	DC_46    = 0x2e,
	PD_47    = 0x2f,
	SP_48    = 0x30,
	SP_50    = 0x32,
	CALL	= 0x64,
	SET_TIME = 0x67,
	//���
	ST_NA_5 = 0x05,//��λ����Ϣ
	BO_NA_7 = 0x07,
	ME_NB_11 = 0x0B,
	IT_NA_15 = 0x0F,
	PS_NA_20 = 0x14,
	ME_ND_21 = 0x15,
	SP_TB_30 = 0x1E,
	DP_TB_31 = 0x1F,
	ST_TB_32 = 0x20,
	BO_TB_33 = 0x21,
	ME_TD_34 = 0x22,
	ME_TE_35 = 0x23,
	ME_TF_36 = 0x24,
	IT_TB_37 = 0x25,
	EP_TD_38 = 0x26,
	EP_TE_39 = 0x27,
	EP_TF_40 = 0x28,

	//���Ʒ���
	SC_NA_45 = 0x2D,
	DC_NA_46 = 0x2E,
	SE_NA_48 = 0x30,
	SC_NA_58 = 0x3A,
	DC_NA_59 = 0x3B,
	RC_NA_60 = 0x3C,
	SE_TA_61 = 0x3D,
	SE_TB_62 = 0x3E,
	SE_TC_63 = 0x3F,
	BO_NA_64 = 0x40,
	//���Ʒ����ϵͳ��Ϣ
	IC_NA_100 = 0x64,
	CI_NA_101 = 0x65,
	RD_NA_102 = 0x66,
	CS_NA_103 = 0x67,
	RP_NA_105 = 0x69,
	TS_NA_107 = 0x71,
	M_EI_NA_70 = 0x46

};
enum U_TYPE
{
	STARTDT_ACT   = 0x04|0x03,    // 0x07
	STARTDT_CON   = 0x08|0x03,    // 0x0b
	STOPDT_ACT    = 0x10|0x03,    // 0x13
	STOPDT_CON    = 0x20|0x03,    // 0x23
	TESTFR_ACT    = 0x40|0x03,    // 0x43
	TESTFR_CON    = 0x80|0x03     // 0x83
};

enum DATA_LEN
{ 
	COMMON_ADDR_LEN = 2,///<������ַ����
	COT_LEN = 2,		///<����ԭ�򳤶�
	INDEX_LEN     = 3,	///<��Ϣ���ַ����
	CTL_FLD = 4,		///<�����򳤶�
	HEAD_LEN      = 10,	///<���Ŀ�ʼ����Ϣ��ĳ���
	ASDU_DATA_LEN = 256	///<ASDU��Ϣ�峤�ȣ����ر����壬��Ϣ��ύ�ɷ�����������
};

typedef union
{
	unsigned short int val1;
	struct
	{
		unsigned char b1;
		unsigned char b2;
	}val2;
}BYTE2_104;

//16λ�޷�������
struct int16u_104
{
	BYTE2_104 word;
	void operator = (unsigned short int n)
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2_104 one;
		one.val1 = n;
		word.val2.b1 = one.val2.b2;
		word.val2.b2 = one.val2.b1;
#else
		word.val1 = n;
#endif
	}

	operator unsigned short int ()
	{
#if (defined(__sun) && defined(__sparc) || defined(_AIX) || defined(__hpux))
		BYTE2_104_104 one;
		one.val2.b1 = word.val2.b2;
		one.val2.b2 = word.val2.b1;
		return one.val1;
#else
		return word.val1;
#endif
	}
};
/**
@brief ����ԭ��
*/
enum COT_TYPE
{
	COT_SPONT        = 3,
	COT_ACT          = 6,
	COT_ACTCON       = 7,
	COT_DEACT        = 8,
	COT_DEACTCON     = 9,
	COT_ACTTERM     = 10,

	COT_INRO         = 20,

	COT_UNKOWN_TYPE  = 44,
	COT_UNKOWN_COT   = 45,
	COT_UNKOWN_ADDR  = 46,
	COT_UNKOWN_INDEX = 47
};

/**
@brief �����е�ʱ���ʽ
*/
typedef struct CP56TIME
{
	BYTE ms[2];
	BYTE minute;
	BYTE hour;
	BYTE day:5;
	BYTE wday:3;
	BYTE month;
	BYTE year;
};

/**
@brief ASDU�е���Ϣ�壬�����ֽ�������
*/
typedef    struct    DATA_STRUCT
{
	BYTE    buf[ASDU_DATA_LEN];
};

/**
@brief ���͡�������Žṹ
*/
union NSR_FORMAT
{
	int16u_104 num;
	struct
	{
		BYTE lsb;
		BYTE msb;
	};
};

/**
@brief I֡ʱ������ͷ
*/
typedef struct I_FORMAT    // I : Information Transmit Format ��ŵ���Ϣ�����ʽ
{
	NSR_FORMAT ns;
	NSR_FORMAT nr;
};

/**
@brief S֡ʱ������ͷ
*/
typedef struct S_FORMAT    // S : Numbered Supervisory Function ��ŵļ��ӹ��ܸ�ʽ
{
	BYTE  s_1;
	BYTE  s_2;

	NSR_FORMAT nr;
};

/**
@brief U֡ʱ������ͷ
*/
typedef struct U_FORMAT    // U : Unnumbered Control Function ����ŵĿ��ƹ��ܸ�ʽ
{
	BYTE  u_type;
	BYTE  u_2;
	BYTE  u_3;
	BYTE  u_4;
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

/**
@brief ���ٻ�/ң�ؽṹ
*/
typedef struct C_STRUCT
{

	int16u_104  index_no;	///<��Ϣ���ַ
	BYTE pos_holder;	///<104�涨��
	BYTE qoi;
};


/**
@brief ��ʱ�ṹ
*/
typedef struct SYNC_TIME_STRUCT
{
	int16u_104  index_no;
	BYTE	pos_holder;
	CP56TIME cp56time;
};

/**
@brief ACPI : Application Protocol Control Information Ӧ�ù�Լ������Ϣ
*/
typedef struct P104_APCI  
{
	BYTE    start_id;    // buf[0] �����ַ�
	BYTE    apdu_len;    // buf[1] APDU�ĳ���

	union				 // buf[2-5] ������
	{
		I_FORMAT type_i;
		S_FORMAT type_s;
		U_FORMAT type_u;
		BYTE    buf[CTL_FLD];
	};
};

/**
@brief ASDU : Application Service Data Unit Ӧ�÷������ݵ�Ԫ
*/
typedef struct P104_ASDU
{
	BYTE    type;        // buf[6] type identification ���ͱ�ʶ
	BYTE    vsq;		 // buf[7] variable structure qualifier �ɱ�ṹ�޶���

	int16u_104    cot;       // buf[8-9] cause of transmission ����ԭ��
	int16u_104   addr;       // buf[10-11] common address Ӧ�÷������ݵ�Ԫ������ַ

	union
	{
		C_STRUCT cs;
		SYNC_TIME_STRUCT sts;
		DATA_STRUCT    data;
	};
}P104_ASDU;

/**
@brief APDU : Application Protocol Data Unit Ӧ�ù�Լ���ݵ�Ԫ
*/
typedef struct P104_APDU  
{
	P104_APCI    apci;
	P104_ASDU    asdu;
};


const double base_num=256.0;


enum SEND_REASON
{
	BURST = 0x0003,  //ͻ��
	ACTIVATION = 0x0106, //����
	ACTIVATION_CONFIRM = 0x0107, //����ȷ��
	ACTIVATION_END_CONFIRM = 0x010A//�������ȷ��
};

#endif