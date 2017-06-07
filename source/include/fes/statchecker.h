/**
@file statchecker.h
@brief ͨ��״̬����������ļ�
@author zhangyun 
@version 1.0.0
@date 2012-11-15
*/

#ifndef STAT_CHECKER_H_
#define STAT_CHECKER_H_

#include "ace/Task.h"

class ACE_Thread_Mutex;

/**
@brief ���ͨ��״̬��

ѭ����鱾��ֵ�������ͨ����״̬
*/
class Stat_checker : public ACE_Task<ACE_MT_SYNCH>
{
	//�����ַ�����
	enum
	{
		SHORTCHAR_LEN = 24,
		DEFAULTCHAR_LEN = 64,
		NORMALCHAR_LEN = 128,
		LONGCHAR_LEN = 256,
		CHECK_STAT_TIME_SPAN = 2
	};

public:
	explicit Stat_checker(ACE_Thread_Mutex& check_watch_load_mutex);
	/**
	@brief �������캯��

	ֻ������ʵ�֣���ֹ����
	*/
	Stat_checker(const Stat_checker&);

	/**
	@brief ��ֵ�����

	ֻ������ʵ�֣���ֹ����
	*/
	Stat_checker operator = (const Stat_checker&);

	/**
	@brief ��������
	*/
	virtual ~Stat_checker();

	/**
	@brief ����
	*/
	virtual int open (void *args = 0);

	/**
	@brief ֹͣ
	*/
	virtual int close (u_long flags = 0);

	/**
	@brief ͨ��״̬����߳���

	������check_stat��Ȼ������һ��ʱ�䣬200ms ?
	*/
	virtual int svc(void);

private:
	/**
	@brief ͨ��״̬������

	���˼��ͨ��״̬������Ҫ����Ϣ���л�ȡͨ����switch policy���Զ����ֶ�
	ע����ǰ��Ҫ����check_watch_mutex,����ʹ��
	��:
	ACE_Guard<ACE_Thread_Mutex> guard (m_check_watch_mutex)�����������ֶ�ȥ����
	*/
	virtual int check_stat(void);
	/**
	@brief ��ͨ���л�
	���switch policy���Զ�����ͨ��CHANPARA�е�parent_channel���ݣ��л��������״̬������ͨ����
	�������ʧ�ܣ���ͨ��child_channel���ݣ��л��������״̬������ͨ����
	*/
	void switch_channel(const int& channel_no);
	/**
	@brief ͨ������
	��ͨ��״̬�����仯��������Ӧ������
	*/
	void channel_event(int channel_no);
	/**
	@brief ͨ���л�����
	��ͨ������״̬�����仯��������Ӧ������
	*/
	void switch_channel_event(int channel_no, bool main_back);
	/**
	@brief ���ͨ������
	��ͨ�������������TRUE�����򷵻�FALSE
	*/
	bool check_channel_event(int channel_no);
	/**
	@brief ���ͨ��״̬
	ִ�м��ͨ��״̬,��ǿ��ִ������״̬���л�
	*/
	void check_channel_status(int channel_no);

private:
	ACE_Thread_Mutex&	m_check_watch_load_mutex;	//Ϊ�˼��ء����ؾ����״̬���Ļ���
	unsigned char*		m_mail_buf;		//�ʼ����棬������ڵ��������ܵ��ʼ�����
	int m_host_no;		//�����ڵ��
};
#endif