/**
@file portwatch.h
@brief �˿�ֵ��������
@author zhangyun 
@version 1.0.0
@date 2012-11-15
*/
#ifndef FES_PORT_WATCH_H_
#define FES_PORT_WATCH_H_

#include "ace/Task.h"
#include <vector>
#include "fes/fesshare.h"
using namespace std;

class ACE_Thread_Mutex;
class Fe_node;
class Fes_shm;


/**
@brief �˿�ֵ����

�ṩ�˿�ֵ��״̬���͡����ա��жϹ���
*/
class Port_watch : public ACE_Task<ACE_MT_SYNCH>
{
	enum
	{
		SLEEP_TIME = 1,				///<ÿ�ζ˿�ֵ��������˯��ʱ��
		GET_MSG_WAIT = 200,			///<����Ϣ���л�ȡ�ʼ��ĵȴ�ʱ��,200����
		GET_MSG_NUM = 1,			///<ÿ�λ�ȡ��ȡ������Ϣ
		HEART_BEAT_TIME_LIMIT = 5	///<����
	};
	/**
	@brief �˿�ֵ�౨�ĵĸ�ʽ
	*/
	struct PORT_WATCH_MSG
	{
		int fe_index;				///<������
		unsigned long load_map;		///<����
	
		unsigned char give_back_flag;//�й黹ͨ��

// 		time_t msg_time;			///�ʼ�ʱ��

		int attend_gps_num;			///<��ܵ�GPS������
		GPS_INFO gps[MAX_GPS_NUM];	///<����GPS����

		int attend_channel_num;		///<��ܵ�ͨ��������

		FE_CHAN_STAT chan_stat[1];	///<ͨ��״̬��ע������鳤��Ϊ1��Ϊ�������ڴ��㹻��ǰ�����ṩһ�ּ��׵ı䳤���飬ʡȥ

		/**
		@brief ����ͨ�����������㱨�ĵĳ���
		*/
		int get_lenth(void)
		{
			return sizeof(PORT_WATCH_MSG) + ((attend_channel_num > 0 ? (attend_channel_num - 1) * sizeof(FE_CHAN_STAT) : 0));
		}

		void clear(void)
		{
			fe_index = 0;
			attend_gps_num = 0;
			attend_channel_num = 0;
			load_map = 0;
			memset(gps, 0, sizeof(gps));
		}

		static int get_lenth(const int& channel_num)
		{
			return sizeof(PORT_WATCH_MSG) + (channel_num > 0 ? (channel_num - 1) * sizeof(FE_CHAN_STAT) : 0);
		}
	};

public:

	Port_watch(ACE_Thread_Mutex& check_watch_load_mutex);

	/**
	@brief �������캯��

	ֻ������ʵ�֣���ֹ����
	*/
	Port_watch(const Port_watch&);

	/**
	@brief ��ֵ�����

	ֻ������ʵ�֣���ֹ����
	*/
	Port_watch operator = (const Port_watch&);

	/**
	@brief ��������
	*/
	virtual ~Port_watch();

	/**
	@brief ����
	*/
	virtual int open(void *args = 0);

	/**
	@brief ֹͣ
	*/
	virtual int close(u_long flags = 0);

	/**
	@brief ���ؾ����߳���

	port_watch_proc-˯��һ����
	*/
	virtual int svc(void);
private:
	/**
	@brief �����˿�ֵ���õĸ�ǰ�ýڵ�
	*/
	void create_host_nodes(void);
	/**
	@brief ���ؾ������

	�������Ƿ���-����-�����ͻ-�ӹ�
	ע����ǰ��Ҫ����check_watch_load_mutex,����ʹ��
	��:
	ACE_Guard<ACE_Thread_Mutex> guard (m_check_watch_load_mutex)�����������ֶ�ȥ����
	*/
	void port_watch(void);
	/**
	@brief ���ʼ�������ǰ�ýڵ��scada�ڵ㷢�Ͷ˿�ֵ�౨��
	*/
	int send_stat(void);
	/**
	@brief ����Ϣ����ȡ���ʼ��߳�ת�͵Ķ˿�ֵ���ʼ�
	
	�ѽ��յ�ͨ��״̬д�빲���ڴ棬ͬʱ����ÿ���ڵ��״̬
	ע����Ҫ�����ɱ����������ʼ�
	*/
	void recv_stat(void);

	/**
	@brief ��������������ڵ��ֵ���ͻ

	��������ֱ����������ڵ�ͬʱֵ��һ��������������Է��ڵ㸺���뱾��һ�����ҽڵ�űȱ���С���򱾻������ӹ�
	����Է��ڵ���Ĭ��ֵ�����Ҳ�����ӹ�
	*/
	void resolve_conflict(void);

	/**
	@brief �ӹܹ���

	����Ƿ������������е�ֹͣ������У���鱾���Ƿ��Ǹ�����С�ҽڵ����С�ģ�������нӹ�
	����Ƿ���������ֹͣ�����У���������Ǳ����ڽӹܣ�������ӹ�
	*/
	void take_over(void);

private:
	vector<Fe_node*>	m_fe_nodes;		//����ǰ�ýڵ�ļ��ϣ���������ƽ̨�е�����
	int					m_index;		//������ǰ�ýڵ㼯���е�λ��
	unsigned char*		m_mail_buf;		//�ʼ����棬������ڵ��������ܵ��ʼ�����
	ACE_Thread_Mutex&	m_check_watch_load_mutex;	//Ϊ�˼��ء����ؾ����״̬���Ļ���
	Fes_shm&			m_fes_shm;			//�����ڴ��������

	unsigned char		m_give_back_flag;		//��ǰ�÷����˹黹����,0:�� 1:��
// 	unsigned char		m_valid_msg;		//������Ч���ϱ仯λ,0:��Ч 1:��Ч ʹ����ǰ���ܹ��жϸ�ǰ�ø����ѷ����仯
	unsigned char		m_already_give_up;	//����ǰ�÷����˹黹����,0:�� 1:��
};

#endif