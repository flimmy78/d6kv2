/**
@file iec104.h
@brief 104��Լ����ͷ�ļ�

@author chenkai 
@version 1.0.0
@date 2015-04-07
*/

#ifndef IEC101_H_
#define IEC101_H_

#ifdef WIN32
#ifdef FES_IEC101_PROTOCOL_DLL
#define	FES_IEC101_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_IEC101_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_IEC101_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "iec101_type_def.h"

/**
@brief 101��Լ��վ��

ת��101��Լ�࣬�̳���ת����Լ������
*/
class FES_IEC101_PROTOCOL_EXPORT Iec101: public Base_protocol
{
public:
	explicit Iec101(const int& channel);
	~Iec101();

	/**
	@brief ��Լ����������

	���øú�����ʼһ��101��Լ����
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief ���ķ���
	*/
	int Iec101_send_proc(void);

	/**
	@brief ���Ľ���
	*/
	int Iec101_receive_proc(void);

	/**
	@brief ����101��Լ������Ϣ

	�����������ӽ�����������101��Լ������Ϣ
	*/
	virtual void init_protocol(void);

private:
	/**
	@brief ��λ��·
	*/
	int reset_link( void ) ;

	/**
	@brief ������·״̬
	*/
	int request_link_status( void ) ;

	/**
	@brief ���ٻ�
	*/
	int call_all_data( void ) ;

	/**
	@brief ʱ��ͬ��
	*/
	int sync_time( void ) ;

	/**
	@brief �ٻ�һ������
	*/
	int call_classone_data( void ) ;

	/**
	@brief �ٻ���������
	*/
	int call_classtwo_data( void ) ;

	/**
	@brief �ٻ��������
	*/
	int call_kwh_data( void ) ;

	/**
	@brief ����̶�֡
	*/
	int deal_fixed_frame( void ) ;

	/**
	@brief ����ɱ�֡
	*/
	int deal_unfixed_frame( void ) ;

	/**
	@brief ������Ч֡
	*/
	int deal_error_frame(void);

	//////����Ϊyx�Ĵ�������������
	/**
	@brief ����M_SP_NA_1ң��
	*/
	int deal_sequence_M_SP_NA_1(void);
		/**
	@brief ����M_DP_NA_1ң��
	*/
	int deal_sequence_M_DP_NA_1(void);
		/**
	@brief ����M_PS_NA_1ң��
	*/
	int deal_sequence_M_PS_NA_1(void);

//////����Ϊyxsoe�Ĵ�������������
	/**
	@brief M_SP_TA_1ң��SOE
	*/
	int deal_sequence_M_SP_TA_1(void);
		/**
	@brief M_DP_TA_1ң��SOE
	*/
	int deal_sequence_M_DP_TA_1(void);
		/**
	@brief M_SP_TB_1ң��SOE
	*/
	int deal_sequence_M_SP_TB_1(void);
		/**
	@brief M_DP_TB_1ң��SOE
	*/
	int deal_sequence_M_DP_TB_1(void);
	
//////����Ϊyc�Ĵ�������������

	/**
	@brief ����M_ME_NA_1 9��11ң��
	*/
	int deal_sequence_M_ME_NA_1(void);
	/**
	@brief ����M_ME_TA_1ң��
	*/
	int deal_sequence_M_ME_TA_1(void);
	/**
	@brief ����M_ME_NC_1ң��
	*/
	int deal_sequence_M_ME_NC_1(void);
	/**
	@brief ����M_ME_ND_1ң��
	*/
	int deal_sequence_M_ME_ND_1(void);
	

	/**
	@brief ����M_IT_NA_1���
	*/
	int deal_sequence_M_IT_NA_1(void);
	
	/**
	@brief ����ң��
	*/
	int proc_yk_frame(void);

	int deal_prot_frame(void);


	/**
	@brief ����ң�ر���
	@para in  ykno ң�غ�
	@para in  bclosed �غ�/�ط�			true���غ� false���ط�
	@para in  yk_flag ѡ��/ִ��/ȡ����	0��ѡ�� 1��ִ�� 2��ȡ��
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);

	/**
	@brief ������㱨��
	@para in  ytno ң����
	@para in  set_value ���ֵ			
	@para in  yt_flag ѡ��/ִ��/ȡ����	0��ѡ�� 1��ִ�� 2��ȡ��
	*/
	virtual int send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag);

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd);
	
	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf){return 0;};

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

private:
	unsigned char m_rec_buf[MAX_BUF_SIZE];  ///< �洢һ֡�����Ĵ�������

	Timer	m_t0_timer;		//��ʱ��ʱ��

	Timer	m_call_all_data_timer;	//���ټ�ʱ��
	Timer	m_call_kwh_timer;		//�ٵ�ȼ�ʱ
	Timer	m_sync_time_timer;		//��ʱ��ʱ

	unsigned char m_current_ykoder;	//��ǰ�·�ң��״̬

	const IEC101_CONFIG* const	m_config_ptr;		//��Լ�����޷��޸�����
	short	m_common_addr;	//������ַ

	bool m_t0_timeout;			//t0��ʱ�ж�����
	bool m_islink_busy ;			//��·���ñ�־
	int m_link_flag;		//��·��־
	bool m_request_link_flag ; //������·��־
	bool m_reset_link_flag ;//��λ��·��־

	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;
	int m_calltwodata_cnt ;

	unsigned char m_mainstation_FCB ; //��վFCB
	unsigned	char	m_substation_FCB ;		//��վFCB
	unsigned	char	m_substation_ACD ;		//��վ���Ӷ�վ  ACD=0����ʾ��վ��1���û�����

	unsigned char m_resend_count ;		 //�ط�����
	unsigned char m_resend_limit ;		//�ط�������
};

/**
@brief ��ȡ��Լʵ����C���ӿ�
*/
extern "C" FES_IEC101_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief ��ȡ���ݿ����õ�C���ӿ�
*/
extern "C" FES_IEC101_PROTOCOL_EXPORT void get_config(void);

/**
@brief ��ȡ�汾��C���ӿ�
*/
extern "C" FES_IEC101_PROTOCOL_EXPORT char* get_version(void);

/**
@brief ��ȡ������Ϣ��C���ӿ�
*/
extern "C" FES_IEC101_PROTOCOL_EXPORT char* get_desc(void);

#endif