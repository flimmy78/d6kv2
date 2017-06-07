/**
@file iec104.h
@brief 104��Լ����ͷ�ļ�

@author lujiashun 
@version 1.0.0
@date 2016-08-30
*/

#ifndef IEC104_H_
#define IEC104_H_

#ifdef WIN32
#ifdef FES_IEC104_PROTOCOL_DLL
#define	FES_IEC104_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_IEC104_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_IEC104_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "iec104_type_def.h"

/**
@brief 104��Լ��վ��

ת��104��Լ�࣬�̳���ת����Լ������
*/
class FES_IEC104_PROTOCOL_EXPORT Iec104: public Base_protocol
{
public:

	enum NET_RECV///<������Ϣ
	{
		RCV_INVALID_MESSAGE = -1,///<104���ķǷ����򵥵��ж�֡�����Ƿ�Ϸ�
		RCV_NO_MESSAGE = 0,///<û�н��յ����ģ��������
		RCV_OK = 1,///<���յ�����
	};

	enum INVALID_TIME///< �Ƿ����ĵ�����������
	{
		MAX_INVALID_MESSAGE_TIMES = 10,
	};

	enum RECV_STEP///<�������
	{
		RECV_APCI = 0,
		RECV_ASDU = 1,
	};

	enum FRAME_TYPE///<֡���
	{
		INVALID_FRAME = -1,///<�Ƿ�֡

		I_FRAME = 0,	///< I֡
		S_FRAME = 1,
		U_FRAME_STARTDT = 2,
		U_FRAME_TESTFR = 3,
		U_FRAME_STOPDT = 4,
		U_FRAME_TESTFR_ACK = 5,
	};

	enum SIZE_104///< 104��Լ�г��ȶ���
	{
		MAX_BUF_SIZE = 255,///<����������
		MAX_LENGTH = 253,
		MAX_ASDU_INF = 244,
		APDU_INFORM_LEN = 9,
		US_FRAME_LENGTH = 6,
		US_DATA_LENGTH  = 4,
		CTRL_FRAME_LEN  = 16,
	};

	enum  ///< ��Ӧ���ٻ�ʱһ�ⱨ���е���Ϣ�������
	{
		YC_FRAME_NUM = 24, 
		YC_CHG_FRAME_NUM = 24,
		YX_FRAME_NUM = 64,
		PI_FRAME_NUM = 20,

		WAVE_FILE_MAX =	64,
	};


public:
	explicit Iec104(const int& channel);
	~Iec104();

	/**
	@brief ��Լ����������

	���øú�����ʼһ��104��Լ����
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief ���ķ���
	*/
	int Iec104_send_proc(void);

	/**
	@brief �ж�֡���
	���104��Լ���ĵ����S֡��U֡����I֡�����������У׼�������
	*/
	FRAME_TYPE judge_frame();

	/**
	@brief ����104��Լ������Ϣ

	�����������ӽ�����������104��Լ������Ϣ
	*/
	virtual void init_protocol(void);

	//�������º�������Ҫע������ܷ��������������κ���Ҫ�ر����ӵĴ��󣬷���-1
	/**
	@brief ���Ͳ���֡
	*/
	int send_test_fr(void);
	
	/**
	@brief ����I֡����
	*/
	int deal_i_frame(void);
	
	/**
	@brief ����S֡����
	*/
	int deal_s_frame(void);
	
	/**
	@brief ��Ӧ��վ���Ա���
	*/
	int deal_test_fr(void);
	
	/**
	@brief ������վ������Ӧ����
	*/
	int deal_test_ack(void);
	
	/**
	@brief ȷ�ϼ��������,����ʼ��������
	*/
	int deal_start_dt(void);
	
	/**
	@brief ���������
	*/
	int send_start_DT(void);

	/**
	@brief ȷ��ֹͣ��������
	*/
	int deal_stop_dt(void);
	
	/**
	@brief ֹͣ��������
	*/
	int send_stop_DT(void);
	
