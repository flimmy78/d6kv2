#ifndef FES_SHARE_H_
#define FES_SHARE_H_

#include "fes/fesscdshm.h"
#include <time.h>

const char* const DDEFAULT_ENV = "NBENV";

enum
{
	MAX_FE_SRV_NUM = 16,		///<ǰ�û��������
	MAX_PROTOCOL_NUM = 128,		///<����Լ����
	MAX_GPS_NUM = 4,			///<���GPS����
	MAX_RTU_NUM = 16385,		///<����ն�����
	MAX_CHAN_NUM = MAX_RTU_NUM,	///<���ͨ������

	MAX_YC_NUM = 0x200000,		///<���ң�����
	MAX_YX_NUM =  MAX_YC_NUM,	///<���ң�ŵ���
	MAX_KWH_NUM = 0x80000,		///<�������
};

enum
{
	DEFAULT_GRP_NO = 1,			///<Ĭ�����
	CODE_LENTH = 24,			///<���볤��
	SHORTCHAR_LEN = 24,			///���ַ�����
	SHORT_DESC_LENTH = 64,		///<����������
	LONG_DESC_LENTH = 255,		///<����������
	DEV_NAME_LEN = 64,			///<�豸��ַ����
	REC_LEN = 4096,				///ͨ�����ջ���������
	TRN_LEN = REC_LEN,			///ͨ�����ͻ���������
	MSG_MNG_PORT = 7899			///���ͱ��Ķ˿ں�
};

/**
@brief �豸����
*/
enum DEVTYPE
{
	SERIAL,		///<�����豸
	TCP_CLIENT,	///<TCP�ͻ���
	TCP_SERVER,	///<TCP������
	MOXA,		///<MOXA�豸
	UDP			///<UDP�豸
};


/**
@brief ͨ��״̬
*/
enum CHANSTAT
{
	CHANUNCHECK, ///<�����
	CHANSEARCH,	 ///<ͨ������
	CHANRUN,	 ///<ͨ������
	CHANSTOP,	 ///<ͨ��ֹͣ
	CHANRATE,	 ///<ͨ������
	CHANUNSYN	 ///<ͨ��ʧ��
};

/**
@brief ��������
*/
enum DEBUGTYPE
{
	COLLECTDBG,		///<�ɼ�����
	PROTODBG,		///<��Լ����
	PORTWATCHDBTG   ///<�˿�ֵ�����
};

/**
@brief ң�ط���״̬
*/
enum YKRET
{
	SUCCESS,		///<�ɹ�
	MSG_FAIL,		///<����ʧ��
	ENCRYPT_FAIL   ///<����У��ʧ��
};



/**
@brief ǰ��ϵͳ��Ϣ
*/
typedef struct  
{
	int  	host_no;			///<�ڵ��
	int		master_host_no;		///<��ǰ�ýڵ��

	int		protocol_num;		///<��Լ����
	int		gps_num;			///<GPS������
	int		max_chan_no;		///<���ͨ����
	int		max_rtu_no;			///<����ն˺�
	time_t	curr_time;			///<��ǰʱ��
}FE_SYS_DATA;

/**
@brief ͨ�������ṹ
ʹ��ʱ���ʼ�������轫 default_attend_host ��Ϊ-1����֤����ͨ��ѭ��ʱ�ж�������ȷ
*/
typedef struct
{
	int		order;						///<ͨ�����
	char	desdribe[SHORT_DESC_LENTH];	///<ͨ����
	char	protocol[CODE_LENTH];		///<��Լ����

	unsigned char	gps;				///<GPS��־��0Ϊ��GPS��1ΪGPS
	unsigned char	gps_priority;		///<��ͨ����GPS_INFO�����е����
	unsigned char	check;				///<ͨ�������־��0�����飬1����
	unsigned char	valid;				///<ͨ��ʹ�ñ�ʶ��0Ϊ��ʹ�ã�1Ϊʹ��
	
	int	common_addr;       		///<������ַ
	int	master_addr;		///<��վ��ַ

	int	last_channel;		///<������ϵ�е���һ��ͨ���ţ�-1����û����(����Ϊ���,���ն����)
	int	next_channel;		///<������ϵ�е���һ��ͨ���ţ�-1����û����

	int child_rtu;			///<���-RTU��
	unsigned char	dev_type;			///<�豸����

	//�豸��صĲ���
	char	local_dev[DEV_NAME_LEN];		///<�����豸��ַ����TCP server����
	short	local_port;						///<���ض˿ںţ���TCP server��UDP����
	char	dev_name[DEV_NAME_LEN];			///<�豸��ַ
	short	port;							///<�˿ڣ�TCP server��ʹ�ø��ֶΣ���ʹ��back_up_port
	char	backup_dev_name[DEV_NAME_LEN];	///<���豸��ַ
	short	back_up_port;					///<���˿ں�

	short	band;									///<������
	unsigned char	check_bit;						///<У��λ
	unsigned char	stop_bit;						///<ֹͣλ
	unsigned char	data_bit;						///<����λ
	unsigned char	flow;							///<������

	//ͨ��״̬��صĲ���
	unsigned char	initial_stat;		///<��ʼͨ��״̬
	short		stop_time_limit;		///<ͨ��ֹͣʱ��
	short		bit_error_rate;			///<�����ʱ�׼

	short	yc_num;		///<ң��������¹��ն˲�����֮�ͣ���ͬ
	short	yx_num;		///<ң�Ÿ���
	short	kwh_num;	///<��ȸ���

	short	default_attend_host;	///<Ĭ��ֵ����ڵ㣬�������������д-1������ϵͳ�Զ�����Ĭ��ֵ����ڵ�
}CHANPARA;

