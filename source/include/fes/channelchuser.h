/**
@file paraloader.h
@brief ǰ�ò�������������
@author zhangyun 
@version 1.0.0
@date 2012-11-16
*/
#ifndef CHANNEL_CHUSER_H_
#define CHANNEL_CHUSER_H_

#include "fes/fesshm.h"
#include "fes/protocolfactory.h"

#include <map>
#include <string>
using namespace std;

class Base_protocol;
class ACE_Thread_Mutex;


//int ret = Protocol_factory::instance()->get_all_config();

/**
@brief ǰ��ͨ������

�ṩ�����ļ��أ�ͨ��ʵ���Ĵ����ȹ���
*/
class Channel_chuser
{
	enum
	{
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		LONGCHAR_LEN = 256
	};

public:
	explicit Channel_chuser(ACE_Thread_Mutex& check_watch_load_mutex);
	/**
	@brief �������캯��

	ֻ������ʵ�֣���ֹ����
	*/
	Channel_chuser(const Channel_chuser&);
	/**
	@brief ��ֵ�����

	ֻ������ʵ�֣���ֹ����
	*/
	Channel_chuser operator = (const Channel_chuser&);

	~Channel_chuser();
	/**
	@brief �����Լ�������á�ͨ�����ն˵�

	ע��֮ǰ��Ҫ����Լ�����Ƿ�Ϊ�գ�����ǿյģ�����ֱ�ӷ��ش���
	������ȡ�ɹ��󣬿ɵ���check_channels
	*/
	int load(void);

	/**
	@brief ���ʼ�д���Լ����Ϣ����

	����ACE_Task��putq()�������ʼ�д����Ϣ����
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int put_mail(void* mail, const int& lenth);
private:
	/**
	@brief ��ȡ��Լ���ͱ�, ������Լ����

	ֻ��ϵͳ��ʼ��ʱ����һ�Σ��Ժ��ÿ��load�����漰��Լ���������Է��ڹ��캯��������load�����е���
	*/
	void generate_protocol_factory(void);
	/**
	@brief ���ù�Լ�����ö�ȡ���ܶ�ȡ��Լ��������
	*/
	void read_protocol_config(void);
	/**
	@brief �����ݿ��ж�ȡͨ������

	��ȡ�ɹ����Ѳ���д���ļ�
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_channel_para_from_db(void);
	/**
	@brief ���ļ��ж�ȡͨ������
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_channel_para_from_file(void);
	/**
	@brief �����ݿ��ж�ȡͨ������

	��ȡ�ɹ����Ѳ���д���ļ�
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_rtu_para_from_db(void);
	/**
	@brief ���ļ��ж�ȡͨ������
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_rtu_para_from_file(void);
	/**
	@brief ���ļ��ж�ȡң�����
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_yc_para_from_db(void);
	/**
	@brief ���ļ��ж�ȡң�����
	@return �ɹ����ظ�����ʧ�ܷ���-1
	*/
	int read_yc_para_from_file(YC_PARA_DB*);
	/**
	@brief ���ļ��ж�ȡң�����
	@return �ɹ����ظ�����ʧ�ܷ���-1
	*/
	int read_yx_para_from_file(YX_PARA_DB*);
	/**
	@brief ���ļ��ж�ȡң�����
	@return �ɹ����ظ�����ʧ�ܷ���-1
	*/
	int read_kwh_para_from_file(KWH_PARA_DB*);
    /**
	@brief ���ļ��ж�ȡң�Ų���
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_yx_para_from_db(void);
	/**
	@brief ���ļ��ж�ȡ���������
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	int read_kwh_para_from_db(void);

	/**
	@brief ����GPS����

	����GPSͨ����GPS���ݵĶ�Ӧ��ϵ
	*/
	void set_gps_para(void);
	/**
	@brief ��ձ��ػ���Ĳ���
	*/
	void clear_para(void);
	/**
	@brief ����ͨ��������ϵ
	*/
	void calc_channel_priority(void);
	/**
	@brief �����ն����ȼ�
	*/
	void calc_rtu_priority(void);
	/**
	@brief �����ն�ͨ����ϵ
	*/
	void calc_rtu_channel(void);
	/**
	@brief ����ͨ�������������ļ�,�ɹ�����0,ʧ�ܷ���-1
	*/
	int save_channel_para(void); 
	/**
	@brief �����ն˲����������ļ�,�ɹ�����0,ʧ�ܷ���-1
	*/
	int save_rtu_para(void); 
	/**
	@brief ����ң������������ļ�,�ɹ�����0,ʧ�ܷ���-1
	*/
	int save_yc_para(YC_PARA_DB* p_data, int rcd_cnt);
	/**
	@brief ����ң�Ų����������ļ�,�ɹ�����0,ʧ�ܷ���-1
	*/
	int save_yx_para(YX_PARA_DB* p_data, int rcd_cnt);
	/**
	@brief �����Ȳ����������ļ�,�ɹ�����0,ʧ�ܷ���-1
	*/
	int save_kwh_para(KWH_PARA_DB* p_data, int rcd_cnt);

	/**
	@brief ���ÿ��ͨ�������ı仯�����ݱ仯�����������ɾ�����Լʵ��

	���ͨ�����޵��У�ͨ������������������������е��ޣ���رղ�ɾ��֮������������޸ģ����ȹرա�ɾ�������´���
	ע����ǰ��Ҫ����check_watch_load_mutex,����ʹ��
	��:
	ACE_Guard<ACE_Thread_Mutex> guard (*m_check_watch_load_mutex)�����������ֶ�ȥ����
	*/
	void check_channels(void);

	/**
	@brief ���ͨ���������е�Ĭ�Ͻڵ��Ƿ�Ϊǰ�ýڵ㣬�����ǽ�������Ϊ�����С��ǰ�û�

	*/
	void calc_hostno_enale(int& hostno);


private:
	CHANPARA*				   m_channel_para;		///<��ȡ��ͨ������
	RTUPARA*				   m_rtu_para;			///<��ȡ���ն˲���
	int						   m_max_channel;		///<�����ݿ��ж��������ͨ����
	int						   m_max_rtu;			///<�����ݿ��ж��������RTU
	Base_protocol**			   m_protocol;			///<��Լʵ��ָ�������
	ACE_Thread_Mutex&		   m_check_watch_load_mutex;//Ϊ�˼��ء����ؾ����״̬���Ļ���

	bool					   m_bnew_protocol[MAX_CHAN_NUM];	//�ǲ����´����߳�
};

//�ļ�������
// const char* const PROTOCOLFILE_NAME = "/dat/protocol.dat";
// const char* const CHANNELFILE_NAME = "/dat/channel.dat";
// const char* const RTUFILE_NAME = "/dat/rtu.dat";

#endif