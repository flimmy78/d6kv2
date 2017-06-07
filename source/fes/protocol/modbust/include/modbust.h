/**
@file modbust.h
@brief modbus��ʱ���Լ����ͷ�ļ�

@author lujishun 
@version 1.0.0
@date 2016-3-22
*/

#ifndef MODBUST_H_
#define MODBUST_H_

#ifdef WIN32
#ifdef FES_MODBUST_PROTOCOL_DLL
#define	FES_MODBUST_PROTOCOL_EXPORT __declspec(dllexport) 
#else
#define	FES_MODBUST_PROTOCOL_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_MODBUST_PROTOCOL_EXPORT
#endif

#include "fes/baseprotocol.h"
#include "fes/timer.h"
#include "modbust_type_def.h"


/**
@brief modbust��Լ��վ��

ת��modbus��Լ�࣬�̳���ת����Լ������
*/
class FES_MODBUST_PROTOCOL_EXPORT Modbust: public Base_protocol
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
// 		MAX_BUF_SIZE = 255,///<����������
		MAX_BUF_SIZE = 512,///<����������
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
	};


public:
	explicit Modbust(const int& channel);
	~Modbust();

	/**
	@brief ��Լ����������

	���øú�����ʼһ��modbus��Լ����
	*/
	virtual void protocol_proc(void);

protected:
	/**
	@brief ���ķ���
	*/
	int modbust_send_proc(void);

	/**
	@brief �ж�֡���
	���modbus��Լ���ĵ����S֡��U֡����I֡�����������У׼�������
	*/
	FRAME_TYPE judge_frame();

	/**
	@brief ����modbus��Լ������Ϣ

	�����������ӽ�����������modbus��Լ������Ϣ
	*/
	virtual void init_protocol(void);

	//�������º�������Ҫע������ܷ��������������κ���Ҫ�ر����ӵĴ��󣬷���-1
	
	/**
	@brief �����ٻ�ң��֡����
	*/
	int call_yc_data(void);

	/**
	@brief �����ٻ�ң��֡����
	*/
	int call_yx_data(void);

	/**
	@brief �����ٻ�ң��֡����
	*/
	int call_ym_data(void);
	
	/**
	@brief ����M_ME_TA_1 0x10ң��
	*/
	int deal_sequence_M_ME_NA_1(void);

	/**
	@brief ����M_IT_NA_1 0x10 ���
	*/
	int deal_sequence_M_IT_NA_1(void);

	/**
	@brief ����M_SP_NA_1 0x0fң��
	*/
	int deal_sequence_M_SP_NA_1(void);


	/**
	@brief ����ɱ�������
	*/
	int deal_i_frame(void);
	
	/**
	@brief ������Ч֡
	*/
	int deal_error_frame(void);

	/**
	@brief ȷ��
	*/
	int confirm_link(void);

	/**
	@brief modbus CRC�������
	*/
	unsigned short CRC16 ( unsigned char *arr_buff, int len);

		/**
	@brief ����ң�ر���
	@para in  ykno ң�غ�
	@para in  bclosed �غ�/�ط�			true���غ� false���ط�
	@para in  yk_flag ѡ��/ִ��/ȡ����	0��ѡ�� 1��ִ�� 2��ȡ��
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag);

	virtual int send_yt_frame(unsigned char ytno, float set_value, unsigned char yt_flag){return 0;};

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd){return 0;};

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf){return 0;};

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) {return 0;};

private:
	unsigned char m_apdu_buf[MAX_BUF_SIZE];  ///< �洢һ֡�����Ĵ�������

	int m_msg_invalid_times;///< ���յ��Ƿ����ĵĴ���

	int m_i_frame_count;///< I֡����
	int m_yc_all_num;
	int m_yx_all_num;
	int m_pi_all_num;

	bool	m_confirm_link_flag ;	//ȷ�ϱ�־

	int8u	m_call_yc_data_flag;		//�ٻ�ң���ʶ��0:�����ٻ� 1����Ҫ�ٻ� 2:�����ٻ�
	int8u	m_call_yx_data_flag;		//�ٻ�ң�ű�ʶ��0:�����ٻ� 1����Ҫ�ٻ� 2:�����ٻ�
	int8u	m_call_ym_data_flag;		//�ٻ�ң����ʶ��0:�����ٻ� 1����Ҫ�ٻ� 2:�����ٻ�

	int m_v_recv;///< V(R)   ���ռ�����
	int m_v_send;///< V(S)   ���ͼ�����
	int m_v_ack;///<  Ack    ȷ�϶Է�����ȷ���յ�������ֵ�I��ʽ֡

	int m_send_yx_index;///< ��ǰ���͵�ң������
	int m_send_yc_index;///< ��ǰ���͵�ң������

	int m_send_pi_index;///< ��ǰ���͵ĵ������
	int m_send_chg_yc_index;  ///<��ǰ���͵ı�λң������

	short	m_common_addr;	//������ַ

	const MODBUST_CONFIG* const	m_config_ptr;		//��Լ�����޷��޸�����
};

/**
@brief ��ȡ��Լʵ����C���ӿ�
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT Base_protocol* get_protocol(const int& channel);

/**
@brief ��ȡ���ݿ����õ�C���ӿ�
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT void get_config(void);

/**
@brief ��ȡ�汾��C���ӿ�
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT char* get_version(void);

/**
@brief ��ȡ������Ϣ��C���ӿ�
*/
extern "C" FES_MODBUST_PROTOCOL_EXPORT char* get_desc(void);


#endif