	/**
	@brief ������Ч֡
	*/
	int deal_error_frame(void);

	/**
	@brief ����S֡
	*/
	int send_s_frame(void);
	
	/**
	@brief ���ٻ�
	*/
	int call_all_data(void);
	
	/**
	@brief �ٻ����
	*/
	int call_pi_data(void);
	
	/**
	@brief ʱ��ͬ��
	*/
	int sync_time(void);
	

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
	@brief ����M_ME_TE_1ң��
	*/
	int deal_sequence_M_ME_TE_1(void);
	/**
	@brief ����M_ME_TF_1ң��
	*/
	int deal_sequence_M_ME_TF_1(void);
	

	/**
	@brief ����M_IT_NA_1���
	*/
	int deal_sequence_M_IT_NA_1(void);

	/**
	@brief ����M_IT_TB_1���
	*/
	int deal_sequence_M_IT_TB_1(void);
	
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
	@brief �������
	*/
	int proc_yt_frame(void);

	/**
	@brief ������㱨��
	@para in  ytno ң����
	@para in  set_value ���ֵ			
	@para in  yt_flag ѡ��/ִ��/ȡ����	0��ѡ�� 1��ִ�� 2��ȡ��
	*/
	virtual int send_yt_frame(unsigned char ykno,float set_value,unsigned char yt_flag);

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd);

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf);

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

	int proc_wave_frame();
private:
	unsigned char m_apdu_buf[MAX_BUF_SIZE];  ///< �洢һ֡�����Ĵ�������

	int m_msg_invalid_times;///< ���յ��Ƿ����ĵĴ���

	int m_i_frame_count;///< I֡����
	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;

	Timer	m_t0_timer;		//��ʱ��ʱ��
	Timer	m_t1_timer;
	Timer	m_t2_timer;
	Timer	m_t3_timer;

	Timer	m_call_all_data_timer;	//���ټ�ʱ��
	Timer	m_call_kwh_timer;		//�ٵ�ȼ�ʱ
	Timer	m_sync_time_timer;		//��ʱ��ʱ

	bool	m_bt1_timeout;			//t1��ʱ�ж���������������I֡��U֡
	bool	m_bt2_timeout;			//t2��ʱ�ж���������������I֡
	bool	m_bstartdt_flag;		//��·��ʼ����־
	bool	m_btestfr_flag;			//����֡���ͱ�־
	bool	m_bSFrame_flag;				//S֡���ͱ�־

	int m_v_recv;///< V(R)   ���ռ�����
	int m_v_send;///< V(S)   ���ͼ�����
	int m_v_ack;///<  Ack    ȷ�϶Է�����ȷ���յ�������ֵ�I��ʽ֡

	int m_send_yx_index;///< ��ǰ���͵�ң������
	int m_send_yc_index;///< ��ǰ���͵�ң������

	int m_send_pi_index;///< ��ǰ���͵ĵ������
	int m_send_chg_yc_index;  ///<��ǰ���͵ı�λң������

	short	m_common_addr;	//������ַ

	unsigned char m_current_ykoder;	//��ǰ�·�ң��״̬

	WAVE_LIST m_wave_file_list[WAVE_FILE_MAX];
	WAVE_LIST_RUN m_cur_wave_file;
	unsigned char m_Manual_wave;	//�Ƿ��ֶ��ٻ�¼��
	unsigned char m_wave_num_pos;

	const IEC104_CONFIG* const	m_config_ptr;		//��Լ�����޷��޸�����

};

/**
@brief ��ȡ��Լʵ����C���ӿ�
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief ��ȡ���ݿ����õ�C���ӿ�
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT void get_config(void);

/**
@brief ��ȡ�汾��C���ӿ�
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT char* get_version(void);

/**
@brief ��ȡ������Ϣ��C���ӿ�
*/
extern "C" FES_IEC104_PROTOCOL_EXPORT char* get_desc(void);


#endif
