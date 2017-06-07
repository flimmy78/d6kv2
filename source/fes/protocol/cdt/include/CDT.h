/**
@file CDT.h
@brief 104��Լ����ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-09-12
*/

#ifndef CDT_H_
#define CDT_H_

#ifdef WIN32
#ifdef FES_CDT_PROTOCOL_DLL
#define	FES_CDT_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_CDT_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_CDT_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "fes/fesshare.h"
#include "CDT_type_def.h"


/**
@brief 104��Լ��վ��

ת��104��Լ�࣬�̳���ת����Լ������
*/
class FES_CDT_PROTOCOL_EXPORT CDT: public Base_protocol
{
	enum SIZE_CDT
	{
		MAX_BUF_SIZE = 1024,///<����������
		MAX_LENGTH = 253,
		MAX_ASDU_INF = 244,
		APDU_INFORM_LEN = 9,
		US_FRAME_LENGTH = 6,
		US_DATA_LENGTH  = 4,
		CTRL_FRAME_LEN  = 16,
	};

public:
	explicit CDT(const int& channel);
	~CDT();

	/**
	@brief ��Լ����������

	���øú�����ʼһ��104��Լ����
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief �ж�֡���,����֡���
	У�鱨�ģ��������������Ĵ�����
	*/
	TYPE_IDENTIFICATION calc_frame();

	/**
	@brief �ж�֡���,����֡���
	У�鱨�ģ��������������Ĵ�����
	*/
	int proc_cdt_frame(TYPE_IDENTIFICATION frametype);

	/**
	@brief ����104��Լ������Ϣ

	�����������ӽ�����������104��Լ������Ϣ
	*/
	virtual void init_protocol(void);

	//�������º�������Ҫע������ܷ��������������κ���Ҫ�ر����ӵĴ��󣬷���-1	
	/**
	@brief ����A��B��C֡ң�ⱨ��
	*/
	int deal_cdt_yc(INFO_STRUCT* info_body);
	
	/**
	@brief ����S֡����
	*/
	int deal_cdt_yx(INFO_STRUCT* info_body);
	
	/**
	@brief ��Ӧ��վ���Ա���
	*/
	int deal_cdt_yxsoe(INFO_STRUCT* info_body, int frame_count);
	
	/**
	@brief ������վ������Ӧ����
	*/
	int deal_cdt_pi(INFO_STRUCT* info_body);

	/**
	@brief �����֡ң��
	*/
	int deal_insert_frame(INFO_STRUCT* info_body);

	/**
	@brief ����ң�ر���
	@para in  ykno ң�غ�
	@para in  bclosed �غ�/�ط�			true���غ� false���ط�
	@para in  yk_flag ѡ��/ִ��/ȡ����	0��ѡ�� 1��ִ�� 2��ȡ��
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);

	virtual int send_yt_frame(unsigned char ykno, float ytvalue,unsigned char yk_flag){ return 0;};

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd){return 0;};

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf){return 0;};

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

	int deal_yk_frame(INFO_STRUCT* info_body);

private:
	unsigned char m_cdt_buf[MAX_BUF_SIZE];  ///< �洢һ֡�����Ĵ�������


	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;

	Timer	m_timeout_timer;

	Timer	m_call_all_data_timer;	//���ټ�ʱ��
	Timer	m_call_kwh_timer;		//�ٵ�ȼ�ʱ
	Timer	m_sync_time_timer;		//��ʱ��ʱ

	enum
	{
		NO_NEED = 0,
		NEED_CALL = 1,
		ON_PROCESS,
	};

	short	m_common_addr;	//������ַ
	short	m_master_addr;		//��վ��ַ

	int m_current_ykno;		//��ǰң�ص�ַ
	unsigned char m_current_ykoder;	//��ǰ�·�ң��״̬

	FETIME m_soe_time;		//soeʱ��

	const CDT_CONFIG* const	m_config_ptr;		//��Լ�����޷��޸�����

};

/**
@brief ��ȡ��Լʵ����C���ӿ�
*/
extern "C" FES_CDT_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief ��ȡ���ݿ����õ�C���ӿ�
*/
extern "C" FES_CDT_PROTOCOL_EXPORT void get_config(void);

/**
@brief ��ȡ�汾��C���ӿ�
*/
extern "C" FES_CDT_PROTOCOL_EXPORT char* get_version(void);

/**
@brief ��ȡ������Ϣ��C���ӿ�
*/
extern "C" FES_CDT_PROTOCOL_EXPORT char* get_desc(void);


#endif