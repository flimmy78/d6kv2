/**
@file iec101_type_def.h
@brief 101��Լ�������Ͷ���ͷ�ļ�

@author chenkai 
@version 1.0.0
@date 2015-04-07
*/

#ifndef IEC101_TYPE_DEF_H_
#define IEC101_TYPE_DEF_H_

#include "fes/crossplatformtypes.h"

/**
@brief ���ĳ����ֽ�
*/
enum DATA_CON
{
	FIXED_FRAME_HEAD = 0x10,  //�̶�֡ͷ
	FRAME_TAIL = 0x16, //������
	UNFIXED_FRAME_HEAD = 0x68 , //�ɱ�֡ͷ
	YK_CONFIRM_HEAD = 0xE5 ,//ң����·��ȷ��֡
	STATE_SP_ON = 1,  //�����λ
	STATE_SP_OFF = 0,  //�����λ
	STATE_DP_ON = 0X02,  //˫���λ
	STATE_DP_OFF = 0X01,  //˫���λ
};
/**
@brief IEC101���ͱ�ʶ

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
	M_BO_NA_1 = 0x07,	///<32���ش�
	M_BO_TA_1 = 0x08 , ///<��ʱ���32���ش�

	M_ME_NA_1 = 0x09,	///<����ֵ����һ��ֵ
	M_ME_TA_1 = 0x0a,	///<��ʱ��Ĳ���ֵ
	M_ME_NB_1 = 0x0b,	///<����ֵ����Ȼ�ֵ
	M_ME_TB_1 = 0x0c, ///<����ֵ, ��ʱ��ı�Ȼ�ֵ
	M_ME_NC_1 = 0x0d,	///<����ֵ���̸���ֵ
	M_ME_TC_1 = 0x0e, ///<����ֵ, ��ʱ��Ķ̸�����
	M_IT_NA_1 = 0x0f,	///<�ۼ���
	M_IT_TA_1 = 0x10,	///<��ʱ����ۼ���

	M_EP_TA_1 = 0x11, ///<��ʱ��ļ̵籣���豸�¼�
	M_EP_TB_1 = 0x12, ///<��ʱ��ļ̵籣���豸���������¼�
	M_EP_TC_1 = 0x13, ///<��ʱ��ļ̵籣���豸���������·��Ϣ
	M_PS_NA_1 = 0x14,	///<����λ����ĳ��鵥����Ϣ
	M_ME_ND_1 = 0x15,	///<����Ʒ�������Ĺ�һ������ֵ

	M_SP_TB_1 = 0x1e,	///<��CP56Time2a ʱ��ĵ�����Ϣ
	M_DP_TB_1 = 0x1f,	///<��ʱ��CP56Time2a��˫����Ϣ
	M_ST_TB_1 = 0x20,   ///<��CP56Time2aʱ��Ĳ�λ����Ϣ
	M_BO_TB_1  = 0x21, ///<��CP56Time2aʱ���32���ش�
	M_ME_TD_1 = 0x22,	///<��ʱ��CP56Time2a�ĵĲ���ֵ����һ��ֵ
	M_ME_TE_1 = 0x23,	///<��ʱ��CP56Time2a�ĵĲ���ֵ����Ȼ�ֵ
	M_ME_TF_1 = 0x24,	///<��ʱ��CP56Time2a�ĵĲ���ֵ���̸�����
	M_IT_TB_1 = 0x25,	    ///<��ʱ��CP56Time2a���ۼ���
	M_EP_TD_1 = 0x26,	///<��CP56Time2aʱ��ļ̵籣���豸�¼�
	M_EP_TE_1 = 0x27,	///<��CP56Time2aʱ��ļ̵籣���豸���������¼�
	M_EP_TF_1 = 0x28,	///<��CP56Time2aʱ��ļ̵籣���豸���������·��Ϣ

	C_SC_NA_1 = 0x2d,	///<��������
	C_DC_NA_1 = 0x2e,	///<˫������
	C_RC_NA_1 = 0x2f,	///<��������
	C_SE_NA_1 = 0x30,	///<�趨ֵ����, ��һ��ֵ
	C_SE_NB_1 = 0x31,	///<�趨ֵ����, ��Ȼ�ֵ
	C_SE_NC_1 = 0x32,	///<�趨ֵ����, �̸�����
	C_BO_NA_1 = 0x33,	///<32���ش�

	M_EI_NA_1 = 0x46,	///<��ʼ������

	C_IC_NA_1 = 0x64,	///<���ٻ�����
	C_CI_NA_1 =	0x65,	///<�������ٻ�����
	C_RD_NA_1 =	0x66,	///<������
	C_CS_NA_1 = 0x67,	///<ʱ��ͬ������
	C_TS_NA_1 = 0x68,	///<��������
	C_RP_NA_1 =	0x69,	///<��λ��������
	C_CD_NA_1 = 0x6a,	///<��ʱ�������

	P_ME_NA_1 = 0x6e,	///<����ֵ����, ��һ��ֵ
	P_ME_NB_1 = 0x6f,	///<����ֵ����, ��Ȼ�ֵ
	P_ME_NC_1 =	0x70,	///<����ֵ����, �̸�����
	P_AC_NA_1 = 0x71,	///<��������

	F_FR_NA_1 =	0x78,	///<�ļ���������
	F_SR_NA_1 =	0x79,	///<�ڻ�������
	F_SC_NA_1  = 0x7a,	///<�ٻ�Ŀ¼, ѡ���ļ�, �ٻ��ļ����ٻ���
	F_LS_NA_1 = 0x7b,		///<���Ľ�,���Ķ�
	F_AF_NA_1 =	0x7c,	///<�Ͽ��ļ�,�Ͽɽ�
	F_SG_NA_1 = 0x7d,	///<��
	F_DR_TA_1 = 0x7e		///<Ŀ¼
};

//����������·������
enum M_LINK_FUNCCODE
{
	RESET = 0 ,	//��λԶ����·
	RESETUSER = 1 ,	//��λ�û�����
	TESTLINK = 2 ,	//
	SENDCONFIRM = 3 ,	//����/ȷ���û�����
	SENDNOCONFIRM = 4 ,	//����/�޻ش��û�����
	REQUESTDATA = 8 , //��������
	REQUESTLINKSTATUS = 9 ,	//�ٻ���·״̬
	REQUESTONEDATA = 10 ,	//�ٻ�һ������
	REQUESTTWODATA = 11 	//�ٻ���������
};

//�Ӷ�������·������
enum S_LINK_FUNCCODE
{
	ACK = 0,
	NACK = 1 ,	
	RESPONSE101 = 8 ,	
	NODATA = 9	,
	LINKNOWORKING = 14 ,	//��·����δ����
	LINKREDAY = 11 ,	//��·���
	LINKNOEND = 15 ,	//��·����δ���
};

typedef	struct
{
	unsigned char		FC ;		//������
	unsigned char		FCV;		//������Чλ(1��ʾ�仯��Ч)
	unsigned char		FCB;		//֡����λ
	unsigned char		PRM;		//��������λ(����Ϊ1; �Ӷ�Ϊ0)
	unsigned char		DIR;		//���䷽��λ(��->�� 0)
}ControlField;

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

//�ٻ��鶨��
#define				CALLTOTAL				20			//վ���ٻ�
#define				ALLDATAACK				10			//���ٻ�ȷ��
#define				CALLKWHTOTAL			5	//			//������ٻ�
#define				BROADCASTCALL       3 //�㲥�ٻ�1������

enum SIZE_101///< 101��Լ�г��ȶ���
{
	MAX_BUF_SIZE = 255,///<����������
	UNFIXED_DATA_LEN = 6,///<�ɱ�֡����������ӵĳ���
	FIXED_FRAME_LEN = 5, ///<�̶�֡��
	YK_CONFIRM_LEN = 1 ,///<ң����·��ȷ��֡��
};

enum FRAME_TYPE///<֡���
{
	INVALID_FRAME = -1,///<�Ƿ�֡
	VALID_FRAME = 0,///<�Ƿ�֡
	FIXED_FRAME = 1 ,///<�̶�֡
	UNFIXED_FRAME = 2 ,///<�ɱ�֡
	YKCONFIRM_FRAME = 3 ,///<ң��ȷ��֡
};

enum DEAL_ERR_TYPE
{
	DEFAULT_ERROR = -1 ,///<Ĭ�ϴ���
	DEAL_SUCCESS = 0 ,///<����ɹ�
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
@brief ң������DCO
*/
struct YKDCO
{
	unsigned char dco_se;		//ѡ��/ִ��
	unsigned char dco_qu;		
	unsigned char dco_dcs;		//˫��ң��״̬
};

/**
@brief ��ʱ�ṹ
*/
struct SYNC_TIME_STRUCT
{
	CP56TIME cp56time;
};

/**
@brief 101��Ϣ���ַ�ṹ
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

#pragma pack()


/**
@brief IEC101������ṹ
*/
struct IEC101_CONFIG
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
	unsigned short trp;

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

	};

	/**
	@brief IEC101������ṹ���캯��
	*/
	IEC101_CONFIG()
	{
		set_default();
	};
};

#endif