/**
@brief ��Կ�����ṹ
*/
typedef struct
{
	unsigned char oper_key_no ;//�������Ĺ�Կ��� 1 - 4
	unsigned char cur_key_no[ 4 ] ;//��ǰ��Կ��� 1 - 50
	unsigned char new_key_no[ 4 ] ;//�¹�Կ��ţ������ڸ��¹�Կ���� 1 - 50
}ENCRYPT_KEYNO;

/**
@brief �ն˲����ṹ
*/
typedef struct
{
	int		order;				///<�ն����
	int		last_channel;		///<��ͨ����

	int		last_rtu;			///��һ��RTU��-1����û����һ��
	int		next_rtu;			///<��һ��RTU�����Ϊͨ���ĵ�һ��rtuno����û����

	char	rtu_code[CODE_LENTH];		///<�ն˴���
	char	desdribe[SHORT_DESC_LENTH];	///<�ն���

	short	yc_num;		///<ң�����
	short	yx_num;		///<ң�Ÿ���
	short	kwh_num;	///<��ȸ���

	unsigned char	valid;			///<�ն�ʹ�ñ�ʶ,0Ϊ��ʹ�ã�1Ϊʹ��

	ENCRYPT_KEYNO encry_key_info;//��Կ��Ϣ
}RTUPARA;


/**
@brief ͨ�����ݽṹ
ʹ��ʱ���ʼ�������轫 default_attend_host ��Ϊ-1����֤����ͨ��ѭ��ʱ�ж�������ȷ
*/
typedef struct
{
	CHANSTAT	stat;					///<ͨ��״̬
	time_t		last_recv_time;			///<�ϴ��յ����ݵ�ʱ��
	time_t		last_connected_time;	///<�ϴ����ӳɹ���ʱ��
 	time_t      last_up_time;			///<�ϴ�����ʱ��
 	time_t		last_down_time;			///<�ϴ�ֹͣʱ��  

	time_t		last_sync_time;			///<�ϴζ�ʱ�ɹ�ʱ��

	int			frame_check_err;		///<һ֡�����е��������
	int			frame_corr_cnt;			///<����ȷ���ĵĸ���
	int			frame_err_cnt;			///<�ܴ����ĵĸ���
	float		bit_err_rate;			///<������

	unsigned char		master_flag;			///��ͨ����־
	unsigned char		auto_switch;			///<��վ�Զ����ű�־��0Ϊ�û�ǿ��������1Ϊ��վ�Զ�����
	short		curr_attend_host;		///<��ǰֵ���

	unsigned char		sync_time_success;	///<��ʱ�ɹ���־

	float			recv_count;		///<ͨ�������ֽ���
	float			send_count;		///<ͨ�������ֽ���

}CHANDATA;


/**
@brief GPS���ݽṹ
*/
typedef struct
{
	time_t	update_time;	///<GPS���ݸ��µ�ʱ��
	time_t	last_check_time;///<<��һ��ȫ����ʱ��ʱ��
	int		channel;		///<GPSͨ����
	FETIME	time;			///GPS�ṩ��ʱ��
	int		attend_host_no;	///GPS��ֵ�������
}GPS_INFO;



/**
@brief ͨ��״̬
*/
typedef struct
{
	short		channel_no;		///<ͨ�����
	CHANSTAT	stat;			///<ͨ��״̬
	float		bit_err_rate;	///<������
	int			master;			///<��ͨ����־
}FE_CHAN_STAT;

/**
@brief ���Ľṹ��
*/
typedef struct
{
	enum
	{
		MAX_MSGLENTH = 512
	};
	int channel_no;						//ͨ����
	FETIME time;						//����ʱ��
	bool  recv_flag;					//���ձ�־ 1���գ�0����;
	short lenth;						//���ĳ���
	unsigned char msg[MAX_MSGLENTH];	//����֡
}FE_MSG;

//�ṹ������Ҫ����䶯,ǣ���ֽڶ���
struct YC_PARA_DB
{
	unsigned short  yc_no;//ң�����
	double slope;	//б��
	double ordinate;//�ؾ�
	unsigned short  ter_no;//�ն����
};

struct YX_PARA_DB
{
	unsigned char negate;	//ȡ����0Ϊ��ȡ����1Ϊȡ��
	unsigned short ter_no;//�ն����
	unsigned short  yx_no;//ң�����

};

struct KWH_PARA_DB
{
	unsigned short  kwh_no;//������
	double full_code;	//����
	double slope;		//б�� = ����/���
	unsigned short  ter_no;//�ն����

};

#endif