/**
@file baseprotocol.h
@brief ��Լ����ͷ�ļ�

@author zhangyun 
@version 1.0.0
@date 2012-10-25
*/
#ifndef FES_BASE_PROTOCOL_H_
#define FES_BASE_PROTOCOL_H_

#ifdef WIN32
#ifdef FES_PROTOCOL_API_DLL
#define	FES_PROTOCOL_API_EXPORT __declspec(dllexport) 
#else
#define	FES_PROTOCOL_API_EXPORT __declspec(dllimport) 
#endif
#else
#define	FES_PROTOCOL_API_EXPORT
#endif

#include "fes/fesshare.h"
#include "fes/fesdev.h"
#include "fes/msgtransceiver.h"
#include "fes/crossplatformtypes.h"
#include "ace/Task.h"

typedef union
{
	float yc_value;
	unsigned char yx_value;
	unsigned long kwh_value;
}real_data_value;

typedef struct
{
	short  	terminalno;             //�ն˺�
	unsigned char protocoltype;     //��Լ����  (����������Ϣ�Ľ��ͷ���) 0-->LFP, 1-->ISA ,   2-->SEL
	unsigned char gram_len;			//�������ĵĳ���
	unsigned char gram[256];		//��������
}FEP_PROT_STRU;

/////////////////////////////////¼����ؽṹ��/////////////////////////////////////////
#define IEC_101_NOF			2		//��׼101�ļ������ļ�����Ϊ16λ
#define IEC_101_NOS			1		//��׼101�ļ����������Ϊ8λ
#define FILE_NAME_LEN		512		//��׼101�ļ������ļ�����Ϊ16λ

typedef struct
{
	unsigned char STATUS:5;
	unsigned char LFD:1;
	unsigned char FOR:1;
	unsigned char FA:1;
}FILE_SOF;

typedef struct
{
	int  len;
	unsigned short info_addrL;//��ַ
	unsigned char file_type;
	unsigned char file_name[IEC_101_NOF];	//�ļ���
	int  time_sec;//1970��1��1��0��0��0��0����
	int  time_msec;
	FILE_SOF sof;
}WAVE_LIST;

typedef struct
{
	unsigned short info_addr;//�ļ���Ϣ���ַ
	int file_lenth;			//�ļ�����
	int sum_file_lenth;			//�Ѵ��ļ��ͳ���
	int cur_node_lenth;		//��ǰ���ͽڳ���
	int sum_node_lenth;		//�Ѵ��͵�ǰ�ڳ���
	int cur_phase_lenth;	//��ǰ���Ͷγ���
	char save_name[FILE_NAME_LEN];
	unsigned char file_type;
	unsigned char file_name[IEC_101_NOF];
	unsigned char cur_nodename[IEC_101_NOS];	//��ǰ���ͽ���
}WAVE_LIST_RUN;
//////////////////////////////////////////////////////////////////////////


/**
@brief ��Լ����

���й�Լ������̳и��ಢʵ��protocol_proc�ӿ�
*/
class FES_PROTOCOL_API_EXPORT Base_protocol : public ACE_Task<ACE_MT_SYNCH>
{

	//�����ַ�����
protected:
	enum
	{
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		NORMALCHAR_LEN = 128,
		LONGCHAR_LEN = 256,
		REC_BUF = 512,
		REC_LEN = 4096,
		TRN_LEN = REC_LEN
	};

	enum
	{
		NO_NEED = 0,
		NEED_CALL = 1,
		ON_PROCESS,
	};

public:
	/**
	@brief ���캯��
	@param in const int& channel ͨ����
	*/
	explicit Base_protocol(const int& channel_no);
	/**
	@brief ��������
	*/
	virtual ~Base_protocol();

	/**
	@brief ����
	*/
	virtual int open (void *args = 0);

	/**
	@brief ֹͣ
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief ��Լ����������
	*/
	virtual void protocol_proc (void) = 0;


	/**
	@brief ��Լ�����߳��壬��������ʵ��
	*/
	virtual int svc(void);

	/**
	@brief ��ʼ����Լʵ��
	
	�ɸ���ʵ����Ҫ����ʵ��
	*/
	virtual void init_protocol();
	/**
	@brief ��ͨ���˿�
	@return ʧ�ܷ���-1
	*/
	int open_dev(void);

	/**
	@brief �ر�ͨ���˿�
	*/
	void close_dev(void);

	/**
	@brief ͨ���Ƿ�� 
	@return true,ͨ���Ѵ�;false,ͨ��δ��
	*/
	bool is_open(void);

	/**
	@brief �ر�ͨ���߳�
	*/
	void exit_proc();

protected:
	/**
	@brief �������ݵ����ջ�����

	@return �����ݷ����ֽ����������ݷ���0��ʧ�ܷ���-1
	*/
	int recv_msg(void);

	/**
	@brief �ѷ��ͻ����������ݷ��ͳ�ȥ

	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int send_msg(void);

	/**
	@brief �����ɵĵ����������ȫ�ֻ���
	*/
	void buf_event(FE_DATA_EVENT_NODE& event);

	/**
	@brief �����ɵĵ��������ݷ���ȫ�ֻ���
	*/
 	void buf_fe_raw(FE_REAL_DATA_NODE& fe_real_data);

