/**
@file femailstruct.h
@brief ǰ���ʼ��ṹ���
@author hongxiang 
@version 1.0.0
@date 2013-11-28
*/
#ifndef FES_MAIL_STRUCT_H_
#define FES_MAIL_STRUCT_H_

#include "fes/fesshare.h"
#include "fes/fesdatashm.h"

/**
@brief ǰ���ʼ�����
*/
enum MAIL_TYPE
{
	INVALID,		//�ʼ���Ч
	YK,				//ң���ʼ�
	CALLING,		//�ٻ��ʼ�
	CHANNEL_STAT,	//ͨ��״̬�ʼ�
	PORT_WATCH,		//�˿�ֵ���ʼ�
	REAL_DATA,		//ʵʱ�����ʼ�
	LOAD_PARA,		//��������
	SET_POINT,		//����ʼ�
	PROT,			//�����ʼ�
	WAVE,			//¼���ʼ�
	SEND_YX,   //ת��ң��
};


/**
@brief ǰ���ʼ��ṹ
*/
struct FE_MAIL_STRUCT
{
	MAIL_TYPE mail_type;		//�ʼ�����
	int mail_lenth;				//�ʼ�����
	int sub_information1;		//��������1������ң���ʼ�����Ҫ�и��ն˺�
	int sub_information2;		//��������2������
	unsigned char mail_body[1];	//�ɱ䳤���ʼ���

	/**
	@brief �����ʼ��ĳ���
	*/
	int get_lenth(void)
	{
		return sizeof(FE_MAIL_STRUCT);
	}
};


/**
@brief ң���ʼ��ṹ
*/
struct FE_YK_MAIL
{
	int yk_flag; //��ʶλ��0��ѡ�� 1��ִ�� 2��ȡ��
	int	channel_no;//ͨ����
	int rtu_no;	//�ն˺�
	int yk_no;	//ң�غ�
	int action;	//ң�����scada�·����0x33Ϊ�֣�0xCCΪ�ϣ�ת����Լ�̣߳�0:�֣�1:��
};

/**
@brief ����ʼ��ṹ
*/
struct FE_SETPOINT_MAIL
{
	int yt_flag; //��ʶλ��0��ѡ�� 1��ִ�� 2��ȡ��
	int	channel_no;//ͨ����
	int rtu_no;	//�ն˺�
	int yt_no;	//ң�غ�
	float setvalue;	//���ֵ
};

/**
@brief ����ʼ��ṹ
*/
struct FE_PROT_MAIL
{
	int	channel_no;//ͨ����
	int rtu_no;	//�ն˺�
	int prot_type;//������Լ����
	int cmd_lenth;//�����
	int exe_flag; //��ʶλ��0��Ԥ�� 1��ִ�� 2��ȡ��
	unsigned char prot_buf[1];	//�ɱ䳤���ʼ���
};

/**
@brief ң���ʼ��ṹ
*/
struct FE_TRANS_YX_MAIL
{
	int channel_no;//ͨ����
	int rtu_no;	//�ն˺�
	int yx_no;	//ң�ź�
	int yx_value;	//ң��ֵ
};

/**
@brief ������Լ���Ͷ���
Ŀǰ������ô������ͣ���Ҫʹ�õĻ������ֱ���������Ӣ��˵����
Ŀǰ��Ϊ���ֵĶ���û��ʹ�õı�����Լ����
*/
enum PROT_PROTOCOL_TYPE
{
	FE_PROTTYPE_0,   
	FE_PROTTYPE_1,     
	FE_PROTTYPE_2,         
	FE_PROTTYPE_3,         
	FE_PROTTYPE_4,         
	FE_PROTTYPE_5,         
	FE_PROTTYPE_6,         
	FE_PROTTYPE_7,		    
	FE_PROTTYPE_8,	    	
	FE_PROTTYPE_9,			
	FE_PROTTYPE_10,		
	FE_PROTTYPE_11,		
	FE_PROTTYPE_12,        
	FE_PROTTYPE_13,		
	FE_PROTTYPE_14,		
	FE_PROTTYPE_15,		
	FE_PROTTYPE_16,		
	FE_PROTTYPE_17,
	FE_PROTTYPE_18,		
	FE_PROTTYPE_19,		
	FE_PROTTYPE_20,		
	FE_PROTTYPE_21,		
	FE_PROTTYPE_22,		
	FE_PROTTYPE_23,		
	FE_PROTTYPE_24,		
	FE_PROTTYPE_25,		
	FE_PROTTYPE_26,	
	FE_PROTTYPE_27,		
	FE_PROTTYPE_28,
	FE_PROTTYPE_29,		
	FE_PROTTYPE_30,		
	FE_PROTTYPE_31,		
	FE_PROTTYPE_32,		
	FE_PROTTYPE_33,
	FE_PROTTYPE_34,		
	FE_PROTTYPE_35,	
	FE_PROTTYPE_IEC103,			//��׼103   				
	FE_PROTTYPE_37,				
	FE_PROTTYPE_38,				
	FE_PROTTYPE_39,			
	FE_PROTTYPE_40,			
	FE_PROTTYPE_41,			
	FE_PROTTYPE_42,			
	FE_PROTTYPE_43,			
	FE_PROTTYPE_44,			
	FE_PROTTYPE_45,			
	FE_PROTTYPE_46,			
	FE_PROTTYPE_47,			
	FE_PROTTYPE_48,			
	FE_PROTTYPE_49,			
	FE_PROTTYPE_50,			
	FE_PROTTYPE_51,			
	FE_PROTTYPE_52,			
	FE_PROTTYPE_53,			
	FE_PROTTYPE_54,			
	FE_PROTTYPE_ISA103,				//ISA��չ103��Լ
	FE_PROTTYPE_56,			
	FE_PROTTYPE_57,			
	FE_PROTTYPE_58,			
	FE_PROTTYPE_59,			
	FE_PROTTYPE_60,			
};

/**
@brief �ٻ��ʼ��ṹ
*/
struct FE_CALLING_MAIL
{
	enum CALL_TYPE		//�ٻ����
	{
		CALL_ALL,		//����
		RTU_INIT,		//�ն˳�ʼ��
		CHANNEL_INIT,	//ͨ����ʼ��
		CHECK_TIME,		//��ʱ
		CALL_KWH,		//���
		CHECK_PUBLIC_KEY ,	//��鹫Կ
		REFRESH_PUBLIC_KEY	//���¹�Կ
	};
	CALL_TYPE call_type;
	int channel_no;		//ͨ����
	int rtu_no;			//�ն˺�
};

/**
@brief ͨ��״̬�ʼ��ṹ
*/
typedef struct
{
	short		channel_no;		///<ͨ�����
	short		rtu_no;			///<ͨ����ܵ��׸��ն����
	CHANSTAT	stat;			///<ͨ��״̬
	float		bit_err_rate;	///<������
	int			master;			///<��ͨ����־

	int		last_sync_time;			///<�ϴζ�ʱ�ɹ�ʱ��
	float			recv_count;		///<ͨ�������ֽ���
	float			send_count;		///<ͨ�������ֽ���
	unsigned char		sync_time_success;	///<��ʱ�ɹ���־


}FE_CHANNEL_STAT_MAIL;

#endif;