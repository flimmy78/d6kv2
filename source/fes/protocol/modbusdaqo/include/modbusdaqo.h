/**
@file modbusdaqo.h
@brief ��ȫmodbus��Լ����ͷ�ļ�

@author chenkai 
@version 1.0.0
@date 2016-07-21
*/

#ifndef MODBUSDAQO_H_
#define MODBUSDAQO_H_

#ifdef WIN32
#ifdef FES_MODBUSDAQO_PROTOCOL_DLL
#define	FES_MODBUSDAQO_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_MODBUSDAQO_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_MODBUSDAQO_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "fes/fesshare.h"
#include "modbusdaqo_type_def.h"

#include <QTime>
/**
@brief ��ȫmodbus��Լ��վ��

��ȫmodbus��Լ�࣬�̳���ת����Լ������
*/
class FES_MODBUSDAQO_PROTOCOL_EXPORT modbusdaqo: public Base_protocol
{
	enum SIZE_MODBUSDAQO
	{
		MAX_BUF_SIZE = 1024,///<����������
	};
public:
	explicit modbusdaqo(const int& channel);
	~modbusdaqo();

	/**
	@brief ��Լ����������

	���øú�����ʼһ��modbus��Լ����
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief ���ķ���
	*/
	int modbusdaqo_send_proc(void);
	/**
	@brief �ж�֡���,����֡���
	У�鱨�ģ��������������Ĵ�����
	*/
	TYPE_IDENTIFICATION calc_frame();

	/**
	@brief �ж�֡���,����֡���
	У�鱨�ģ��������������Ĵ�����
	*/
	int proc_modbusdaqo_frame(TYPE_IDENTIFICATION frametype);

	/**
	@brief ����modbus��Լ������Ϣ

	�����������ӽ�����������modbus��Լ������Ϣ
	*/
	virtual void init_protocol(void);

	//�������º�������Ҫע������ܷ��������������κ���Ҫ�ر����ӵĴ��󣬷���-1		
	/**
	@brief ����ң�ű���
	*/
	int deal_modbusdaqo_code1( void );
	/**

	@brief ����ң�ⱨ��
	*/
	int deal_modbusdaqo_code3( void );

	/**
	@brief �ٻ�ң�ű���
	*/
	int call_modbusdaqo_code1(void);
	
	/**
	@brief �ٻ�ң�ⱨ��
	*/
	int call_modbusdaqo_code3(void);

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

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) ;

	int deal_yk_frame();

private:
	unsigned char m_modbusdaqo_buf[MAX_BUF_SIZE];  ///< �洢һ֡�����Ĵ�������

	int m_yc_all_num ;
	int m_yx_all_num ;
	int m_acc_handle_flag ;
	int m_protocol_flag ;
	int m_unit_flag ;
	bool m_call_yc_flag ;
	bool m_call_yx_flag ;

	QTime m_timeout_timer;
	QTime m_yc_timer ;
	QTime m_yx_timer ;

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

	const MODBUSDAQO_CONFIG* const	m_config_ptr;		//��Լ�����޷��޸�����
};

/**
@brief ��ȡ��Լʵ����C���ӿ�
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief ��ȡ���ݿ����õ�C���ӿ�
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT void get_config(void);

/**
@brief ��ȡ�汾��C���ӿ�
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT char* get_version(void);

/**
@brief ��ȡ������Ϣ��C���ӿ�
*/
extern "C" FES_MODBUSDAQO_PROTOCOL_EXPORT char* get_desc(void);


#endif