	/**
	@brief �����ɵ�ң�ط�У��Ϣ����ȫ�ֻ���
	*/
 	void buf_yk_check(FE_YK_CHECK_NODE& fe_yk_check_data);
	/**
	@brief ����ʱ��

	ʱ��ᱻд�뻺��������GPSͨ������һ����ʱ����ȫ����ʱ
	*/
	virtual void set_time(const unsigned short& year, 
						  const unsigned char& month,
						  const unsigned char& day, 
						  const unsigned char& hour, 
						  const unsigned char& minute, 
						  const unsigned char& second, 
						  const unsigned short& milisecond);
	/**
	@brief ������õ�ʱ�ĺ����ԣ�ͬʱ��һЩ��Ӧ�Ĵ���
	*/
	 virtual int check_time_ok(const unsigned short& year, 
							   const unsigned char& month, 
							   const unsigned char& day, 
							   const unsigned char& hour, 
							   const unsigned char& minute, 
							   const unsigned char& second, 
							   const unsigned short& milisecond);

	/**
	@brief ����ң�ط����ʼ�

	û��������Ҫ����Ҫ����ʵ��
	*/
	virtual void send_yk_ret(const int& rtu_no, const int& ykno, const int& yk_ret, const char* msg);

	/**
	@brief ���ɵ���������뻺��

	û��������Ҫ����Ҫ����ʵ��
	*/
	void make_event(const FETIME& soe_time, const int& data_no, const unsigned char& yx_value);

	/**
	@brief ����ң��ʵʱ���ݵ㣬�����뻺��

	û��������Ҫ����Ҫ����ʵ��
	*/
	void make_yc_real_data(const int& data_no, const float& yc_value, const unsigned char& quanity, FETIME* time = NULL);

	/**
	@brief ����ң��ʵʱ���ݵ㣬�����뻺��

	û��������Ҫ����Ҫ����ʵ��
	*/

	void make_yx_real_data(const int& data_no, const unsigned char& yx_value, const unsigned char& quanity, FETIME* time = NULL);

	/**
	@brief ����ң��ʵʱ���ݵ㣬�����뻺��

	û��������Ҫ����Ҫ����ʵ��
	*/
	void make_kwh_real_data(const int& data_no, const unsigned long& kwh_value, const unsigned char& quanity, FETIME* time = NULL);

	/**
	@brief �ʼ�������

	û��������Ҫ����Ҫ����ʵ��
	*/
	void check_event(void);

	/**
	@brief ����ң�ر���
	@para in  ykno ң�غ�
	@para in  bclosed �غ�/�ط�			true���غ� false���ط�
	@para in  yk_flag ѡ��/ִ��/ȡ����	0��ѡ�� 1��ִ�� 2��ȡ��
	*/
	virtual int send_yk_frame(unsigned char ykno, bool bclosed, unsigned char yk_flag) = 0;

	virtual int send_yt_frame(unsigned char ytno, float set_value,unsigned char yt_flag)=0;

	virtual int send_command_frame(unsigned char protocol_type,unsigned char exe_flag, unsigned char cmd_len, unsigned char* cmd)=0;

	virtual int send_WaveCommand(unsigned char Type, unsigned char* cmd_buf)=0;

	virtual int send_trans_yx(unsigned char yxno, unsigned char yx_value) = 0;

protected:
	unsigned char m_read_buf[REC_LEN];		///<��������
	unsigned char m_write_buf[TRN_LEN];		///<д������
	int  m_read_lenth;				///<���������ĳ���
	int	 m_write_lenth;				///<���������ĳ���
	
	const int	m_channel_no;			///<ͨ����
	int	m_rtu_no;				///<�ն˺�
	CHANPARA* 	m_chanpara;				///ͨ��������ָ��
	CHANDATA* 	m_chandata;				///ͨ��״̬��ָ��

	Dev_base*	m_dev_ptr;				///<ͨѶ�豸

	unsigned char*		m_yx_ptr;		///���ػ����ң��ֵ��1���ֽ�һ��ֵ��0Ϊ�ϣ�1Ϊ�֣�δˢ�µ�ֵΪUCHAR_MAX��������ACE_Numeric_Limits<T>.max()����ͬ
	float*				m_yc_ptr;		///<���ػ����ң��ֵ��δˢ�µ�ֵΪFLT_MAX
	unsigned long*		m_kwh_ptr;		///<���ػ���ĵ��ֵ��δˢ�µ�ֵδUINT_MAX 

	int8u	m_call_all_data_flag;		//���ٻ���־: 0:�����ٻ� 1����Ҫ�ٻ� 2:�����ٻ�
	int8u	m_call_kwh_data_flag;		//�ٻ���ȱ�־0:�����ٻ� 1����Ҫ�ٻ� 2:�����ٻ�
	int8u	m_sync_time_flag;			//�ٻ���ȱ�־0:�����ʱ 1����Ҫ�ٻ� 2:��ʱ����
	int8u   m_rtu_init_flag;			//�ն˳�ʼ����־
	int8u	m_check_public_key ;		//У�鹫Կ��־0:����У�� 1����ҪУ�� 2:����У��
	int8u	m_refresh_public_key ;		//���¹�Կ��־0:������� 1����Ҫ���� 2:���ڸ���

	int m_current_ykno;		//��ǰң�ص�ַ

	Msg_transceiver*	m_msg_tran_instance; ///<����ת������ʵ��ָ��

	bool m_nEnd;
};

#endif